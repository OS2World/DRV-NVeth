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

#pragma off (unreferenced)

word adapternum = 0;

word OpenDevice(char *devName)
{
	word devHandle;
	dword action;
	int result;

	result = DosOpen(devName, &devHandle, &action, 0L, 0, 0x0001, 0x0091, 0L);

	if (!result)
	{
	        return devHandle;
	}

	return 0;
}

int TestDevice(char *devName)
{
	word devHandle;
	dword action;
	int result;

	result = DosOpen(devName, &devHandle, &action, 0L, 0, 0x0001, 0x0091, 0L);

	if (!result)
	{
		DosClose(devHandle);
	}

	return result;
}

word CallProtMan(word hPM, RqBlk far * RqB)
{
	word stat;

	stat = DosDevIOCtl(0, RqB, 0x58, 0x81, hPM);

	if (stat)
	{
		return stat;
	}

	return RqB->Status;
}

word RegisterModule(word hPM, fptr CCPtr, fptr BList)
{
	RqBlk ReqBlock = {0};

	ReqBlock.Opcode   = opRegisterModule;
	ReqBlock.Pointer1 = CCPtr;
	ReqBlock.Pointer2 = BList;

	return CallProtMan(hPM, &ReqBlock);
}

ModCfg far * GetPMI(word hPM)
{
	RqBlk ReqBlock = {0};

	ReqBlock.Opcode = opGetPMInfo;

	if (CallProtMan(hPM, &ReqBlock))
	{
		return 0;
	}

	return (ModCfg far *)ReqBlock.Pointer1;
}

KeywordEntry far* FindKey(ModCfg far *pConfig, char *pKey)
{
	KeywordEntry far* pEntry;

	pEntry = pConfig->KE;

	do
	{
		if (!stricmp((char far *)pEntry->KeyWord, pKey))
		{
			return pEntry;
		}

		pEntry = pEntry->NextKeywordEntry;
	}
	while(pEntry);

	return 0;
}

char far* GetConfigString(KeywordEntry far *pEntry)
{
	if (pEntry && pEntry->NumParams >= 1 && pEntry->Params[0].ParamType == 1)
	{
		return (char far *)(&pEntry->Params[1]);
	}
	return "";
}

