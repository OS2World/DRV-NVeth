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
#ifndef __PCI_H__
#define __PCI_H__

#define MAX_PCI_BASE_REGS	 4
#define MAX_PCI_DEVICES 	32	
#define MAX_PCI_FUNCTIONS	 8	

#define PCIREG_VENDOR_ID	0x00
#define PCIREG_DEVICE_ID	0x02
#define PCIREG_COMMAND		0x04
#define PCIREG_STATUS		0x06
#define PCIREG_CLASS_CODE	0x08
#define PCIREG_REVISION 	0x08
#define PCIREG_PROGIF		0x09
#define PCIREG_SUBCLASS 	0x0A
#define PCIREG_CLASS		0x0B
#define PCIREG_CACHE_LINE	0x0C
#define PCIREG_LATENCY		0x0D
#define PCIREG_HEADER_TYPE	0x0E
#define PCIREG_BAR0		0x10
#define PCIREG_BAR1		0x14
#define PCIREG_BAR2		0x18
#define PCIREG_BAR3		0x1C
#define PCIREG_BAR4		0x20
#define PCIREG_BAR5		0x24
#define PCIREG_SUBVENDOR	0x2C
#define PCIREG_SUBSYSTEM	0x2E
#define PCIREG_IO_BASE		PCIREG_BAR0
#define PCIREG_CAP	 	0x34
#define PCIREG_CAP_CARDBUS	0x14
#define PCIREG_INT_LINE 	0x3C
#define PCIREG_MIN_GRANT	0x3E
#define PCIREG_MAX_LAT		0x3F
#define PCIREG_SUBORDINATE_BUS	0x1A

#define  PCI_HEADER_TYPE_NORMAL		0
#define  PCI_HEADER_TYPE_BRIDGE		1
#define  PCI_HEADER_TYPE_CARDBUS	2

#define  PCI_STATUS_CAP_LIST		0x0010	
#define  PCI_STATUS_66MHZ		0x0020	
#define  PCI_STATUS_UDF			0x0040	
#define  PCI_STATUS_FAST_BACK		0x0080	
#define  PCI_STATUS_PARITY		0x0100	
#define  PCI_STATUS_DEVSEL_MASK		0x0600	
#define  PCI_STATUS_DEVSEL_FAST		0x0000
#define  PCI_STATUS_DEVSEL_MEDIUM	0x0200
#define  PCI_STATUS_DEVSEL_SLOW		0x0400
#define  PCI_STATUS_SIG_TARGET_ABORT	0x0800 
#define  PCI_STATUS_REC_TARGET_ABORT	0x1000 
#define  PCI_STATUS_REC_MASTER_ABORT	0x2000 
#define  PCI_STATUS_SIG_SYSTEM_ERROR	0x4000 
#define  PCI_STATUS_DETECTED_PARITY	0x8000 

#define PCI_CAP_LIST_ID		0	
#define  PCI_CAP_ID_PM		0x01	
#define  PCI_CAP_ID_AGP		0x02	
#define  PCI_CAP_ID_VPD		0x03	
#define  PCI_CAP_ID_SLOTID	0x04	
#define  PCI_CAP_ID_MSI		0x05	
#define  PCI_CAP_ID_CHSWP	0x06	
#define  PCI_CAP_ID_PCIX	0x07	
#define  PCI_CAP_ID_HT		0x08	
#define  PCI_CAP_ID_VNDR	0x09	
#define  PCI_CAP_ID_SHPC 	0x0C	
#define  PCI_CAP_ID_EXP 	0x10	
#define  PCI_CAP_ID_MSIX	0x11	
#define PCI_CAP_LIST_NEXT	1	
#define PCI_CAP_FLAGS		2	
#define PCI_CAP_SIZEOF		4

