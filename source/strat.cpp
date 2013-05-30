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
#include "version.h"

word StratInit(RPInit far* pPacket)
{
	int rc = 0;

	DevInit(pPacket->In.DevHlp);

	Say(CRLF DNAME VERSION CRLF COPYRIGHT DVENDOR CRLF);

	FixFarPointers();

	rc = NDISInit();
	if (rc)
	{
		Say(cRCStr[-rc]);
		Say(CRLF);
	}

	if (rc)
	{
	        Say(cDevName);
		Say(" driver not loaded.");
		Say(CRLF);

		pPacket->Out.FinalCS = 0;
		pPacket->Out.FinalDS = 0;
	}
	else
	{
	        Say(cDevName);
        	Say(" loaded.");
		Say(CRLF);
        	pPacket->Out.FinalCS = OffsetFinalCS;
	        pPacket->Out.FinalDS = OffsetFinalDS;
	}
	return RPDONE;
}

word StratOpen (RPOpenClose far* pPacket)
{
	(void)pPacket;

	if (wState != ST_CLOSED)
	{
		return RPDONE | RPERR_USE;
	}

	if (!(AdapterCC.CcSSp->MssStatus & MS_BOUND))
	{
		return RPDONE | RPERR_READY;
	}

	wState = ST_OPENED;

	return RPDONE;
}

word StratClose(RPOpenClose far* pPacket)
{
	(void) pPacket;

	if (wState != ST_OPENED)
	{
		return RPDONE | RPERR_GENERAL;
	}

	wState = ST_CLOSED;

	return RPDONE;
}

#if (DEBUG==2)
word StratRead(RPRWV far* rp)
{
	word rc = memdebug.fillbuf(rp->Transfer + rp->Start, rp->Count);
	return RPDONE | rc;
}
#endif 

typedef word (*PHandler)(RP far* rp);

PHandler StratDispatch[] =
{
  (PHandler)StratInit ,                 
  0,                                    
  0,                                    
  0,                                    
  0 ,			                
  0,                                    
  0,                                    
  0,                                    
  0,			                
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  (PHandler)StratOpen ,                 
  (PHandler)StratClose,                 
  0,                                    
  0,			                
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0,                                    
  0                                     
};

extern "C" void Strategy(RP far* rp);

#pragma aux (STRATEGY) Strategy;

void Strategy(RP far* rp)
{
	if (rp->Command < (sizeof(StratDispatch)/sizeof(StratDispatch[0])) && StratDispatch[rp->Command])
	{
		rp->Status = StratDispatch[rp->Command](rp);
	}
	else
	{
		rp->Status = RPDONE | RPERR_COMMAND;
	}
}

