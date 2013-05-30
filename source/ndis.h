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

#ifndef __NDIS_H
#define __NDIS_H

#pragma pack(1)     

#define ETH_ALEN	6		
#define ETH_HLEN	14		
#define ETH_ZLEN	60		
#define ETH_DATA_LEN	1500		
#define ETH_FRAME_LEN	1514		

struct ethhdr 
{
	byte	h_dest[ETH_ALEN];	
	byte	h_source[ETH_ALEN];	
	word	h_proto;		
};

#define NAME_LEN        	16
#define ADDR_LEN        	16
#define ADDR_SIZE            6
#define ETH_HEADER_LEN      14
#define ETH_PROTOCOL        12

#define NUM_MCADDRS  4

#define InitiateBind				1
#define Bind					2

#define opGetPMInfo			1
#define opRegisterModule		2
#define opBindAndStart 			3
#define opGetPMLinkage          	4

#define InitiateDiagnostics			1
#define ReadErrorLog				2
#define SetStationAddress			3
#define OpenAdapter				4
#define CloseAdapter				5
#define ResetMAC				6
#define SetPacketFilter		 		7
#define AddMulticastAddress			8
#define DeleteMulticastAddress 			9
#define UpdateStatistics			10
#define ClearStatistics		 		11
#define Interrupt				12
#define SetFunctionalAddress			13
#define SetLookAhead				14

#define RingStatus				1
#define AdapterCheck				2
#define StartReset				3
#define InterruptStatus			 	4
#define EndReset				5

#define SUCCESS			 	0x0000
#define WAIT_FOR_RELEASE		0x0001
#define REQUEST_QUEUED			0x0002
#define FRAME_NOT_RECOGNIZED		0x0003
#define FRAME_REJECTED			0x0004
#define FORWARD_FRAME			0x0005
#define OUT_OF_RESOURCE		 	0x0006
#define INVALID_PARAMETER		0x0007
#define INVALID_FUNCTION		0x0008
#define NOT_SUPPORTED			0x0009
#define HARDWARE_ERROR			0x000A
#define TRANSMIT_ERROR			0x000B
#define NO_SUCH_DESTINATION		0x000C
#define ALREADY_STARTED		 	0x0020
#define INCOMPLETE_BINDING		0x0021
#define DRIVER_NOT_INITIALIZED 		0x0022
#define HARDWARE_NOT_FOUND		0x0023
#define HARDWARE_FAILURE		0x0024
#define CONFIGURATION_FAILURE		0x0025
#define INTERRUPT_CONFLICT		0x0026
#define INCOMPATIBLE_MAC		0x0027
#define INITIALIZATION_FAILED		0x0028
#define GENERAL_FAILURE		 	0x00FF

#define AdapCheckInoperative		0x8000
#define AdapCheckIllegalOp		0x1000
#define AdapCheckLocalParityErr		0x0800
#define AdapCheckParityErr		0x0400
#define AdapCheckInternalParityErr	0x0100
#define AdapCheckRingXmitParityErr	0x0080
#define AdapCheckRingRcvParityErr	0x0040
#define AdapCheckXmitOverrun		0x0020
#define AdapCheckRcvOverrun		0x0010
#define AdapCheckUnrecogInterrupt	0x0008
#define AdapCheckUnrecogErrInt		0x0004
#define AdapCheckNoPCSysService		0x0003
#define AdapCheckUnrecogSuperReq	0x0002
#define AdapCheckProgramRequest		0x0001

#define MACLvl						1
#define MACTyp						1

#define BROADCAST_SUPP          (dword)0x00000001
#define MULTICAST_SUPP          (dword)0x00000002
#define FUNC_GROUP_ADDR_SUPP    (dword)0x00000004
#define PROMISCUOUS_SUPP        (dword)0x00000008
#define SET_STN_ADDR_SUPP       (dword)0x00000010
#define STAT_ALWAYS_CURR        (dword)0x00000020
#define DIAGNOSTICS_SUPP        (dword)0x00000040
#define LOOPBACK_SUPP           (dword)0x00000080
#define RECEIVECHAIN_MOSTLY     (dword)0x00000100
#define IBM_SOURCE_R_SUPP       (dword)0x00000200
#define RESET_MAC_SUPP          (dword)0x00000400
#define OPEN_ADAPTER_SUPP       (dword)0x00000800
#define INTERRUPT_RQ_SUPP       (dword)0x00001000
#define SRC_RTNG_BRDG_SUPP      (dword)0x00002000
#define GDT_ADDRESS_SUPP        (dword)0x00004000
#define MULTIPLE_XFER_DATA_SUPP (dword)0x00008000
#define RCVLK_FRAME_SIZE_ZERO   (dword)0x00010000

