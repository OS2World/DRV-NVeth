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
#ifndef _OEM_H_
#define _OEM_H_

#include <pci.h>

#define OEMHLP_GETOEMADAPTIONINFO          0X0000
#define OEMHLP_GETMACHINEINFO              0X0001
#define OEMHLP_GETDISPLAYCOMBCODE          0X0002
#define OEMHLP_GETVIDEOFONTS               0X0003
#define OEMHLP_READEISACONFIGINFO          0X0004
#define OEMHLP_GETROMBIOSINFO              0X0005
#define OEMHLP_GETMISCVIDEOINFO            0X0006
#define OEMHLP_GETVIDEOADAPTER             0X0007
#define OEMHLP_GETSVGAINFO                 0X0008
#define OEMHLP_GETMEMINFO                  0X0009
#define OEMHLP_GETDMQSINFO                 0X000A
#define OEMHLP_PCI                         0x000B
#define OEMHLP_QUERYDISKINFO               0x000E

#define  OEMHLP_QUERY_PCI_BIOS      0x00
#define  OEMHLP_FIND_PCI_DEVICE     0x01
#define  OEMHLP_FIND_PCI_CLASS      0x02
#define  OEMHLP_READ_PCI_CONFIG     0x03
#define  OEMHLP_WRITE_PCI_CONFIG    0x04

#define  BUS_PCMCIA	 3
#define  BUS_EISA	 1
#define  BUS_PCI	 2
#define  BUS_UNKNOWN	 0

#define PCI_CONFIG_ADDRESS   0x0CF8
#define PCI_CONFIG_DATA      0x0CFC

#pragma pack(1)

typedef struct _PCI_IDENT {
   USHORT     Device;
   USHORT     Vendor;
   UCHAR      Index;	  
   UCHAR      Revision;   
   UCHAR      TModes;	  
   UCHAR      SGAlign;	  

   BOOL       (NEAR *ChipAccept) (struct _A NEAR *);
   NPUSHORT   CfgTable;
   void       (NEAR *PCIFunc_InitComplete)(struct _A NEAR *);
   int	      (NEAR *CheckIRQ) (struct _A NEAR *);
   USHORT     (NEAR *GetPIOMode) (struct _A NEAR *, UCHAR);
   void       (NEAR *Setup) (struct _A NEAR *);
   void       (NEAR *CalculateTiming) (struct _A NEAR *);
   void       (NEAR *TimingValue) (struct _U NEAR *);
   void       (NEAR *ProgramChip) (struct _A NEAR *);
#if ENABLEBUS
   void       (NEAR *EnableBus) (struct _A NEAR *, UCHAR);
#endif
   void       (NEAR *EnableInterrupts) (struct _A NEAR *);

   void       (NEAR *SetupTF) (struct _A NEAR *);
   void       (NEAR *SetupDMA) (struct _A NEAR *);
   void       (NEAR *StartDMA) (struct _A NEAR *);
   void       (NEAR *StopDMA) (struct _A NEAR *);
   void       (NEAR *ErrorDMA) (struct _A NEAR *);

   void       (NEAR *PreReset) (struct _A NEAR *);
   void       (NEAR *PostReset) (struct _A NEAR *);
   void       (NEAR *PostInitUnit) (struct _U NEAR *);
   void       (NEAR *StartStop) (struct _A NEAR *, UCHAR);
} PCI_IDENT, NEAR *NPPCI_IDENT;

typedef struct _PCI_INFO {
   USHORT     PCIAddr;
   PCI_IDENT  Ident;
   USHORT     CompatibleID;	
   struct _C NEAR *npC; 	
   UCHAR      Level;
} PCI_INFO, NEAR *NPPCI_INFO;

typedef struct _PCI_CAP {
	byte pos;
	byte type;
} PCI_CAP, FAR * PPCI_CAP;

typedef struct _PCI_BAR {
	dword bar;
	word io;
	word type;
	word prefetchable;
	dword start;
	dword size;
} PCI_BAR, FAR * PPCI_BAR;

#define MAXCAPCNT	32

class PCI_DEVICE {
protected:
	byte bus;
	byte func;
	word vendor;
	word device;
	word status;
	word command;
	byte headertype;
	word subvendor;
	word subsystemid;
	byte irq;
	byte ipin;
	PCI_CAP caps[MAXCAPCNT];
	byte capcnt;
	PCI_BAR bars[6];
	char name[20];
public:
	PCI_DEVICE();
	~PCI_DEVICE() {}
	int init(const byte newbus, const byte newfunc);
	long readConfig(const byte reg, const byte size) const;
	int writeConfig(const byte reg, const byte size, const dword data) const;
	int findCaps();
	int setPowerStateD0() const;
	int setBusMaster();
	PPCI_CAP operator[](const byte type) const;
	word getVendor() const {return vendor;}
	word getDevice() const {return device;}
	byte getIRQ() const { return irq; }
	void setName(fpchar drivername, fpchar devname);
	int registerRM();
};

#define PCIC_SUSPEND	    0x0001    
#define PCIC_RESUME	    0x0002    
#define PCIC_START	    0x0004    
#define PCIC_START_COMPLETE 0x0008    
#define PCIC_INIT_COMPLETE  0x0010    

typedef struct {
   UCHAR      SubFunc;
   USHORT     Device;
   USHORT     Vendor;
   UCHAR      Index;
}PCI_PARM_FIND_DEV;

typedef struct {
   UCHAR  SubFunc;
   ULONG  Class;
   UCHAR  Index;
}PCI_PARM_FIND_CLASSCODE, FAR *PPCI_PARM_FIND_CLASSCODE;

typedef struct {
   UCHAR  SubFunc;
   UCHAR  BusNum;
   UCHAR  DevFunc;
   UCHAR  ConfigReg;
   UCHAR  Size;
}PCI_PARM_READ_CONFIG, FAR *PPCI_PARM_READ_CONFIG;

typedef struct {
   UCHAR  SubFunc;
   UCHAR  BusNum;
   UCHAR  DevFunc;
   UCHAR  ConfigReg;
   UCHAR  Size;
   ULONG  Data;
}PCI_PARM_WRITE_CONFIG, FAR *PPCI_PARM_WRITE_CONFIG;

typedef struct _PCI_DATA {
   UCHAR rc;
   union {
      struct {
	 UCHAR HWMech;
	 UCHAR MajorVer;
	 UCHAR MinorVer;
	 UCHAR LastBus;
      } BiosInfo;
      struct {
	 UCHAR	BusNum;
	 UCHAR	DevFunc;
      } FindDev;
      union {
	 ULONG	ulData;
	 USHORT usData;
	 UCHAR	ucData;
      } Config;
   };
} PCI_DATA, FAR *PPCI_DATA;

#pragma pack()

typedef UCHAR	BYTE, NEAR *NPBYTE, FAR *PBYTE;
typedef USHORT	WORD, NEAR *NPWORD, FAR *PWORD;
typedef ULONG	DWORD, NEAR *NPDWORD, FAR *PDWORD;

word SetupOEMHlp(void);

#endif 
