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

#include <base.h>

#ifndef __DEVRP_H
#define __DEVRP_H

#pragma pack(1)

struct RP
{
    byte Length;		
    byte Unit;                  
    byte Command;               
    word Status;                
    byte Flags;                 
    byte Reserved[3];           
    RP far *Link;               

};

typedef struct RP far *PRPH;

struct RPInit:public RP
{
	union
	{
		struct
		{
			byte Reserved;		
            dword DevHlp;       
            fpchar Args;        
            byte Drive;         
        }
		In;
		struct
		{
			byte Units;			
            word FinalCS;       
            word FinalDS;       
            fptr Bpb;           
        }
		Out;
	};
};

struct RPMediaCheck:public RP
{
	byte Media;					
    byte Result;                
    fpchar PrevVolID;           
};

struct RPBuildBPB:public RP
{
	byte Media;					
    fptr Transfer;              
    fptr Bpb;                   
    byte Drive;                 
};

struct RPRWV:public RP
{
    byte Media;					
    dword Transfer;             
    word Count;                 
    dword Start;                
    word FileID;                

};

struct RPReadNoWait:public RP
{
	byte Data;					

};

struct RPIOStatus:public RP
{
};

struct RPIOFlush:public RP
{
};

struct RPOpenClose:public RP
{
	word FileID;				
};

struct RPRemovable:public RP
{
};

struct RPIOCtl:public RP
{
    byte Category;		
    byte Function;              
    fpbyte ParmPacket;          
    fpbyte DataPacket;          
    word FileID;                
    word ParmLength;            
    word DataLength;            
};

struct RPReset:public RP
{
};

struct RPUnit:public RP
{
};

struct RPDeinstall:public RP
{
};

struct RPPartitionable:public RP
{
	byte Count;					
    dword Reserved;             
};

struct RPUnitMap:public RP
{
	dword Units;				
    dword Reserved;             
};

struct RPShutdown:public RP
{
	byte Function;				
    dword Reserved;             
};

struct RPDriverCaps:public RP
{
	byte Reserved[3];			
    fptr DCS;                   
    fptr VCS;                   
};

struct RPInitComplete:public RP
{
};

#define RPERR                 0x8000	
#define RPDEV                 0x4000	
#define RPBUSY                0x0200	
#define RPDONE                0x0100	

#define RPERR_PROTECT         0x8000	
#define RPERR_UNIT            0x8001	
#define RPERR_READY           0x8002	
#define RPERR_COMMAND         0x8003	
#define RPERR_CRC             0x8004	
#define RPERR_LENGTH          0x8005	
#define RPERR_SEEK            0x8006	
#define RPERR_FORMAT          0x8007	
#define RPERR_SECTOR          0x8008	
#define RPERR_PAPER           0x8009	
#define RPERR_WRITE           0x800A	
#define RPERR_READ            0x800B	
#define RPERR_GENERAL         0x800C	
#define RPERR_DISK            0x800D	
#define RPERR_MEDIA           0x8010	
#define RPERR_INTERRUPTED     0x8011	
#define RPERR_MONITOR         0x8012	
#define RPERR_PARAMETER       0x8013	
#define RPERR_USE             0x8014	
#define RPERR_INIT            0x8015	

#define CMDInit                 0       
#define CMDMedChk               1       
#define CMDBldBPB               2       
#define CMDIOCTLR               3       
#define CMDINPUT                4       
#define CMDNDR                  5       
#define CMDInputS               6       
#define CMDInputF               7       
#define CMDOUTPUT               8       
#define CMDOUTPUTV              9       
#define CMDOutputS              10      
#define CMDOutputF              11      
#define CMDIOCTLW               12      
#define CMDOpen                 13      
#define CMDClose                14      
#define CMDRemMed               15      
#define CMDGenIOCTL             16      
#define CMDResMed               17      
#define CMDGetLogMap            18
#define CMDSetLogMap            19
#define CMDDeInstall            20      
#define CMDPartfixeddisks       22      
#define CMDGetfd_logunitsmap    23      
#define CMDInputBypass          24      
#define CMDOutputBypass         25      
#define CMDOutputBypassV        26      
#define CMDInitBase             27      
#define CMDShutdown             28
#define CMDGetDevSupport        29      
#define CMDInitComplete         31      
#define CMDSaveRestore          32
#define CMDGetDriveAssign       34      
#define CMDAddOnPrep            97      
#define CMDStar                 98      
#define CMDStop                 99      

#define IOCTL_ASYNC                        0x0001
#define IOCTL_SCR_AND_PTRDRAW              0x0003
#define IOCTL_KEYBOARD                     0x0004
#define IOCTL_PRINTER                      0x0005
#define IOCTL_LIGHTPEN                     0x0006
#define IOCTL_POINTINGDEVICE               0x0007
#define IOCTL_DISK                         0x0008
#define IOCTL_PHYSICALDISK                 0x0009
#define IOCTL_MONITOR                      0x000A
#define IOCTL_GENERAL                      0x000B
#define IOCTL_POWER                        0x000C
#define IOCTL_OEMHLP                       0x0080
#define IOCTL_TESTCFG_SYS                  0x0080
#define IOCTL_CDROMDISK                    0x0080
#define IOCTL_CDROMAUDIO                   0x0081
#define IOCTL_TOUCH_DEVDEP                 0x0081
#define IOCTL_TOUCH_DEVINDEP               0x0081

#endif 