#define FLTR_DIRECTED   0x01
#define FLTR_BRDCST     0x02
#define FLTR_PRMSCS     0x04
#define FLTR_SRC_RTG    0x08

#define HW_MISSING  0x0000
#define HW_BUD_ERR  0x0001
#define HW_CFG_ERR  0x0002
#define HW_FAULT    0x0003
#define HW_SOFT_ERR 0x0004
#define HW_OK       0x0007
#define MS_HW_MASK  HW_OK

#define MS_BOUND    0x0008
#define MS_OPEN     0x0010
#define MS_DIAGS_ON 0x0020

#define M83Sb_RFCRC     1       
#define M83Sb_RFFE      2       
#define M83Sb_RFMaxSz   4       

#define M83Sb_TFColMx   1       
#define M83Sb_TFCrr     2       
#define M83Sb_TFShrt    4       
#define M83Sb_TFOpen    8       
#define M83Sb_TFLong    16      
#define M83Sb_TFRemFl   32      

#define M85Sb_RFCon     1       
#define M85Sb_RFCopy    2       

#define M85Sb_TFunrun   1       
#define M85Sb_TFLine    2       
#define M85Sb_TFAbDlm   4       
#define M85Sb_TFLost    8       
#define M85Sb_TFToken   16      

#define MAX_IMMED_LEN   64              	
#define MAX_DATABLK     8
#define MAX_TX_DATABLK  MAX_DATABLK         
#define MAX_TD_DATABLK  MAX_DATABLK     	
#define MAX_RX_DATABLK  MAX_DATABLK         

#define MIN_LOOKAHEAD_DEFAULT   64
#define MAX_LOOKAHEAD           256

#define ndiscall    far pascal              

struct MCastAddr
{
    byte mAddr[ADDR_LEN];
};

struct MCastBuf
{
    word McbMax;                
    word McbCnt;                

    MCastAddr McbAddrs[NUM_MCADDRS];
};

struct MACSpecChar
{
    word   MscSize;                 
    char   MscType[NAME_LEN];       
    word   MscStnAdrSz;             
    byte   MscPermStnAdr[ADDR_LEN]; 
    byte   MscCurrStnAdr[ADDR_LEN]; 
    dword  MscCurrFncAdr;           
    MCastBuf far* MscMCp;           
    dword  MscLinkSpd;              
    dword  MscService;              
    word   MscMaxFrame;             
    dword  MscTBufCap;              
    word   MscTBlkSz;               
    dword  MscRBufCap;              
    word   MscRBlkSz;               
    byte   MscVenCode[3];           
    byte   MscVenAdapter;           
    fpchar MscVenAdaptDesc;         
    word   MscInterrupt;            
    word   MscTxQDepth;             
    word   MscMaxDataBlocks;        
};

struct ProtSpecChar
{
    word PscSize;                   
    char PscType[NAME_LEN];         
    word PscTypeCode;               
};

struct MACSpecStat
{
    word  MssSize;          	
    dword MssDiagDT;            
    dword MssStatus;            
    word  MssFilter;            
    fptr  MssM8Sp;              
    dword MssClearDT;           
    dword MssFR;                
    dword MssRFCRC;             
    dword MssFRByt;             
    dword MssRFLack;            
    dword MssFRMC;              
    dword MssFRBC;              
    dword MssRFErr;             
    dword MssRFMax;             
    dword MssRFMin;             
    dword MssFRMCByt;           
    dword MssFRBCByt;           
    dword MssRFHW;              
    dword MssFS;                
    dword MssFSByt;             
    dword MssFSMC;              
    dword MssFSBC;              
    dword MssFSBCByt;           
    dword MssFSMCByt;           
    dword MssSFTime;            
    dword MssSFHW;              
};

struct MAC8023Stat
{
    word  M83sSize;         	
    word  M83sVer;              
    dword M83sRFAln;            
    dword M83sRMask;            
    dword M83sRFOvrn;           
    dword M83sFSCols;           
    dword M83sFSDfr;            
    dword M83sSFColMx;          
    dword M83sTotCol;           
    dword M83sTotLCol;          
    dword M83sFSCol1;           
    dword M83sFSColM;           
    dword M83sFSHrtB;           
    dword M83sJabber;           
    dword M83sLostCS;           
    dword M83sTMask;            
};