int ProcessParms(word hDev)
{
	ModCfg far *pModCfg = GetPMI(hDev);
	ModCfg far *pConfig = pModCfg;

	word rc = 0;
	word found = 0;

	while(pConfig)
	{
		if (!stricmp(GetConfigString(FindKey(pConfig, "DRIVERNAME")), cDevName))
		{
			memcpy(AdapterCC.CcName, pConfig->ModName, NAME_LEN);
			fpchar p = GetConfigString(FindKey(pConfig, "MAX_IRQ"));
			if (*p)
			{
				int val = aton(p);
				if (val > 0) 
				{
					max_interrupt_work = val;
				}
				else
				{
					rc = 1;
				}
			}

			p = GetConfigString(FindKey(pConfig, "MODE"));
			if (*p)
			{
				if (!stricmp(p, "1000HALF"))
				{
					fixed_mode = LPA_1000XHALF;
					AdapterSC.MscLinkSpd = 1000000000;
				} else
				if (!stricmp(p, "100HALF"))
				{
					fixed_mode = LPA_100HALF;
					AdapterSC.MscLinkSpd = 100000000;
				} else
				if (!stricmp(p, "10HALF"))
				{
					fixed_mode = LPA_10HALF;
					AdapterSC.MscLinkSpd = 100000000;
				} else
				if (!stricmp(p, "1000FULL"))
				{
					fixed_mode = LPA_1000XFULL;
					AdapterSC.MscLinkSpd = 1000000000;
				} else
				if (!stricmp(p, "10FULL"))
				{
					fixed_mode = LPA_10FULL;
					AdapterSC.MscLinkSpd = 10000000;
				} else
				if (!stricmp(p, "100FULL"))
				{
					fixed_mode = LPA_100FULL;
					AdapterSC.MscLinkSpd = 100000000;
				} 
				else
				{
					rc = 1;
				}
			}

			p = GetConfigString(FindKey(pConfig, "OPTIMIZE"));
			if (*p)
			{
				if (!stricmp(p, "YES"))
				{
					optimization_mode = NV_OPTIMIZATION_MODE_THROUGHPUT;
				} else
				if (!stricmp(p, "NO"))
				{
					optimization_mode = NV_OPTIMIZATION_MODE_CPU;
				} 
				else
				{
					rc = 1;
				}
			}

			p = GetConfigString(FindKey(pConfig, "TIMER"));
			if (*p)
			{
				if (!stricmp(p, "YES"))
				{
					notimer = 0;
				} else
				if (!stricmp(p, "NO"))
				{
					notimer = 1;
				} 
				else
				{
					rc = 1;
				}
			}

			p = GetConfigString(FindKey(pConfig, "LINK_TIMER"));
			if (*p)
			{
				if (!stricmp(p, "YES"))
				{
					nolinktimer = 0;
				} else
				if (!stricmp(p, "NO"))
				{
					nolinktimer = 1;
				} 
				else
				{
					rc = 1;
				}
			}

			p = GetConfigString(FindKey(pConfig, "RXCHAIN"));
			if (*p)
			{
				if (!stricmp(p, "YES"))
				{
					dorxchain = 1;
				} else
				if (!stricmp(p, "NO"))
				{
					dorxchain = 0;
				} 
				else
				{
					rc = 1;
				}
			}
	
			found = 1;
			break;
		}
		pConfig = pConfig->NextModCfg;
	}

	if (rc)
	{
		return -9;
	}

	if (!found)        
	{
		return -4;
	}

	if (dorxchain)
	{
		AdapterSC.MscService |= RECEIVECHAIN_MOSTLY;
	}

	rc = RegisterModule(hDev, &AdapterCC, 0);

	if (rc)
	{
	        return -6;
	}

	return 0;
}

int NDISInit(void)
{
	int hDev = TestDevice(cDevName);

	if (hDev != 110)    
	{

		char *p = &cDevName[strlen(cDevName)];

		if (!cDevName[0])    
		{
			p[0] = '0';
			p[1] = '$';
			p[2] = 0;
		}
		else
		{
			p[0] = '$';
			p[1] = 0;
			p--;
		}

		for (adapternum = 1; adapternum < MAX_DRIVERS; adapternum++)
		{
			*p = char('0' + adapternum);

			hDev = TestDevice(cDevName);

			if (hDev == 110)
			{
				break;
			}

		}

		if (adapternum >= MAX_DRIVERS)
		{
			return -2;
		}
	}

	memcpy(DevName, cDevName, strlen(cDevName));

	hDev = OpenDevice("PROTMAN$");
	if (!hDev)
	{
		return -3;
	}

	int rc = ProcessParms(hDev);

	DosClose(hDev);
	return rc;
}

word ndiscall SysReq(dword dwParam1, dword dwParam2, word wParam,
                     word wOper, word wProtDS)
{
	word wRC = GENERAL_FAILURE;
	word wDS = DevSetDS(wProtDS);

	switch(wOper)
	{
		case Bind:
		{
			time.init();
			word rc = SetupOEMHlp();
			if (rc)
			{
				break;
			}

			rc = dev.open();
			if (rc)
			{
				break;
			}

			if (!(AdapterCC.CcSSp->MssStatus & MS_BOUND))
			{
				CommChar far* pCC       = (CommChar far *)dwParam1;
				CommChar far* far* ppCC = (CommChar far* far*)dwParam2;

				if (!pCC || !pCC->CcLDp || !ppCC)
				{
					break;
				}

				memcpy(&ProtCC, pCC       , sizeof(CommChar));
				memcpy(&ProtDT, pCC->CcLDp, sizeof(ProtLwrDisp));

				*ppCC = &AdapterCC;

				AdapterCC.CcSSp->MssStatus |= MS_BOUND;
				wRC = 0;
			}
			dev.registerRM();
		} break;
	}

	DevSetDS(wDS);
	return wRC;
}

