/*
 * This source is the part of nveth - NVIDIA ndis driver for OS/2
 *
 * Copyright (C) 2007 nickk
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <driver.h>

MCastBuf AdapterMCB =
{
    NUM_MCADDRS,
    0
};

MACSpecChar AdapterSC =
{
    sizeof(MACSpecChar),
    "DIX+802.3",
    6,
    "",
    "",
    0,
    &AdapterMCB,
    1000000000,
    GDT_ADDRESS_SUPP        |
    MULTIPLE_XFER_DATA_SUPP |
    OPEN_ADAPTER_SUPP       |
    SET_STN_ADDR_SUPP       |
    STAT_ALWAYS_CURR        |
    PROMISCUOUS_SUPP        |
    BROADCAST_SUPP          |
    MULTICAST_SUPP          |
    0,
    1514,
    (dword)1514 * TX_RING_DEFAULT,
    1514,
    (dword)1514 * RX_RING_DEFAULT,
    1514,
    { 0xFF, 0xFF, 0xFF},
    0,
    DNAME,
    0,
    1,
    8
};

MACSpecStat AdapterSS =
{
    sizeof(struct MACSpecStat)
};

MACUprDisp AdapterDT =
{
    &AdapterCC,
    AdapterGenReq,
    AdapterXmitChain,
    AdapterXferData,
    AdapterRcvRelease,
    AdapterIndOn,
    AdapterIndOff
};

CommChar AdapterCC =
{
    sizeof(CommChar), 
    0,                
    0,                
    1,                
    0,                
    0,                
    DVENDOR,             
    1,                
    1,                
    0,                
    1,                
    0,                
    0,                
    SysReq,           
    &AdapterSC,       
    &AdapterSS,       
    &AdapterDT,       
    0,                
    0,
    0
};

ProtLwrDisp ProtDT;
CommChar    ProtCC;

char cDevName[MAX_DEVNAME+1] = DEV_NAME;
word wState        = ST_CLOSED;
word wIndications  = 0;
word wPktMask      = 0;

char *cRCStr[] =
{
    "Init success.",                            
    "Unable to determine device driver name." , 
    "Too many instances already installed."   , 
    "Protocol Manager is not installed."      , 
    "No DRIVERNAME entry matches driver name.", 
    ""                                        , 
    "Unable to register module."              , 
    "Unable to allocate GDT selectors"        , 
    "Unable to setup network adapter"         , 
    "Invalid parameter."                      , 
    "",
};

static word* pDataSegs[] =
{
    ((word*)&AdapterCC.CcDataSeg)    ,
    ((word*)&AdapterCC.CcSCp    ) + 1,
    ((word*)&AdapterCC.CcSSp    ) + 1,
    ((word*)&AdapterCC.CcUDp    ) + 1,
    ((word*)&AdapterCC.CcLDp    ) + 1,

    ((word*)&AdapterSC.MscMCp) + 1,
    ((word*)&AdapterSC.MscVenAdaptDesc) + 1,

    ((word*)&AdapterDT.MudCCp   ) + 1
};

static word* pCodeSegs[] =
{
    ((word*)&AdapterCC.CcSysReq     ) + 1,

    ((word*)&AdapterDT.MudGReq      ) + 1,
    ((word*)&AdapterDT.MudXmitChain ) + 1,
    ((word*)&AdapterDT.MudXferData  ) + 1,
    ((word*)&AdapterDT.MudRcvRelease) + 1,
    ((word*)&AdapterDT.MudIndOn     ) + 1,
    ((word*)&AdapterDT.MudIndOff    ) + 1
};

void Say(char far *cStr)
{
	word wNumBytes = 0;

	DosWrite(1, cStr, strlen(cStr), &wNumBytes);
}

void FixFarPointers(void)
{
	word i;

	for (i = 0; i < (sizeof(pDataSegs)/sizeof(pDataSegs[0])); i++)
	{
		*pDataSegs[i] = DevProtDS;
	}

	for (i = 0; i < (sizeof(pCodeSegs)/sizeof(pCodeSegs[0])); i++)
	{
		*pCodeSegs[i] = DevProtCS;
	}
}

#ifndef ucase
#define ucase(a)    (((a) >= 'a' && (a) <= 'z') ? ((a) -= ('a'-'A')) : (a))
#endif

extern "C" int stricmp(char far *str0, char far *str1)
{
	while(*str0 && *str1 && (ucase(*str0) == ucase(*str1)))
	{
		str0++;
		str1++;
	}

	if (!*str0 && !*str1)
	{
		return 0;
	}

	return ucase(*str0) - ucase(*str1);
}

extern "C" int aton(char far * str)
{
	int size, res = 0, mult = 1, digit;
	if (str == 0)
	{
		return 0;
	}
	size = strlen(str);
	while (size--)
	{
		digit = str[size] - '0';
		if (digit < 0 || digit > 9)
		{
			return 0;
		}
		res += digit * mult;
		mult *= 10;
	}
	return res;
}

dword allocPhysAddress(dword size)
{
	dword addr = 0;
	dword len = (size + (PAGESIZE - 1)) & ~(PAGESIZE - 1);
	word rc = DevPhysAlloc(len, 0, (dword far *)&addr);
	if (rc)
	{
		rc = DevPhysAlloc(len, 1, (dword far *)&addr);
		if (rc)
		{
			return 0L;
		}
	}
	return addr;
}

#pragma inline_depth 0
dword mapPhysAddress(dword address, dword length)
{
	word gdt = 0;
	word rc = DevAllocGDTSel((word far*)&gdt, 1);
	if (rc)
	{
		return 0;
	}
	return mapPhysAddress(gdt, address, length);
}

#pragma inline_depth 0
dword mapPhysAddress(word gdt, dword address, dword length)
{
	if (length >= (dword)0x10000)
	{
		return 0;
	}
	word offset = (word)(address & 0xFFF); 
	dword phys = address & 0xFFFFF000; 
	dword len = (length + offset + (PAGESIZE - 1)) & 0xFFFFF000; 

	word rc = DevPhysToGDTSelector(phys, (word)len, gdt);
	if (rc)
	{
		return 0;
	}
	return (((dword)gdt << 16) & 0xFFFF0000) + offset;

}

word sleephandle = 0;

void msleep(dword msecs)
{
	word rc = DevBlock(HANDLE(sleephandle), msecs, 0);
}

void udelay(word msecs)
{
	for (;msecs > 0; msecs--)
	{
		IODly(Delay500);
		IODly(Delay500);
	}
}

void far irqhandler()
{
	DevCli();
	word i = dev.IRQHandler();
	if (i)
	{
		DevCli();
		DevEOI(dev.getIRQ());
		DevClc();
	}
	else
	{
		DevSti();
		DevStc();
	}
}

void Time::init()
{
	dword ptr = (dword)SAS_SEL << 16;
	struct SAS far * sasptr = (struct SAS far *)ptr;
	ptr = ptr | sasptr->SAS_info_data;
	struct SAS_info_section far * sasinfoptr = (struct SAS_info_section far *)ptr;
	finfogdt = (struct InfoSegGDT far *)((dword)sasinfoptr->SAS_info_global << 16);
	lastsec = getsec();
	lastms = getms();
}

dword Time::diff()
{
	dword diff = 0;
	dword ms = getms();
	if (ms < lastms)
	{
		diff = (-1L - lastms) + ms;
	}
	else
	{
		diff = ms - lastms;
	}
	return diff;
}

Time time;

word Mutex::request(word block)
{
	while (DevXchg(&handle, 1))
	{
		if (!block)
		{
			return 1;
		}
		DevBlock(HANDLE(handle), -1, 0);
	}
	return 0;
}

void Mutex::release()
{
	DevXchg(&handle, 0);
	DevRun(HANDLE(handle));
}

