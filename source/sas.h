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
#ifndef _SAS_H_
#define _SAS_H_

#pragma pack(1)

#define SAS_SIG         "SAS "
#define SAS_CBSIG       4

struct SAS {
    unsigned char  SAS_signature[SAS_CBSIG]; 
    unsigned short SAS_tables_data;  
    unsigned short SAS_flat_sel;     
    unsigned short SAS_config_data;  
    unsigned short SAS_dd_data;      
    unsigned short SAS_vm_data;      
    unsigned short SAS_task_data;    
    unsigned short SAS_RAS_data;     
    unsigned short SAS_file_data;    
    unsigned short SAS_info_data;    
};

struct SAS_tables_section {
    unsigned short SAS_tbl_GDT;         
    unsigned short SAS_tbl_LDT;         
    unsigned short SAS_tbl_IDT;         
    unsigned short SAS_tbl_GDTPOOL;     
};

struct SAS_config_section {
    unsigned short SAS_config_table;                                            
};

struct SAS_dd_section {
    unsigned short SAS_dd_bimodal_chain; 
                                         
    unsigned short SAS_dd_real_chain;    
                                         
    unsigned short SAS_dd_DPB_segment;   
                                         
    unsigned short SAS_dd_CDA_anchor_p;  
                                         
    unsigned short SAS_dd_CDA_anchor_r;  
                                         
    unsigned short SAS_dd_FSC;           
};

struct SAS_vm_section {
    unsigned long SAS_vm_arena;     
    unsigned long SAS_vm_object;    
    unsigned long SAS_vm_context;   
    unsigned long SAS_vm_krnl_mte;  
    unsigned long SAS_vm_glbl_mte;  
                                    
    unsigned long SAS_vm_pft;       
    unsigned long SAS_vm_prt;       
    unsigned long SAS_vm_swap;      
                                    
    unsigned long SAS_vm_idle_head; 
    unsigned long SAS_vm_free_head; 
    unsigned long SAS_vm_heap_info; 
    unsigned long SAS_vm_all_mte;   
};

struct SAS_task_section {
    unsigned short SAS_task_PTDA;       
    unsigned long SAS_task_ptdaptrs;    
    unsigned long SAS_task_threadptrs;  
    unsigned long SAS_task_tasknumber;  
    unsigned long SAS_task_threadcount; 
};

struct SAS_RAS_section {
    unsigned short SAS_RAS_STDA_p;      
                                        
    unsigned short SAS_RAS_STDA_r;      
                                        
    unsigned short SAS_RAS_event_mask;  
};

struct SAS_file_section {
    unsigned long SAS_file_MFT; 
    unsigned short SAS_file_SFT;        
                                        
    unsigned short SAS_file_VPB;        
                                        
    unsigned short SAS_file_CDS;        
                                        
    unsigned short SAS_file_buffers;    
};

struct SAS_info_section {
    unsigned short SAS_info_global;     
    unsigned long SAS_info_local;       
    unsigned long SAS_info_localRM;     
    unsigned short SAS_info_CDIB;       
                                        
};

#define SAS_SEL 0x70

struct InfoSegGDT {

unsigned long   SIS_BigTime;    
unsigned long   SIS_MsCount;    
unsigned char   SIS_HrsTime;    
unsigned char   SIS_MinTime;    
unsigned char   SIS_SecTime;    
unsigned char   SIS_HunTime;    
unsigned short  SIS_TimeZone;   
unsigned short  SIS_ClkIntrvl;  

unsigned char   SIS_DayDate;    
unsigned char   SIS_MonDate;    
unsigned short  SIS_YrsDate;    
unsigned char   SIS_DOWDate;    

unsigned char   SIS_VerMajor;   
unsigned char   SIS_VerMinor;   
unsigned char   SIS_RevLettr;   

#ifdef  OLDVER
unsigned char   CurScrnGrp;     
#else

unsigned char   SIS_CurScrnGrp; 

#endif

unsigned char   SIS_MaxScrnGrp; 
unsigned char   SIS_HugeShfCnt; 
unsigned char   SIS_ProtMdOnly; 
unsigned short  SIS_FgndPID;    

unsigned char   SIS_Dynamic;    
unsigned char   SIS_MaxWait;    
unsigned short  SIS_MinSlice;   
unsigned short  SIS_MaxSlice;   

unsigned short  SIS_BootDrv;    

unsigned char   SIS_mec_table[32]; 

unsigned char   SIS_MaxVioWinSG;  
unsigned char   SIS_MaxPresMgrSG; 

unsigned short  SIS_SysLog;     

unsigned short  SIS_MMIOBase;   
unsigned long   SIS_MMIOAddr;   

unsigned char   SIS_MaxVDMs;      
unsigned char   SIS_Reserved;
};

#pragma pack()

#endif 

