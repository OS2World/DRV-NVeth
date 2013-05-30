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

#ifndef __DEVHELP_H
#define __DEVHELP_H

#define PAGESIZE 4096

#define NULL (0)

#define NEAR near
#define FAR far

typedef unsigned short USHORT;
typedef unsigned char UCHAR;
typedef USHORT NEAR *NPUSHORT;
typedef void   NEAR *NPVOID;
typedef unsigned long BOOL;
typedef unsigned long ULONG;
typedef unsigned char NEAR *NPSZ;
typedef unsigned char far *PUCHAR;

#define DevInit(DeviceHelp) { Device_Help = DeviceHelp;}

#define OffsetFinalCS         	((word)&__OffFinalCS)
#define OffsetFinalDS         	((word)&__OffFinalDS)

#define SELECTOROF(p)       	(((word*)&(p))[1])
#define OFFSETOF(p)             (((word*)&(p))[0])

#define MAX_DEVNAME     8
#define MAX_DRIVERS     9

extern "C"
{

#pragma aux APIENTRY16 "^" far parm reverse routine [] \
  value struct float struct caller [] modify [ax bx cx dx];

word DosClose(word);
word DosDevIOCtl(fptr, fptr, word, word, word);
word DosOpen(fpchar, word far *, dword far *, dword, word, word, word, dword);
word DosWrite(word, fptr, word, word far*);
word DosPutMessage(word, word Length, fpchar Message);

#pragma aux (APIENTRY16) DosClose;
#pragma aux (APIENTRY16) DosDevIOCtl;
#pragma aux (APIENTRY16) DosOpen;
#pragma aux (APIENTRY16) DosWrite;
#pragma aux (APIENTRY16) DosPutMessage;

#pragma aux STRATEGY far parm [es bx];

extern byte __OffFinalCS;               
extern byte __OffFinalDS;               
extern dword Device_Help;
extern char DevName[];
extern word DevProtCS;
extern word DevProtDS;
extern word DevRealCS;
extern word DevRealDS;
extern word OemHlpDS;
extern dword OemHlpProc;
extern word  Delay500;
}

#pragma pack(1)

typedef word far cdecl ProtocolIDCEntry(word, fptr, word, word);
typedef ProtocolIDCEntry * PIDC;

struct IDC_Entry
{
    word reserverd[3];
    PIDC pFunc;
    word wDS;
};

#pragma pack()

word DevAttachDD(char* cName, IDC_Entry* pEntry);

word DevAllocGDTSel(word far* SelectorArray, word Count);

word DevPhysToGDTSelector(dword Address, word Length, word Selector);

word DevPhysToGDTSel(dword Address, dword Length, word Selector, word access, fpword modifiedSel);

word DevBlock(dword Id, dword WaitMilliSec, byte InterruptableFlag);

word DevRun(dword Id);

word DevPhysToVirt(dword PhysAddr, word Length, fptr far* VirtAddr);

word DevVerifyAccess(fptr Address, word Length, byte Type);

word DevFreeGDTSel(word Selector);

word DevGetDS(void);

word DevSetDS(word newDS);

void DevYield();

word DevPhysAlloc(dword size, byte loFlag, dword far * physAddr);
word DevPhysFree(dword PhysAddr);  

void DevCli();                   
void DevSti();                   

void DevClc();                   
void DevStc();                   

void DevEOI(word IRQ);

word DevIRQSet(word IRQ, word offset, byte SharableFlag);

word DevIRQClear(word IRQ);

void IODly(word count);

void GetDWORD(fptr address, fpdword holder);
void SetDWORD(fptr address, dword value);

void GetWORD(fptr address, fpword holder);
void SetWORD(fptr address, word value);

void GetBYTE(fptr address, fpbyte holder);
void SetBYTE(fptr address, byte value);

word DevXchg(fpword ptr, word value);