#define PCI_PM_PMC		2	
#define  PCI_PM_CAP_VER_MASK	0x0007	
#define  PCI_PM_CAP_PME_CLOCK	0x0008	
#define  PCI_PM_CAP_RESERVED    0x0010  
#define  PCI_PM_CAP_DSI		0x0020	
#define  PCI_PM_CAP_AUX_POWER	0x01C0	
#define  PCI_PM_CAP_D1		0x0200	
#define  PCI_PM_CAP_D2		0x0400	
#define  PCI_PM_CAP_PME		0x0800	
#define  PCI_PM_CAP_PME_MASK	0xF800	
#define  PCI_PM_CAP_PME_D0	0x0800	
#define  PCI_PM_CAP_PME_D1	0x1000	
#define  PCI_PM_CAP_PME_D2	0x2000	
#define  PCI_PM_CAP_PME_D3	0x4000	
#define  PCI_PM_CAP_PME_D3cold	0x8000	
#define PCI_PM_CTRL		4	
#define  PCI_PM_CTRL_STATE_MASK	0x0003	
#define  PCI_PM_CTRL_NO_SOFT_RESET	0x0004	
#define  PCI_PM_CTRL_PME_ENABLE	0x0100	
#define  PCI_PM_CTRL_DATA_SEL_MASK	0x1e00	
#define  PCI_PM_CTRL_DATA_SCALE_MASK	0x6000	
#define  PCI_PM_CTRL_PME_STATUS	0x8000	
#define PCI_PM_PPB_EXTENSIONS	6	
#define  PCI_PM_PPB_B2_B3	0x40	
#define  PCI_PM_BPCC_ENABLE	0x80	
#define PCI_PM_DATA_REGISTER	7	
#define PCI_PM_SIZEOF		8

#define PCI_AGP_VERSION		2	
#define PCI_AGP_RFU		3	
#define PCI_AGP_STATUS		4	
#define  PCI_AGP_STATUS_RQ_MASK	0xff000000	
#define  PCI_AGP_STATUS_SBA	0x0200	
#define  PCI_AGP_STATUS_64BIT	0x0020	
#define  PCI_AGP_STATUS_FW	0x0010	
#define  PCI_AGP_STATUS_RATE4	0x0004	
#define  PCI_AGP_STATUS_RATE2	0x0002	
#define  PCI_AGP_STATUS_RATE1	0x0001	
#define PCI_AGP_COMMAND		8	
#define  PCI_AGP_COMMAND_RQ_MASK 0xff000000  
#define  PCI_AGP_COMMAND_SBA	0x0200	
#define  PCI_AGP_COMMAND_AGP	0x0100	
#define  PCI_AGP_COMMAND_64BIT	0x0020 	
#define  PCI_AGP_COMMAND_FW	0x0010 	
#define  PCI_AGP_COMMAND_RATE4	0x0004	
#define  PCI_AGP_COMMAND_RATE2	0x0002	
#define  PCI_AGP_COMMAND_RATE1	0x0001	
#define PCI_AGP_SIZEOF		12

#define PCI_VPD_ADDR		2	
#define  PCI_VPD_ADDR_MASK	0x7fff	
#define  PCI_VPD_ADDR_F		0x8000	
#define PCI_VPD_DATA		4	

#define PCI_SID_ESR		2	
#define  PCI_SID_ESR_NSLOTS	0x1f	
#define  PCI_SID_ESR_FIC	0x20	
#define PCI_SID_CHASSIS_NR	3	

#define PCI_MSI_FLAGS		2	
#define  PCI_MSI_FLAGS_64BIT	0x80	
#define  PCI_MSI_FLAGS_QSIZE	0x70	
#define  PCI_MSI_FLAGS_QMASK	0x0e	
#define  PCI_MSI_FLAGS_ENABLE	0x01	
#define  PCI_MSI_FLAGS_MASKBIT	0x100	
#define PCI_MSI_RFU		3	
#define PCI_MSI_ADDRESS_LO	4	
#define PCI_MSI_ADDRESS_HI	8	
#define PCI_MSI_DATA_32		8	
#define PCI_MSI_DATA_64		12	
#define PCI_MSI_MASK_BIT	16	

