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

struct IDC_Entry OemHlpIDC = {0};

struct RPIOCtl rp;
int PCInumBuses = 0;
PCI_PARM_WRITE_CONFIG rpparam = {0};
PCI_DATA  rpdata = {0};

extern word adapternum;

PCI_DEVICE::PCI_DEVICE()
{
	*name = 0;
}

void PCI_DEVICE::setName(fpchar drivername, fpchar devname)
{
	strcpy(name, drivername);
	strcat(name, devname);
}

int PCI_DEVICE::registerRM()
{
	if (RMinit())
	{
		return 1;
	}
	int rc = 0;
	do 
	{
		for (int n = 0; !rc && n < 6; n++)
		{
			if (bars[n].bar)
			{
				if (bars[n].io)
				{
					rc = RMaddio((word)bars[n].start, (word)bars[n].size);
				}
				else
				{
					rc = RMaddmem(bars[n].start, bars[n].size);
				}
			}
		}

		rc = RMaddirq(irq, ipin);
		if (rc)
		{
			break;
		}
		rc = RMcreateadapter((fpchar)name, adapternum);
	} while (0);
	if (rc)
	{
		RMdestroy();
	}
	return rc;
}

long PCI_DEVICE::readConfig(const byte reg, const byte size) const
{
	PPCI_PARM_READ_CONFIG param = (PPCI_PARM_READ_CONFIG)rp.ParmPacket;
	param->BusNum = bus;
	param->DevFunc = func;
	param->ConfigReg = reg;
	param->Size = size;
	param->SubFunc = OEMHLP_READ_PCI_CONFIG;
	rp.ParmLength = sizeof(*param);
	rp.Status     = 0;
	CallOEMHlp((PRPH)&rp);
	if (rp.Status & 0x8000)
	{
		return -1;
	}
	PPCI_DATA data = (PPCI_DATA)rp.DataPacket;
	switch (size)
	{
		case 1: return data->Config.ucData;
		case 2: return data->Config.usData;
		case 4: return data->Config.ulData;
		default:
		{
			return -1;
		}
	}
}

int PCI_DEVICE::writeConfig(const byte reg, const byte size, const dword data) const
{
	PPCI_PARM_WRITE_CONFIG param = (PPCI_PARM_WRITE_CONFIG)rp.ParmPacket;
	param->BusNum = bus;
	param->DevFunc = func;
	param->ConfigReg = reg;
	param->Size = size;
	param->Data = data;
	param->SubFunc = OEMHLP_WRITE_PCI_CONFIG;
	rp.ParmLength = sizeof(*param);
	rp.Status     = 0;
	CallOEMHlp((PRPH)&rp);
	if (rp.Status & 0x8000)
	{
		return -1;
	}
	return 0;
}

int PCI_DEVICE::findCaps()
{
	long rc = readConfig(PCIREG_STATUS, sizeof(short));
	if (rc == -1)
	{
		return -1;
	}
	if (!(rc & PCI_STATUS_CAP_LIST))
	{
		return 0;
	}
	byte pos = 0;
	switch (headertype) {
		case PCI_HEADER_TYPE_NORMAL:
		case PCI_HEADER_TYPE_BRIDGE:
		{
			pos = PCIREG_CAP;
		} break;	
		case PCI_HEADER_TYPE_CARDBUS:
		{
			pos = PCIREG_CAP_CARDBUS;
		} break;	
		default :
		{
			return -1;
		}
	}

	for (capcnt = 0; capcnt < MAXCAPCNT && pos > 0; capcnt++)
	{
		rc = readConfig(pos, sizeof(short));
		if (rc == -1)
		{
			break;
		}
		caps[capcnt].type = (byte)(rc & 0xFF);
		caps[capcnt].pos = pos;
		pos = (byte)((rc >> 8) & 0xFF);
	}
	return rc == -1 ? -1 : 0;
}

PPCI_CAP PCI_DEVICE::operator[](const byte type) const
{
	for (int i = 0; i < capcnt; i++)
	{
		if (caps[i].type == type)
		{
			return (PPCI_CAP)(caps + i);
		}
	}
	return NULL;
}

int PCI_DEVICE::setPowerStateD0() const
{
	PPCI_CAP pm = (*this)[PCI_CAP_ID_PM];
	
	if (!pm)
	{
		return -1; 
	}

	long rc = readConfig((byte)(pm->pos + PCI_PM_PMC), sizeof(word));
	if (rc == -1)
	{
		return -1;
	}
	word pmc = (word)rc;
	if ((pmc & PCI_PM_CAP_VER_MASK) > 3) 
	{		
		return -1;
	}

	rc = readConfig((byte)(pm->pos + PCI_PM_CTRL), sizeof(word));
	word pmcsr = (word)rc;
	if (rc == -1)
	{
		return -1;
	}
	word powerstate = pmcsr & PCI_PM_CTRL_STATE_MASK;
	if (powerstate == PCI_D0)
	{
		return 0;
	}

	pmcsr &= ~PCI_PM_CTRL_STATE_MASK;
	rc = writeConfig((byte)(pm->pos + PCI_PM_CTRL), sizeof(pmcsr), pmcsr);
	if (rc)
	{
		return -1;
	}

	return 0;
}