#define VMDHA_16M               0x0001                               
#define VMDHA_FIXED             0x0002                               
#define VMDHA_SWAP              0x0004                               
#define VMDHA_CONTIG            0x0008                               
#define VMDHA_PHYS              0x0010                               
#define VMDHA_PROCESS           0x0020                               
#define VMDHA_SGSCONT           0x0040                               
#define VMDHA_RESERVE           0x0100                               
#define VMDHA_USEHIGHMEM        0x0800                               

word CallOEMHlp(PRPH Pointer);

#pragma aux DevAttachDD =       	\
    	"mov dl,2Ah"                \
    	"call dword ptr Device_Help"  \
    	"setc dl"                   \
    	"xor dh,dh"                 \
    	parm [bx] [di]              \
    	value [dx];

#pragma aux DevAllocGDTSel =        \
        "mov dl,2Dh"                \
        "xor dh,dh"                 \
        "call dword ptr Device_Help"  \
        "setc dl"                   \
        "xor dh,dh"                 \
        "neg dx"                    \
        "and ax,dx"                 \
        modify [dx]                 \
        parm [es di] [cx]           \
        value [ax];

#pragma aux DevPhysToGDTSelector =       \
        "xchg ax,bx"                \
        "mov dl,2Eh"                \
        "call dword ptr Device_Help"  \
        "setc dl"                   \
        "xor dh,dh"                 \
        "neg dx"                    \
        "and ax,dx"                 \
        modify [dx]                 \
        parm [bx ax] [cx] [si]      \
        value [ax];

#pragma aux DevPhysToGDTSel = \
	"movzx   esp,sp" \
	"mov  eax,[esp]" \
	"mov  ecx,[esp+4]" \
	"mov  si,[esp+8]"           \
	"mov  dx,[esp+10]"          \
        "shl  dx,08h"               \
	"mov  dl,54h"               \
	"call dword ptr Device_Help"  \
	"jc   error" \
	"les  di,[esp+12]" \
	"mov  es:[di],si" \
	"xor  ax,ax" \
	"error:" \
	value [ax] \
	parm caller nomemory [] \
	modify exact [ax cx dx di es];

#pragma aux DevBlock =              \
        "xchg ax,bx"                \
        "xchg di,cx"                \
        "mov dl,04h"                \
        "call dword ptr Device_Help"  \
        "setnz al"                  \
        "setc bl"                   \
        "inc al"                    \
        "neg bl"                    \
        "and al,bl"                 \
        "xor ah,ah"                 \
        modify [dl]                 \
        parm [bx ax] [cx di] [dh]   \
        value [ax];

#pragma aux DevRun =                \
        "xchg ax,bx"                \
        "mov dl,05h"                \
        "call dword ptr Device_Help"  \
        modify [dl]                 \
        parm [bx ax]                \
        value [ax];

#pragma aux DevPhysToVirt =     	\
        "xchg ax,bx"                \
        "mov dx,0115h"              \
        "call dword ptr Device_Help"  \
        "setc dl"                   \
        "xor dh,dh"                 \
        "neg dx"                    \
        "mov fs:[si][0],di"         \
        "mov fs:[si][2],es"         \
        "and ax,dx"                 \
        modify [dx es di]           \
        parm [bx ax] [cx] [fs si]   \
        value [ax];

#pragma aux DevVerifyAccess =   	\
        "xchg ax,di"                \
        "mov dl,27h"                \
        "call dword ptr Device_Help"  \
        "setc dl"                   \
        "xor dh,dh"                 \
        modify [dl]                 \
        parm [di ax] [cx] [dh]      \
        value [dx];

#pragma aux DevFreeGDTSel =     	\
        "mov dl,53h"                \
        "call dword ptr Device_Help"  \
        "setc dl"                   \
        "xor dh,dh"                 \
        "neg dx"                    \
        "and ax,dx"                 \
        modify [dx]                 \
        parm [ax]                   \
        value [ax];

#pragma aux DevGetDS =          	\
        "mov ax, ds"            	\
        value [ax];

#pragma aux DevSetDS =              \
        "mov ax, ds"                \
        "mov ds, dx"                \
        parm [dx]                   \
        value [ax];

#pragma aux DevYield =          	\
        "mov dl,02h"                \
    	"call dword ptr Device_Help"  \
    	modify [dl];