#define PCI_MSIX_FLAGS_QSIZE	0x7FF
#define PCI_MSIX_FLAGS_ENABLE	(1 << 15)
#define PCI_MSIX_FLAGS_BIRMASK	(7 << 0)
#define PCI_MSIX_FLAGS_BITMASK	(1 << 0)

#define PCI_CHSWP_CSR		2	
#define  PCI_CHSWP_DHA		0x01	
#define  PCI_CHSWP_EIM		0x02	
#define  PCI_CHSWP_PIE		0x04	
#define  PCI_CHSWP_LOO		0x08	
#define  PCI_CHSWP_PI		0x30	
#define  PCI_CHSWP_EXT		0x40	
#define  PCI_CHSWP_INS		0x80	

#define PCI_X_CMD		2	
#define  PCI_X_CMD_DPERR_E	0x0001	
#define  PCI_X_CMD_ERO		0x0002	
#define  PCI_X_CMD_MAX_READ	0x000c	
#define  PCI_X_CMD_MAX_SPLIT	0x0070	
#define  PCI_X_CMD_VERSION(x) 	(((x) >> 12) & 3) 
#define PCI_X_STATUS		4	
#define  PCI_X_STATUS_DEVFN	0x000000ff	
#define  PCI_X_STATUS_BUS	0x0000ff00	
#define  PCI_X_STATUS_64BIT	0x00010000	
#define  PCI_X_STATUS_133MHZ	0x00020000	
#define  PCI_X_STATUS_SPL_DISC	0x00040000	
#define  PCI_X_STATUS_UNX_SPL	0x00080000	
#define  PCI_X_STATUS_COMPLEX	0x00100000	
#define  PCI_X_STATUS_MAX_READ	0x00600000	
#define  PCI_X_STATUS_MAX_SPLIT	0x03800000	
#define  PCI_X_STATUS_MAX_CUM	0x1c000000	
#define  PCI_X_STATUS_SPL_ERR	0x20000000	
#define  PCI_X_STATUS_266MHZ	0x40000000	
#define  PCI_X_STATUS_533MHZ	0x80000000	

#define PCI_EXP_FLAGS		2	
#define PCI_EXP_FLAGS_VERS	0x000f	
#define PCI_EXP_FLAGS_TYPE	0x00f0	
#define  PCI_EXP_TYPE_ENDPOINT	0x0	
#define  PCI_EXP_TYPE_LEG_END	0x1	
#define  PCI_EXP_TYPE_ROOT_PORT 0x4	
#define  PCI_EXP_TYPE_UPSTREAM	0x5	
#define  PCI_EXP_TYPE_DOWNSTREAM 0x6	
#define  PCI_EXP_TYPE_PCI_BRIDGE 0x7	
#define PCI_EXP_FLAGS_SLOT	0x0100	
#define PCI_EXP_FLAGS_IRQ	0x3e00	
#define PCI_EXP_DEVCAP		4	
#define  PCI_EXP_DEVCAP_PAYLOAD	0x07	
#define  PCI_EXP_DEVCAP_PHANTOM	0x18	
#define  PCI_EXP_DEVCAP_EXT_TAG	0x20	
#define  PCI_EXP_DEVCAP_L0S	0x1c0	
#define  PCI_EXP_DEVCAP_L1	0xe00	
#define  PCI_EXP_DEVCAP_ATN_BUT	0x1000	
#define  PCI_EXP_DEVCAP_ATN_IND	0x2000	
#define  PCI_EXP_DEVCAP_PWR_IND	0x4000	
#define  PCI_EXP_DEVCAP_PWR_VAL	0x3fc0000 
#define  PCI_EXP_DEVCAP_PWR_SCL	0xc000000 
#define PCI_EXP_DEVCTL		8	
#define  PCI_EXP_DEVCTL_CERE	0x0001	
#define  PCI_EXP_DEVCTL_NFERE	0x0002	
#define  PCI_EXP_DEVCTL_FERE	0x0004	
#define  PCI_EXP_DEVCTL_URRE	0x0008	
#define  PCI_EXP_DEVCTL_RELAX_EN 0x0010 
#define  PCI_EXP_DEVCTL_PAYLOAD	0x00e0	
#define  PCI_EXP_DEVCTL_EXT_TAG	0x0100	
#define  PCI_EXP_DEVCTL_PHANTOM	0x0200	
#define  PCI_EXP_DEVCTL_AUX_PME	0x0400	
#define  PCI_EXP_DEVCTL_NOSNOOP_EN 0x0800  
#define  PCI_EXP_DEVCTL_READRQ	0x7000	
#define PCI_EXP_DEVSTA		10	
#define  PCI_EXP_DEVSTA_CED	0x01	
#define  PCI_EXP_DEVSTA_NFED	0x02	
#define  PCI_EXP_DEVSTA_FED	0x04	
#define  PCI_EXP_DEVSTA_URD	0x08	
#define  PCI_EXP_DEVSTA_AUXPD	0x10	
#define  PCI_EXP_DEVSTA_TRPND	0x20	
#define PCI_EXP_LNKCAP		12	
#define PCI_EXP_LNKCTL		16	
#define  PCI_EXP_LNKCTL_CLKREQ_EN 0x100	
#define PCI_EXP_LNKSTA		18	
#define PCI_EXP_SLTCAP		20	
#define PCI_EXP_SLTCTL		24	
#define PCI_EXP_SLTSTA		26	
#define PCI_EXP_RTCTL		28	
#define  PCI_EXP_RTCTL_SECEE	0x01	
#define  PCI_EXP_RTCTL_SENFEE	0x02	
#define  PCI_EXP_RTCTL_SEFEE	0x04	
#define  PCI_EXP_RTCTL_PMEIE	0x08	
#define  PCI_EXP_RTCTL_CRSSVE	0x10	
#define PCI_EXP_RTCAP		30	
#define PCI_EXP_RTSTA		32	