struct MAC8025Stat
{
    word  M85sSize;             
    word  M85sVer;              
    dword M85sFCS;              
    dword M85sRMask;            
    dword M85s5hbtt;            
    dword M85sACerr;            
    dword M85sFsAbrtD;          
    dword M85sFsNrcv;           
    dword M85sFrNoBuf;          
    dword M85sFcopyErr;         
    dword M85sFreqErr;          
    dword M85sMonRegen;         
    dword M85sRsrvd1;           
    dword M85sRsrvd2;           
    dword M85sRsrvd3;           
    dword M85sTMask;            
};

struct CommChar;

struct MACUprDisp
{
    CommChar far *MudCCp;           

    word (ndiscall *MudGReq)      (word, word, word, dword, word, word);
    word (ndiscall *MudXmitChain) (word, word, fpbyte, word);
    word (ndiscall *MudXferData)  (fpbyte, word, fpbyte, word);
    word (ndiscall *MudRcvRelease)(word, word);
    word (ndiscall *MudIndOn)     (word);
    word (ndiscall *MudIndOff)    (word);
};

struct ProtLwrDisp
{
    CommChar far *PldCCp;       
    dword PldIFF;               
    word (ndiscall *PldReqConfirm) (word,word,word,word,word,word);
    word (ndiscall *PldXmitConfirm)(word,word,word,word,word);
    word (ndiscall *PldRcvLkAhead) (word,word,word,fpbyte,fpbyte,word);
    word (ndiscall *PldIndComplete)(word,word);
    word (ndiscall *PldRcvChain)   (word,word,word,fpbyte,fpbyte,word);
    word (ndiscall *PldStatInd)    (word,word,fpbyte,word,word);
};

struct CommChar
{
    word  CcSize;               
    word  CcLvl;                
    word  CcSpLvl;              
    byte  CcMjrVer;             
    byte  CcMnrVer;             
    dword CcBindFnc;            
    char  CcName[NAME_LEN];     
    byte  CcUPLevel;            
    byte  CcUIType;             
    byte  CcLPLevel;            
    byte  CcLIType;             
    word  CcModuleID;           
    word  CcDataSeg;            

    word (ndiscall *CcSysReq)    (dword, dword, word, word, word);

    MACSpecChar far* CcSCp;     
    MACSpecStat far* CcSSp;     
    MACUprDisp  far* CcUDp;     
    ProtLwrDisp far* CcLDp;     

    dword CcRsv1;               
    dword CcRsv2;               
};

struct TxDataBlock
{
    byte   TxPtrType;           
    byte   TxRsvdByte;          
    word   TxDataLen;           
    fpbyte TxDataPtr;           
};

struct TxBufDesc
{
    word   TxImmedLen;          
    fpbyte TxImmedPtr;          
    word   TxDataCount;         

    TxDataBlock TxDataBlk[MAX_TX_DATABLK];
};

struct TDDataBlock
{
    byte   TDPtrType;           
    byte   TDRsvdByte;          
    word   TDDataLen;           
    fpbyte TDDataPtr;           
};

struct TDBufDesc
{
    word TDDataCount;           

    TDDataBlock TDDataBlk[MAX_TD_DATABLK];
};

struct RxDataBlock
{
    word   RxDataLen;           
    fpbyte RxDataPtr;           
};

struct RxBufDesc
{
    word RxDataCount;           

    RxDataBlock RxDataBlk[MAX_RX_DATABLK];
};

typedef struct RxBufOne
{
    word RxDataCount;           
    RxDataBlock RxDataBlk;
} RxBufOne;

struct Param
{
    word ParamType;             
    word ParamLen;              

};

struct KeywordEntry
{
    KeywordEntry far *NextKeywordEntry; 
    KeywordEntry far *PrevKeywordEntry; 
    char  KeyWord[NAME_LEN];            
    word  NumParams;                    
    Param Params[1];                    
};

struct ModCfg
{
    ModCfg far *NextModCfg;             
    ModCfg far *PrevModCfg;             

    char ModName[NAME_LEN];             

    KeywordEntry KE[1];                 

};

struct ProIni
{
    ModCfg MC[1];                       
};

struct RqBlk
{
    word Opcode;
    word Status;
    fptr Pointer1;
    fptr Pointer2;
    word Word1;
};

#define NUMBINDINGS  1

struct BindingsList
{
    word NumBindings;
    struct Module
    {
        char ModuleName[NAME_LEN];

    } BoundDriver[NUMBINDINGS];
};

#pragma pack()

#endif  