#pragma aux DevPhysAlloc =    \
	"xchg ax,bx"                \
	"mov dl,18h"                \
	"call dword ptr Device_Help"  \
	"setc dl"                   \
	"xor dh,dh"                 \
	"neg dx"                    \
	"mov es:[si][0],bx"         \
	"mov es:[si][2],ax"         \
	"and ax,dx"                 \
	modify [dl]                 \
	parm [bx ax] [dh] [es si]   \
	value [ax];

#pragma aux DevPhysFree =     \
	"xchg ax,bx"                \
	"mov dl,19h"                \
	"call dword ptr Device_Help"  \
	"setc dl"                   \
	"xor dh,dh"                 \
	parm [bx ax]                \
	value [dx];

void DevInt3(void);
#pragma aux DevInt3 =           	\
        "int 3";

#pragma aux CallOEMHlp =       	\
	"push bx" \
	"push cx" \
	"push dx" \
	"push si" \
	"push di" \
	"push fs" \
	"push ds" \
	"pop fs" \
	"push ds" \
	"mov ds, OemHlpDS" \
	"call dword ptr fs:OemHlpProc" \
	"pop ds" \
	"pop fs" \
	"pop di" \
	"pop si" \
	"pop dx" \
	"pop cx" \
	"pop bx" \
	"mov ax, 0" \
        parm [es bx]   \
        value [ax];

#pragma aux IODly =    	\
	"mov cx, ax" \
	"inc cx" \
	"l: " \
	"loop l " \
        parm [ax]   \
        modify [cx];

#pragma aux DevCli =          \
	"cli";

#pragma aux DevSti =          \
	"sti";

#pragma aux DevClc =          \
	"clc";

#pragma aux DevStc =          \
	"stc";

#pragma aux DevEOI =          \
	"mov dl,31h"                \
	"call dword ptr Device_Help"  \
	modify [dl]                 \
	parm [ax];

#pragma aux DevIRQSet =       \
	"mov dl,1Bh"                \
	"call dword ptr Device_Help"  \
	"setc dl"                   \
	"xor dh,dh"                 \
	"neg dx"                    \
	"and ax,dx"                 \
	modify [dl]                 \
	parm [bx] [ax] [dh]         \
	value [ax];

#pragma aux DevIRQClear =     \
	"mov dl,1Ch"                \
	"call dword ptr Device_Help"  \
	"setc dl"                   \
	"xor dh,dh"                 \
	parm [bx]                   \
	value [dx];

#pragma aux SetDWORD =     	  \
	"movzx   esp,sp"          \
	"les  di,[esp]"           \
	"mov  eax,[esp+4]"        \
	"mov es:[di],eax"         \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux GetDWORD =     	    \
	"movzx   esp,sp"          \
	"les  di,[esp]"           \
	"mov eax, es:[di]"         \
	"les  di,[esp+4]"         \
	"mov es:[di],eax"         \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux SetWORD =     	  \
	"movzx   esp,sp"          \
	"les  di,[esp]"            \
	"mov  ax,[esp+4]"          \
	"mov es:[di],ax"          \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux GetWORD =             \
	"movzx   esp,sp"          \
	"les  di,[esp]"            \
	"mov ax, es:[di]"         \
	"les  di,[esp+4]"          \
	"mov es:[di],ax"          \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux SetBYTE =     	  \
	"movzx   esp,sp"          \
	"les  di,[esp]"            \
	"xor  ah,ah"              \
	"mov  al,[esp+4]"          \
	"mov es:[di],al"          \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux GetBYTE =             \
	"movzx   esp,sp"          \
	"les  di,[esp]"            \
	"mov al, es:[di]"         \
	"xor  ah,ah"              \
	"les  di,[esp+4]"          \
	"mov es:[di],al"          \
	parm caller nomemory []   \
	modify exact [ax di es];

#pragma aux DevXchg = \
        "xchg es:[di],ax"           \
        parm [es di] [ax]           \
        value [ax];

#endif  