#define PCI_EXT_CAP_ID(header)		(header & 0x0000ffff)
#define PCI_EXT_CAP_VER(header)		((header >> 16) & 0xf)
#define PCI_EXT_CAP_NEXT(header)	((header >> 20) & 0xffc)

#define PCI_EXT_CAP_ID_ERR	1
#define PCI_EXT_CAP_ID_VC	2
#define PCI_EXT_CAP_ID_DSN	3
#define PCI_EXT_CAP_ID_PWR	4

#define PCI_MASS_STORAGE	0x01	
#define PCI_IDE_CONTROLLER	0x01	
#define PCI_RAID_CONTROLLER	0x04	
#define PCI_SATA_CONTROLLER	0x06	
#define PCI_IDE_NATIVE_IF1	0x01
#define PCI_IDE_NATIVE_IF2	0x04
#define PCI_IDE_BUSMASTER	0x80
#define PCI_STATUS_MASTER_ABORT 0x2000
#define PCI_COMMAND_INTX_DIS	0x0400
#define PCI_CLASS_ISA_BRIDGE	0x0601
#define PCI_BARTYPE_IO		0x01

#define PCI_D0		 0
#define PCI_D1		 1
#define PCI_D2		 2
#define PCI_D3hot	 3
#define PCI_D3cold	 4
#define PCI_UNKNOWN	 5
#define PCI_POWER_ERROR	-1

#define  PCI_COMMAND_IO			0x1	
#define  PCI_COMMAND_MEMORY		0x2	
#define  PCI_COMMAND_MASTER		0x4	
#define  PCI_COMMAND_SPECIAL		0x8	
#define  PCI_COMMAND_INVALIDATE		0x10	
#define  PCI_COMMAND_VGA_PALETTE 	0x20	
#define  PCI_COMMAND_PARITY		0x40	
#define  PCI_COMMAND_WAIT 		0x80	
#define  PCI_COMMAND_SERR		0x100	
#define  PCI_COMMAND_FAST_BACK		0x200	
#define  PCI_COMMAND_INTX_DISABLE 	0x400	

#endif 
