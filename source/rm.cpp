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
extern "C"
{
#include <os2def16.h>
#include <rmbase.h>
#include <rmcalls.h>

ULONG           RMFlags = 0;
PFNRM           RM_Help0 = 0;
PFNRM           RM_Help3 = 0;
}

#include <base.h>

#define	MAX_RESHANDLES	8

class RM
{
private:
	HDRIVER		hDriver;
	HADAPTER	hAdapter;
	USHORT		nAdapters;

	struct
	{
		ULONG ctHandles;
		HRESOURCE arHandles[MAX_RESHANDLES];
	} ahRes;

	int addResource(PRESOURCESTRUCT pResStruct)
	{
		int	rc = -1;

		if (ahRes.ctHandles < MAX_RESHANDLES)
		{
			rc = RMAllocResource(hDriver, &ahRes.arHandles[ahRes.ctHandles], pResStruct);
			if (!rc)
			{
				ahRes.ctHandles++;
			}
		}

		return rc;
	}

public:
	RM() : hDriver(0), hAdapter(0), nAdapters(0) 
	{
			ahRes.ctHandles = 0; 
	}

	~RM()
	{
		destroy();
	}

	int init()
	{
		static DRIVERSTRUCT drs =
		{
			(PSZ)DFILE,
			(PSZ)DNAME,
			(PSZ)DVENDOR,
			CMVERSION_MAJOR,
			CMVERSION_MINOR,
			{ 
				(DDATE / 10000),
				(DDATE % 10000) / 100,
				(DDATE % 100)
			},
			DRF_STATIC,
			DRT_NETWORK,
			0,
			NULL
		};

		hAdapter = 0;
		return (!hDriver && RMCreateDriver(&drs, &hDriver)) ? 1 : 0;
	}

	void destroy()
	{
		while (ahRes.ctHandles)
		{
			RMDeallocResource(hDriver, ahRes.arHandles[--ahRes.ctHandles]);
		}

		if (hAdapter)
		{
			RMDestroyAdapter(hDriver, hAdapter);	
			hAdapter = 0;
			if (nAdapters)
			{
				nAdapters--;
			}
		}

		if (hDriver && nAdapters == 0)
		{
			RMDestroyDriver(hDriver);
			hDriver = 0;
		}
	}

	int addmem(dword addr, dword length)
	{
		RESOURCESTRUCT	ResStruct = {0};

		ResStruct.ResourceType = RS_TYPE_MEM;
		ResStruct.MEMResource.MemBase = addr;
		ResStruct.MEMResource.MemSize = length;
		ResStruct.MEMResource.MemFlags = RS_MEM_EXCLUSIVE;

		return addResource(&ResStruct);
	}

	int addirq(word irq, word pin)
	{
		RESOURCESTRUCT	ResStruct = {0};

		ResStruct.ResourceType = RS_TYPE_IRQ;
		ResStruct.IRQResource.IRQLevel = irq;
		ResStruct.IRQResource.PCIIrqPin = pin;
		ResStruct.IRQResource.IRQFlags = RS_IRQ_SHARED;

		return addResource(&ResStruct);
	}

	int addio(word addr, word length)
	{
		RESOURCESTRUCT	ResStruct = {0};

		ResStruct.ResourceType = RS_TYPE_IO;
		ResStruct.IOResource.BaseIOPort = addr;
		ResStruct.IOResource.NumIOPorts = length;
		ResStruct.IOResource.IOFlags = RS_IO_EXCLUSIVE;
		ResStruct.IOResource.IOAddressLines = 16;

		return addResource(&ResStruct);
	}

	int createadapter(const char far * const adapter, word num)
	{
		int		rc;
		char		adapterkey[128];

		static ADJUNCT adj =
		{
			NULL, sizeof(ADJUNCT), ADJ_ADAPTER_NUMBER
		};

		static ADAPTERSTRUCT  ads =
		{
			NULL,
			AS_NO16MB_ADDRESS_LIMIT,
			AS_BASE_NETWORK,
			AS_SUB_ETHERNET,
			0,
			AS_HOSTBUS_PCI,
			AS_BUSWIDTH_32BIT,
			&adj,
			0
		};
		
		strcpy(adapterkey, "NVETH_# ");
		strcat(adapterkey, adapter);
		strcat(adapterkey, " Network Adapter");
		ads.AdaptDescriptName = (PSZ)adapterkey;
		adj.Adapter_Number = num;

		rc = RMCreateAdapter(hDriver, &hAdapter, &ads, 0, (PAHRESOURCE)&ahRes);
		if (rc == 0)
		{
			ahRes.ctHandles = 0;
			nAdapters++;
		}
		return rc;
	}
};

RM rm;

int RMinit()
{
	return rm.init();
}

void RMdestroy()
{
	rm.destroy();
}

int RMaddmem(dword addr, dword length)
{
	return rm.addmem(addr, length);
}

int RMaddio(word addr, word length)
{
	return rm.addio(addr, length);
}

int RMaddirq(word irq, word pin)
{
	return rm.addirq(irq, pin);
}

int RMcreateadapter(const char far * const adapter, word num)
{
	return rm.createadapter(adapter, num);
}