int PCI_DEVICE::setBusMaster()
{	
	int res = 0;

	long rc = readConfig(PCIREG_COMMAND, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	command = (word)(rc & 0xFFFF);
	command |= PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY;

	rc = writeConfig(PCIREG_COMMAND, sizeof(command), command);
	if (rc == -1)
	{
		res = -1; 
	}

	rc = readConfig(PCIREG_LATENCY, sizeof(byte));
	if (rc == -1)
	{
		return -1;
	}
	byte lat = (byte)rc;
	if (lat < 16)
	{
		lat = 64;
		long rc = writeConfig(PCIREG_LATENCY, sizeof(lat), lat);
		if (rc == -1)
		{
			res = -1; 
		}
	}
	return res;
}

int PCI_DEVICE::init(const byte newbus, const byte newfunc)
{
	bus = newbus;
	func = newfunc;
	vendor = 0;
	device = 0;
	status = 0;
	command = 0;
	headertype = 0;
	subvendor = 0;
	subsystemid = 0;
	irq = 0;
	ipin = 0;
	capcnt = 0;

	long rc = readConfig(PCIREG_VENDOR_ID, sizeof(word));
	if (rc == -1L)
	{
		return -1;	
	}
	vendor = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_DEVICE_ID, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	device = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_SUBVENDOR, sizeof(word));
	if (rc == -1L)
	{
		return -1;	
	}
	subvendor = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_SUBSYSTEM, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	word subsystemid = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_INT_LINE, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	irq = (byte)(rc & 0xFF);
	ipin = (byte)((rc >> 8) & 0xFF);

	rc = readConfig(PCIREG_COMMAND, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	command = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_STATUS, sizeof(word));
	if (rc == -1L)
	{
		return -1;		
	}
	status = (word)(rc & 0xFFFF);

	rc = readConfig(PCIREG_HEADER_TYPE, sizeof(byte));
	if (rc == -1L)
	{
		return -1;		
	}
	headertype = (byte)(rc & 0x7F);

	for (word i = 0; i < 6; i++)
	{
		bars[i].bar = 0L;
		byte reg = (byte)(PCIREG_BAR0 + (i << 2));
		rc = readConfig(reg, sizeof(long));
		if (rc == -1L)
		{
			continue;		
		}
		dword addr = (dword)rc;
		bars[i].io = (word)(addr & 1);
		bars[i].type = (word)((addr >> 1) & 3);
		bars[i].prefetchable = (word)((addr >> 3) & 1);
		dword mask = bars[i].io ? ~3 : ~15;
		if (!(addr & mask))
		{
			continue;
		}
		bars[i].start = addr & mask;
		bars[i].bar = addr;
		rc = writeConfig(reg, sizeof(addr), -1L);
		if (rc == -1)
		{
			continue;
		}
		rc = readConfig(reg, sizeof(long));
		mask &= rc;
		rc = writeConfig(reg, sizeof(addr), addr);
		if (rc == -1)
		{
		}
		bars[i].size = -mask;
	}
	rc = findCaps();
	if (rc)
	{
	}
	return 0;
}

word SetupOEMHlp(void)
{
	PPCI_PARM_FIND_CLASSCODE  param = (PPCI_PARM_FIND_CLASSCODE)&rpparam;
	PPCI_DATA  data = (PPCI_DATA)&rpdata;

	PRPH		    p = (PRPH)&rp;

	if (DevAttachDD(OEMHLP_NAME, &OemHlpIDC))
	{
		return 1;
	}

	if ((SELECTOROF(OemHlpIDC.pFunc) == NULL) ||
		(OemHlpIDC.wDS == NULL))
	{
		return 2;     
	}
	OemHlpProc = (dword)OemHlpIDC.pFunc;
	OemHlpDS = OemHlpIDC.wDS;

	rp.Length     = sizeof(rp);
	rp.Command    = CMDGenIOCTL;
	rp.Status     = 0;
	rp.Category   = IOCTL_OEMHLP;
	rp.Function   = OEMHLP_PCI;
	rp.ParmPacket = (fpbyte)param;
	rp.DataPacket = (fpbyte)data;
	rp.ParmLength = sizeof(*param);
	rp.DataLength = sizeof(*data);
	
	param->SubFunc = OEMHLP_QUERY_PCI_BIOS;

	CallOEMHlp(p);
	if (rp.Status & 0x8000)
	{
		return 3;
	}

	if (data->rc) 
	{
		return 4;
	}
	PCInumBuses = data->BiosInfo.LastBus + 1;
	if (data->BiosInfo.MajorVer < BUS_PCI)
	{
		return 5;
	}

	int found = 0;
	dword pciclass = 0x00068000;
	for (char index = 0; index < MAX_PCI_DEVICES; index++)
	{  
		param->SubFunc = OEMHLP_FIND_PCI_CLASS;
		param->Class = pciclass;
		param->Index = (UCHAR)index;
		rp.Status    = 0;
		rp.ParmLength = sizeof(*param);
		CallOEMHlp(p);
		if (rp.Status & 0x8000)
		{
			if (pciclass == 0x00020000)
			{
				break;
			}
			else
			{
				pciclass = 0x00020000;
				index = -1;
				continue;
			}
		}
		int rc = dev.init(data->FindDev.BusNum, data->FindDev.DevFunc);
		if (rc)
		{
			continue;
		}
		if (!checkDevice(dev))
		{
			continue;
		}
		found++;
		if (found <= adapternum)
		{
			continue;
		}
		dev.setPowerStateD0();
		dev.setBusMaster();
		rc = dev.setup();
		if (rc)
		{
			continue;
		}
		return 0;
	}

	return 6; 
}

