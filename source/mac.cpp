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

word AddMCast(fpchar pAddr)
{
	int i;

	if (AdapterMCB.McbCnt == AdapterMCB.McbMax)
	{
		return 1;
	}

	for(i = 0; i < AdapterMCB.McbCnt; i++)
	{
		if (!memcmp(AdapterMCB.McbAddrs[i].mAddr, pAddr, 6))
		{
			return 1;
		}
	}

	memcpy(AdapterMCB.McbAddrs[AdapterMCB.McbCnt].mAddr, pAddr, 6);

	AdapterMCB.McbCnt++;
	return 0;

}

word DelMCast(fpchar pAddr)
{
	word i;

	for (i = 0; i < AdapterMCB.McbCnt; i++)
	{
		if (!memcmp(AdapterMCB.McbAddrs[i].mAddr, pAddr, 6))
		{
			int j;
			for (j = i + 1; j < AdapterMCB.McbCnt; j++)
			{
				AdapterMCB.McbAddrs[j - 1] = AdapterMCB.McbAddrs[j];
			}
			AdapterMCB.McbCnt--;
			return 0;
		}
	}
	return 1;
}

word ndiscall AdapterGenReq(word, word, word wParm, dword dwBuf,
                        	word wOp, word wAdapterDS)
{
	word wRC = 0;
	word wDS = DevSetDS(wAdapterDS);

	switch(wOp)
	{
		case OpenAdapter:
		{
			if (AdapterCC.CcSSp->MssStatus & MS_OPEN)
			{
				wRC = INVALID_FUNCTION;
			}
			else
			{
				AdapterCC.CcSSp->MssStatus |= MS_OPEN | HW_OK;
			}
		} break;

		case CloseAdapter:
		{
			if (AdapterCC.CcSSp->MssStatus & MS_OPEN)
			{
				AdapterCC.CcSSp->MssStatus &= ~MS_OPEN;
			}
			else
			{
				wRC = INVALID_FUNCTION;
			}
		} break;

		case SetLookAhead:
		{
			if (wParm > AdapterCC.CcSCp->MscMaxFrame)
			{
				wRC = INVALID_PARAMETER;
				break;
			}
		} break;

		case SetPacketFilter:
		{
			if (wParm & 0xFFF8)
			{
				wRC = INVALID_PARAMETER;
				break;
			}
			wPktMask = wParm;
			dev.setMcast();
		} break;

		case UpdateStatistics:
		{
			dev.getHWStats();
		} break;

		case ClearStatistics:
		break;

		case SetStationAddress:
		{
			if (!(AdapterCC.CcSSp->MssStatus & MS_OPEN))
			{
				memcpy(AdapterCC.CcSCp->MscCurrStnAdr, fpbyte(dwBuf), 6);
				dev.setMac();
			}
			else
			{
				if (!memcmp(AdapterCC.CcSCp->MscCurrStnAdr, fpbyte(dwBuf), 6))
				{
					wRC = GENERAL_FAILURE;
				}
			}
		} break;

		case AddMulticastAddress:
		{
			if (AddMCast(fpchar(dwBuf)))
			{
				wRC = INVALID_PARAMETER;
			}
			else
			{
				dev.setMcast();
			}
		} break;

		case DeleteMulticastAddress:
		{
			if (DelMCast(fpchar(dwBuf)))
			{
				wRC = INVALID_PARAMETER;
			}
			else
			{
				dev.setMcast();
			}
		} break;

		default:
		{
			wRC = NOT_SUPPORTED;
		} break;

	}

	DevSetDS(wDS);
	return wRC;
}

word ndiscall AdapterXmitChain(word, word, fpbyte pBuff, word wAdapterDS)
{
	word wRC = OUT_OF_RESOURCE;
	word wDS = DevSetDS(wAdapterDS);

	if (!pBuff)
	{
		wRC = INVALID_PARAMETER;
	}
	else
	{
		wRC = dev.startXmit((TxBufDesc far *)pBuff);
	}

	DevSetDS(wDS);
	return wRC;
}

word ndiscall AdapterXferData(fpbyte copied, word frameOffset, fpbyte pBuff, word wAdapterDS)
{
	word wRC = INVALID_PARAMETER;
	word wDS = DevSetDS(wAdapterDS);

	if (pBuff)
	{
		wRC = dev.xferRX((fpword)copied, frameOffset, (TDBufDesc far *)pBuff);
	}

	DevSetDS(wDS);
	return wRC;
}

word ndiscall AdapterRcvRelease(word pos, word wAdapterDS)
{
	word wDS = DevSetDS(wAdapterDS);
	word wRC = dev.releaseRX(pos);
	DevSetDS(wDS);
	return wRC;
}

word ndiscall AdapterIndOn(word wAdapterDS)
{
	word wRC = 0;
	word wDS = DevSetDS(wAdapterDS);

	indOn();

	DevSetDS(wDS);
	return wRC;
}

word ndiscall AdapterIndOff(word wAdapterDS)
{
	word wRC = 0;
	word wDS = DevSetDS(wAdapterDS);

	indOff();

	DevSetDS(wDS);
	return wRC;
}

void indOn(void)
{
	wIndications--;

	if (!wIndications)
	{
		DevRun(HANDLE(wIndications));
	}
}

void indOff(void)
{
	wIndications++;
}

