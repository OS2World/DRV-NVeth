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
#ifndef __NVIDIA_H__
#define __NVIDIA_H__

#include <mii.h>

#define DEV_NEED_TIMERIRQ	0x00000001  
#define DEV_NEED_LINKTIMER	0x00000002	
#define DEV_HAS_LARGEDESC	0x00000004	
#define DEV_HAS_HIGH_DMA        0x00000008  
#define DEV_HAS_CHECKSUM        0x00000010  
#define DEV_HAS_VLAN            0x00000020  
#define DEV_HAS_MSI             0x00000040  
#define DEV_HAS_MSI_X           0x00000080  
#define DEV_HAS_POWER_CNTRL     0x00000100  
#define DEV_HAS_PAUSEFRAME_TX   0x00000200  
#define DEV_HAS_STATISTICS_V1   0x00000400  
#define DEV_HAS_STATISTICS_V2   0x00000800  
#define DEV_HAS_TEST_EXTENDED   0x00001000  
#define DEV_HAS_MGMT_UNIT       0x00002000  
#define DEV_NEED_PHY_INIT_FIX   0x00004000  
#define DEV_HAS_CORRECT_MACADDR 0x00008000  
#define DEV_HAS_COLLISION_FIX   0x00010000  

#define PCI_VENDOR_ID_NVIDIA			0x10de
#define PCI_DEVICE_ID_NVIDIA_NVENET_10		0x0037
#define PCI_DEVICE_ID_NVIDIA_NVENET_11		0x0038
#define PCI_DEVICE_ID_NVIDIA_NVENET_8		0x0056
#define PCI_DEVICE_ID_NVIDIA_NVENET_9		0x0057
#define PCI_DEVICE_ID_NVIDIA_NVENET_2		0x0066
#define PCI_DEVICE_ID_NVIDIA_NVENET_4		0x0086
#define PCI_DEVICE_ID_NVIDIA_NVENET_5		0x008c
#define PCI_DEVICE_ID_NVIDIA_NVENET_3		0x00d6
#define PCI_DEVICE_ID_NVIDIA_NVENET_7		0x00df
#define PCI_DEVICE_ID_NVIDIA_NVENET_6		0x00e6
#define PCI_DEVICE_ID_NVIDIA_NVENET_1		0x01c3
#define PCI_DEVICE_ID_NVIDIA_NVENET_12		0x0268
#define PCI_DEVICE_ID_NVIDIA_NVENET_13		0x0269
#define PCI_DEVICE_ID_NVIDIA_NVENET_14          0x0372
#define PCI_DEVICE_ID_NVIDIA_NVENET_15          0x0373
#define PCI_DEVICE_ID_NVIDIA_NVENET_16          0x03E5
#define PCI_DEVICE_ID_NVIDIA_NVENET_17          0x03E6
#define PCI_DEVICE_ID_NVIDIA_NVENET_18          0x03EE
#define PCI_DEVICE_ID_NVIDIA_NVENET_19          0x03EF
#define PCI_DEVICE_ID_NVIDIA_NVENET_20          0x0450
#define PCI_DEVICE_ID_NVIDIA_NVENET_21          0x0451
#define PCI_DEVICE_ID_NVIDIA_NVENET_22          0x0452
#define PCI_DEVICE_ID_NVIDIA_NVENET_23          0x0453
#define PCI_DEVICE_ID_NVIDIA_NVENET_24          0x054C
#define PCI_DEVICE_ID_NVIDIA_NVENET_25          0x054D
#define PCI_DEVICE_ID_NVIDIA_NVENET_26          0x054E
#define PCI_DEVICE_ID_NVIDIA_NVENET_27          0x054F
#define PCI_DEVICE_ID_NVIDIA_NVENET_28		0x07DC
#define PCI_DEVICE_ID_NVIDIA_NVENET_29		0x07DD
#define PCI_DEVICE_ID_NVIDIA_NVENET_30		0x07DE
#define PCI_DEVICE_ID_NVIDIA_NVENET_31		0x07DF
#define PCI_DEVICE_ID_NVIDIA_NVENET_32		0x0760
#define PCI_DEVICE_ID_NVIDIA_NVENET_33		0x0761
#define PCI_DEVICE_ID_NVIDIA_NVENET_34		0x0762
#define PCI_DEVICE_ID_NVIDIA_NVENET_35		0x0763
#define PCI_DEVICE_ID_NVIDIA_NVENET_36		0x0AB0
#define PCI_DEVICE_ID_NVIDIA_NVENET_37		0x0AB1
#define PCI_DEVICE_ID_NVIDIA_NVENET_38		0x0AB2
#define PCI_DEVICE_ID_NVIDIA_NVENET_39		0x0AB3
#define PCI_DEVICE_ID_NVIDIA_NVENET_40		0x0D7D

#define DESC_VER_1	1
#define DESC_VER_2	2
#define DESC_VER_3	3

#define NV_PKTLIMIT_1	1500	
#define NV_PKTLIMIT_2	9100	

#define NvRegIrqStatus		0x000
#define		 NVREG_IRQSTAT_MIIEVENT	0x040
#define		 NVREG_IRQSTAT_MASK		0x81ff
#define NvRegIrqMask		0x004
#define		 NVREG_IRQ_RX_ERROR		0x0001
#define		 NVREG_IRQ_RX			0x0002
#define		 NVREG_IRQ_RX_NOBUF		0x0004
#define		 NVREG_IRQ_TX_ERR		0x0008
#define		 NVREG_IRQ_TX_OK			0x0010
#define		 NVREG_IRQ_TIMER			0x0020
#define		 NVREG_IRQ_LINK			0x0040
#define		 NVREG_IRQ_RX_FORCED		0x0080
#define		 NVREG_IRQ_TX_FORCED		0x0100
#define		 NVREG_IRQ_RECOVER_ERROR		0x8000
#define		 NVREG_IRQMASK_THROUGHPUT	0x00df
#define		 NVREG_IRQMASK_CPU		0x0040
#define		 NVREG_IRQ_TX_ALL		(NVREG_IRQ_TX_ERR|NVREG_IRQ_TX_OK|NVREG_IRQ_TX_FORCED)
#define		 NVREG_IRQ_RX_ALL		(NVREG_IRQ_RX_ERROR|NVREG_IRQ_RX|NVREG_IRQ_RX_NOBUF|NVREG_IRQ_RX_FORCED)
#define		 NVREG_IRQ_OTHER			(NVREG_IRQ_TIMER|NVREG_IRQ_LINK|NVREG_IRQ_RECOVER_ERROR)
#define		 NVREG_IRQ_UNKNOWN	(~(NVREG_IRQ_RX_ERROR|NVREG_IRQ_RX|NVREG_IRQ_RX_NOBUF|NVREG_IRQ_TX_ERR| \
					NVREG_IRQ_TX_OK|NVREG_IRQ_TIMER|NVREG_IRQ_LINK|NVREG_IRQ_RX_FORCED| \
					NVREG_IRQ_TX_FORCED|NVREG_IRQ_RECOVER_ERROR))
#define NvRegUnknownSetupReg6		0x008
#define		 NVREG_UNKSETUP6_VAL		3
#define NvRegPollingInterval		0x00c
#define		 NVREG_POLL_DEFAULT_THROUGHPUT	970 
#define		 NVREG_POLL_DEFAULT_CPU	13
#define NvRegMSIMap0		0x020
#define NvRegMSIMap1		0x024
#define NvRegMSIIrqMask		0x030
#define		 NVREG_MSI_VECTOR_0_ENABLED 0x01
#define NvRegMisc1		0x080
#define		 NVREG_MISC1_PAUSE_TX	0x01
#define		 NVREG_MISC1_HD		0x02
#define		 NVREG_MISC1_FORCE	0x3b0f3c
#define NvRegMacReset		0x3c
#define		 NVREG_MAC_RESET_ASSERT	0x0F3
#define NvRegTransmitterControl		0x084
#define		 NVREG_XMITCTL_START	0x01
#define		 NVREG_XMITCTL_MGMT_ST	0x40000000
#define		 NVREG_XMITCTL_SYNC_MASK		0x000f0000
#define		 NVREG_XMITCTL_SYNC_NOT_READY	0x0
#define		 NVREG_XMITCTL_SYNC_PHY_INIT	0x00040000
#define		 NVREG_XMITCTL_MGMT_SEMA_MASK	0x00000f00
#define		 NVREG_XMITCTL_MGMT_SEMA_FREE	0x0
#define		 NVREG_XMITCTL_HOST_SEMA_MASK	0x0000f000
#define		 NVREG_XMITCTL_HOST_SEMA_ACQ	0x0000f000
#define		 NVREG_XMITCTL_HOST_LOADED	0x00004000
#define		 NVREG_XMITCTL_TX_PATH_EN	0x01000000
#define NvRegTransmitterStatus		0x088
#define		 NVREG_XMITSTAT_BUSY	0x01
#define NvRegPacketFilterFlags		0x8c
#define		 NVREG_PFF_PAUSE_RX	0x08
#define		 NVREG_PFF_ALWAYS	0x7F0000
#define		 NVREG_PFF_PROMISC	0x80
#define		 NVREG_PFF_MYADDR	0x20
#define		 NVREG_PFF_LOOPBACK	0x10
#define NvRegOffloadConfig		0x90
#define		 NVREG_OFFLOAD_HOMEPHY	0x601
#define		 NVREG_OFFLOAD_NORMAL	RX_NIC_BUFSIZE
#define NvRegReceiverControl		0x094
#define		 NVREG_RCVCTL_START	0x01
#define		 NVREG_RCVCTL_RX_PATH_EN	0x01000000
#define NvRegReceiverStatus		0x98
#define		 NVREG_RCVSTAT_BUSY	0x01
#define NvRegRandomSeed		0x9c
#define		 NVREG_RNDSEED_MASK	0x00ff
#define		 NVREG_RNDSEED_FORCE	0x7f00
#define		 NVREG_RNDSEED_FORCE2	0x2d00
#define		 NVREG_RNDSEED_FORCE3	0x7400
#define NvRegTxDeferral		0xA0
#define		 NVREG_TX_DEFERRAL_DEFAULT	0x15050f
#define		 NVREG_TX_DEFERRAL_RGMII_10_100	0x16070f
#define		 NVREG_TX_DEFERRAL_RGMII_1000	0x14050f
#define		 NVREG_TX_DEFERRAL_RGMII_STRETCH_10	0x16190f
#define		 NVREG_TX_DEFERRAL_RGMII_STRETCH_100	0x16300f
#define		 NVREG_TX_DEFERRAL_MII_STRETCH		0x152000
#define NvRegRxDeferral		0xA4
#define		 NVREG_RX_DEFERRAL_DEFAULT	0x16
#define NvRegMacAddrA		0xA8
#define NvRegMacAddrB		0xAC
#define NvRegMulticastAddrA		0xB0
#define		 NVREG_MCASTADDRA_FORCE	0x01
#define NvRegMulticastAddrB		0xB4
#define NvRegMulticastMaskA		0xB8
#define NvRegMulticastMaskB		0xBC
#define NvRegPhyInterface		0xC0
#define		 PHY_RGMII		0x10000000
#define NvRegTxRingPhysAddr		0x100
#define NvRegRxRingPhysAddr		0x104
#define NvRegRingSizes		0x108
#define		 NVREG_RINGSZ_TXSHIFT 0
#define		 NVREG_RINGSZ_RXSHIFT 16
#define NvRegTransmitPoll		0x10c
#define		 NVREG_TRANSMITPOLL_MAC_ADDR_REV	0x00008000
#define NvRegLinkSpeed		0x110
#define		 NVREG_LINKSPEED_FORCE 0x10000
#define		 NVREG_LINKSPEED_10	1000
#define		 NVREG_LINKSPEED_100	100
#define		 NVREG_LINKSPEED_1000	50
#define		 NVREG_LINKSPEED_MASK	(0xFFF)
#define NvRegUnknownSetupReg5		0x130
#define		 NVREG_UNKSETUP5_BIT31	0x80000000
#define NvRegTxWatermark		0x13c
#define		 NVREG_TX_WM_DESC1_DEFAULT	0x0200010
#define		 NVREG_TX_WM_DESC2_3_DEFAULT	0x1e08000
#define		 NVREG_TX_WM_DESC2_3_1000	0xfe08000
#define NvRegTxRxControl		0x144
#define		 NVREG_TXRXCTL_KICK	0x0001
#define		 NVREG_TXRXCTL_BIT1	0x0002
#define		 NVREG_TXRXCTL_BIT2	0x0004
#define		 NVREG_TXRXCTL_IDLE	0x0008
#define		 NVREG_TXRXCTL_RESET	0x0010
#define		 NVREG_TXRXCTL_RXCHECK	0x0400
#define		 NVREG_TXRXCTL_DESC_1	0
#define		 NVREG_TXRXCTL_DESC_2	0x002100
#define		 NVREG_TXRXCTL_DESC_3	0xc02200
#define		 NVREG_TXRXCTL_VLANSTRIP 0x00040
#define		 NVREG_TXRXCTL_VLANINS	0x00080
#define NvRegTxRingPhysAddrHigh		0x148
#define NvRegRxRingPhysAddrHigh		0x14C
#define NvRegTxPauseFrame		0x170
#define		 NVREG_TX_PAUSEFRAME_DISABLE	0x1ff0080
#define		 NVREG_TX_PAUSEFRAME_ENABLE	0x0c00030
#define NvRegMIIStatus		0x180
#define		 NVREG_MIISTAT_ERROR		0x0001
#define		 NVREG_MIISTAT_LINKCHANGE	0x0008
#define		 NVREG_MIISTAT_MASK		0x000f
#define		 NVREG_MIISTAT_MASK2		0x000f
#define NvRegMIIMask		0x184
#define		 NVREG_MII_LINKCHANGE		0x0008
#define NvRegAdapterControl		0x188
#define		 NVREG_ADAPTCTL_START	0x02
#define		 NVREG_ADAPTCTL_LINKUP	0x04
#define		 NVREG_ADAPTCTL_PHYVALID	0x40000
#define		 NVREG_ADAPTCTL_RUNNING	0x100000
#define		 NVREG_ADAPTCTL_PHYSHIFT	24
#define NvRegMIISpeed		0x18c
#define		 NVREG_MIISPEED_BIT8	0x100
#define		 NVREG_MIIDELAY	5
#define NvRegMIIControl		0x190
#define		 NVREG_MIICTL_INUSE	0x08000
#define		 NVREG_MIICTL_WRITE	0x00400
#define		 NVREG_MIICTL_ADDRSHIFT	5
#define NvRegMIIData		0x194
#define NvRegWakeUpFlags		0x200
#define		 NVREG_WAKEUPFLAGS_VAL		0x7770
#define		 NVREG_WAKEUPFLAGS_BUSYSHIFT	24
#define		 NVREG_WAKEUPFLAGS_ENABLESHIFT	16
#define		 NVREG_WAKEUPFLAGS_D3SHIFT	12
#define		 NVREG_WAKEUPFLAGS_D2SHIFT	8
#define		 NVREG_WAKEUPFLAGS_D1SHIFT	4
#define		 NVREG_WAKEUPFLAGS_D0SHIFT	0
#define		 NVREG_WAKEUPFLAGS_ACCEPT_MAGPAT		0x01
#define		 NVREG_WAKEUPFLAGS_ACCEPT_WAKEUPPAT	0x02
#define		 NVREG_WAKEUPFLAGS_ACCEPT_LINKCHANGE	0x04
#define		 NVREG_WAKEUPFLAGS_ENABLE	0x1111
#define NvRegPatternCRC		0x204
#define NvRegPatternMask		0x208
#define NvRegPowerCap		0x268
#define		 NVREG_POWERCAP_D3SUPP	0x40000000
#define		 NVREG_POWERCAP_D2SUPP	0x04000000
#define		 NVREG_POWERCAP_D1SUPP	0x02000000
#define NvRegPowerState		0x26c
#define		 NVREG_POWERSTATE_POWEREDUP	0x8000
#define		 NVREG_POWERSTATE_VALID		0x0100
#define		 NVREG_POWERSTATE_MASK		0x0003
#define		 NVREG_POWERSTATE_D0		0x0000
#define		 NVREG_POWERSTATE_D1		0x0001
#define		 NVREG_POWERSTATE_D2		0x0002
#define		 NVREG_POWERSTATE_D3		0x0003
#define NvRegTxCnt		0x280
#define NvRegTxZeroReXmt		0x284
#define NvRegTxOneReXmt		0x288
#define NvRegTxManyReXmt		0x28c
#define NvRegTxLateCol		0x290
#define NvRegTxUnderflow		0x294
#define NvRegTxLossCarrier		0x298
#define NvRegTxExcessDef		0x29c
#define NvRegTxRetryErr		0x2a0
#define NvRegRxFrameErr		0x2a4
#define NvRegRxExtraByte		0x2a8
#define NvRegRxLateCol		0x2ac
#define NvRegRxRunt		0x2b0
#define NvRegRxFrameTooLong		0x2b4
#define NvRegRxOverflow		0x2b8
#define NvRegRxFCSErr		0x2bc
#define NvRegRxFrameAlignErr		0x2c0
#define NvRegRxLenErr		0x2c4
#define NvRegRxUnicast		0x2c8
#define NvRegRxMulticast		0x2cc
#define NvRegRxBroadcast		0x2d0
#define NvRegTxDef		0x2d4
#define NvRegTxFrame		0x2d8
#define NvRegRxCnt		0x2dc
#define NvRegTxPause		0x2e0
#define NvRegRxPause		0x2e4
#define NvRegRxDropFrame		0x2e8
#define NvRegVlanControl		0x300
#define		 NVREG_VLANCONTROL_ENABLE	0x2000
#define NvRegMSIXMap0		0x3e0
#define NvRegMSIXMap1		0x3e4
#define NvRegMSIXIrqStatus		0x3f0
#define NvRegPowerState2		0x600
#define		NVREG_POWERSTATE2_POWERUP_MASK		0x0F11
#define		NVREG_POWERSTATE2_POWERUP_REV_A3	0x0001
#define		NVREG_POWERSTATE2_PHY_RESET		0x0004
#define 	NVREG_POWERSTATE2_GATE_CLOCKS		0x0F00

#define NV_MSI_X_MAX_VECTORS  8
#define NV_MSI_X_VECTORS_MASK 0x000f
#define NV_MSI_CAPABLE        0x0010
#define NV_MSI_X_CAPABLE      0x0020
#define NV_MSI_ENABLED        0x0040
#define NV_MSI_X_ENABLED      0x0080

#define NV_MSI_X_VECTOR_ALL   0x0
#define NV_MSI_X_VECTOR_RX    0x0
#define NV_MSI_X_VECTOR_TX    0x1
#define NV_MSI_X_VECTOR_OTHER 0x2

#define NV_PAUSEFRAME_RX_CAPABLE 0x0001
#define NV_PAUSEFRAME_TX_CAPABLE 0x0002
#define NV_PAUSEFRAME_RX_ENABLE  0x0004
#define NV_PAUSEFRAME_TX_ENABLE  0x0008
#define NV_PAUSEFRAME_RX_REQ     0x0010
#define NV_PAUSEFRAME_TX_REQ     0x0020
#define NV_PAUSEFRAME_AUTONEG    0x0040

#define RX_MAPSIZE		61440
#define RX_RING_DEFAULT		128
#define RX_GDT_COUNT		5
#define TX_RING_DEFAULT		256
#define TX_MAPSIZE 		61440
#define TX_GDT_COUNT		9
#define RING_MAX_DESC_VER_1	1024
#define RING_MAX_DESC_VER_2_3	16384

#define NV_PCI_REGSZ_VER1      	0x270
#define NV_PCI_REGSZ_VER2      	0x2d4
#define NV_PCI_REGSZ_VER3      	0x604

#define FLAG_MASK_V1 0xffff0000
#define FLAG_MASK_V2 0xffffc000
#define LEN_MASK_V1 (0xffffffff ^ FLAG_MASK_V1)
#define LEN_MASK_V2 (0xffffffff ^ FLAG_MASK_V2)

#define NV_TX_LASTPACKET	0x00010000
#define NV_TX_RETRYERROR	0x00080000
#define NV_TX_FORCED_INTERRUPT	0x01000000
#define NV_TX_DEFERRED		0x04000000
#define NV_TX_CARRIERLOST	0x08000000
#define NV_TX_LATECOLLISION	0x10000000
#define NV_TX_UNDERFLOW		0x20000000
#define NV_TX_ERROR		0x40000000
#define NV_TX_VALID		0x80000000

#define NV_TX2_LASTPACKET	0x20000000
#define NV_TX2_RETRYERROR	0x00040000
#define NV_TX2_FORCED_INTERRUPT	0x40000000
#define NV_TX2_DEFERRED		0x02000000
#define NV_TX2_CARRIERLOST	0x04000000
#define NV_TX2_LATECOLLISION	0x08000000
#define NV_TX2_UNDERFLOW	0x10000000

#define NV_TX2_ERROR		0x40000000
#define NV_TX2_VALID		0x80000000
#define NV_TX2_TSO		0x10000000
#define NV_TX2_TSO_SHIFT	14
#define NV_TX2_TSO_MAX_SHIFT	14
#define NV_TX2_TSO_MAX_SIZE	((dword)1<<NV_TX2_TSO_MAX_SHIFT)
#define NV_TX2_CHECKSUM_L3	0x08000000
#define NV_TX2_CHECKSUM_L4	0x04000000

#define NV_TX3_VLAN_TAG_PRESENT 0x00040000

#define NV_RX_DESCRIPTORVALID	0x00010000
#define NV_RX_MISSEDFRAME	0x00020000
#define NV_RX_SUBSTRACT1	0x00040000
#define NV_RX_ERROR1		0x00800000
#define NV_RX_ERROR2		0x01000000
#define NV_RX_ERROR3		0x02000000
#define NV_RX_ERROR4		0x04000000
#define NV_RX_CRCERR		0x08000000
#define NV_RX_OVERFLOW		0x10000000
#define NV_RX_FRAMINGERR	0x20000000
#define NV_RX_ERROR		0x40000000
#define NV_RX_AVAIL		0x80000000

#define NV_RX2_CHECKSUMMASK	(0x1C000000)
#define NV_RX2_CHECKSUMOK1	(0x10000000)
#define NV_RX2_CHECKSUMOK2	(0x14000000)
#define NV_RX2_CHECKSUMOK3	(0x18000000)
#define NV_RX2_DESCRIPTORVALID	0x20000000
#define NV_RX2_SUBSTRACT1	0x02000000
#define NV_RX2_ERROR1		0x00040000
#define NV_RX2_ERROR2		0x00080000
#define NV_RX2_ERROR3		0x00100000
#define NV_RX2_ERROR4		0x00200000
#define NV_RX2_CRCERR		0x00400000
#define NV_RX2_OVERFLOW		0x00800000
#define NV_RX2_FRAMINGERR	0x01000000

#define NV_RX2_ERROR		0x40000000
#define NV_RX2_AVAIL		0x80000000

#define NV_RX3_VLAN_TAG_PRESENT 0x00010000
#define NV_RX3_VLAN_TAG_MASK	(0x0000FFFF)

#define NV_OPTIMIZATION_MODE_THROUGHPUT 	0
#define NV_OPTIMIZATION_MODE_CPU		1

#define HZ		100
#define OOM_REFILL	(1+HZ/20)
#define POLL_WAIT	(1+HZ/100)
#define LINK_TIMEOUT	(3*HZ)
#define STATS_INTERVAL	(10*HZ)

#define NV_TXRX_RESET_DELAY	4
#define NV_TXSTOP_DELAY1	10
#define NV_TXSTOP_DELAY1MAX	500000
#define NV_TXSTOP_DELAY2	100
#define NV_RXSTOP_DELAY1	10
#define NV_RXSTOP_DELAY1MAX	500000
#define NV_RXSTOP_DELAY2	100
#define NV_SETUP5_DELAY		5
#define NV_SETUP5_DELAYMAX	50000
#define NV_POWERUP_DELAY	5
#define NV_POWERUP_DELAYMAX	5000
#define NV_MIIBUSY_DELAY	50
#define NV_MIIPHY_DELAY	10
#define NV_MIIPHY_DELAYMAX	10000
#define NV_MAC_RESET_DELAY	64

#define NV_WAKEUPPATTERNS	5
#define NV_WAKEUPMASKENTRIES	4

#define PHY_OUI_MARVELL		0x5043
#define PHY_OUI_CICADA		0x03f1
#define PHY_OUI_VITESSE		0x01c1
#define PHY_OUI_REALTEK		0x0732
#define PHY_OUI_REALTEK2	0x0020
#define PHYID1_OUI_MASK	0x03ff
#define PHYID1_OUI_SHFT	6
#define PHYID2_OUI_MASK	0xfc00
#define PHYID2_OUI_SHFT	10
#define PHYID2_MODEL_MASK		0x03f0
#define PHY_MODEL_REALTEK_8211		0x0110
#define PHY_REV_MASK			0x0001
#define PHY_REV_REALTEK_8211B		0x0000
#define PHY_REV_REALTEK_8211C		0x0001
#define PHY_MODEL_REALTEK_8201		0x0200
#define PHY_MODEL_MARVELL_E3016		0x0220
#define PHY_MARVELL_E3016_INITMASK	0x0300
#define PHY_CICADA_INIT1	0x0f000
#define PHY_CICADA_INIT2	0x0e00
#define PHY_CICADA_INIT3	0x01000
#define PHY_CICADA_INIT4	0x0200
#define PHY_CICADA_INIT5	0x0004
#define PHY_CICADA_INIT6	0x02000
#define PHY_VITESSE_INIT_REG1	0x1f
#define PHY_VITESSE_INIT_REG2	0x10
#define PHY_VITESSE_INIT_REG3	0x11
#define PHY_VITESSE_INIT_REG4	0x12
#define PHY_VITESSE_INIT_MSK1	0xc
#define PHY_VITESSE_INIT_MSK2	0x0180
#define PHY_VITESSE_INIT1	0x52b5
#define PHY_VITESSE_INIT2	0xaf8a
#define PHY_VITESSE_INIT3	0x8
#define PHY_VITESSE_INIT4	0x8f8a
#define PHY_VITESSE_INIT5	0xaf86
#define PHY_VITESSE_INIT6	0x8f86
#define PHY_VITESSE_INIT7	0xaf82
#define PHY_VITESSE_INIT8	0x0100
#define PHY_VITESSE_INIT9	0x8f82
#define PHY_VITESSE_INIT10	0x0
#define PHY_REALTEK_INIT_REG1	0x1f
#define PHY_REALTEK_INIT_REG2	0x19
#define PHY_REALTEK_INIT_REG3	0x13
#define PHY_REALTEK_INIT_REG4	0x14
#define PHY_REALTEK_INIT_REG5	0x18
#define PHY_REALTEK_INIT_REG6	0x11
#define PHY_REALTEK_INIT_REG7	0x01
#define PHY_REALTEK_INIT1	0x0000
#define PHY_REALTEK_INIT2	0x8e00
#define PHY_REALTEK_INIT3	0x0001
#define PHY_REALTEK_INIT4	0xad17
#define PHY_REALTEK_INIT5	0xfb54
#define PHY_REALTEK_INIT6	0xf5c7
#define PHY_REALTEK_INIT7	0x1000
#define PHY_REALTEK_INIT8	0x0003
#define PHY_REALTEK_INIT9	0x0008
#define PHY_REALTEK_INIT10	0x0005
#define PHY_REALTEK_INIT11	0x0200
#define PHY_REALTEK_INIT_MSK1	0x0003

#define PHY_GIGABIT			0x0100

#define PHY_TIMEOUT			0x1
#define PHY_ERROR			0x2

#define PHY_100				0x1
#define PHY_1000			0x2
#define PHY_HALF			0x100

#define PHY_RGMII			0x10000000

#define MII_READ	(-1)

#define NV_RX_HEADERS		(64)

#define NV_RX_ALLOC_PAD		(64)

#define NV_RX_ALLOCATE		2048
#define NV_TX_ALLOCATE		2048

#define NV_SETUP_RX_RING 0x01
#define NV_SETUP_TX_RING 0x02

typedef struct _ring_desc {
	dword buf;
	dword flaglen;
} ring_desc, far * pring_desc;

typedef struct _ring_desc_ex {
	dword bufhigh;
	dword buflow;
	dword txvlan;
	dword flaglen;
} ring_desc_ex, far * pring_desc_ex;

union ring_type {
	pring_desc orig;
	pring_desc_ex ex;
};

class NV_DEVICE : public PCI_DEVICE 
{
private:
	word desc_ver;
	dword txrxctl_bits;
	dword flags;
	word rx_csum;
	word msi_flags;
	word pause_flags;
	word register_size;
	fpbyte ba;
	byte mac[6];
	word irqmask;
	word need_linktimer;
	dword mac_in_use;
	word phy_model;
	word phyaddr;
	word phy_oui;
	word phy_rev;
	word gigabit;
	dword linkspeed;
	word duplex;
	word autoneg;
	word in_shutdown;
	word rxstarted;

	word vlanctl_bits;

	dword ring_addr;	
	word gdt[2];

	union ring_type get_rx, put_rx, first_rx, last_rx;

	dword rxdma;
	dword rxbufsize;

	union ring_type rx_ring;	
	dword rx_buf_sz;
	dword pkt_limit;
	word rx_ring_size;
	word rxgdt[RX_GDT_COUNT];
	fpbyte rx_virt_buf[RX_RING_DEFAULT];
	word rxpos;

	fpbyte currentrxbuffer;
	word currentlen;

	union ring_type get_tx, put_tx, first_tx, last_tx;

	union ring_type tx_ring;	
	word txpos;
	dword tx_flags;
	word tx_ring_size;
	word tx_stop;
	word tx_limit_stop;
	word tx_limit_start;
	dword txdma;
	dword txbufsize;
	word txgdt[TX_GDT_COUNT];
	fpbyte tx_virt_buf[TX_RING_DEFAULT];

	Mutex mutex;

public:
	NV_DEVICE() : PCI_DEVICE(), mutex() {}
	~NV_DEVICE() {}
	int setup();
	int open();
	void setFlags(dword newflags) { flags = newflags; }
	word IRQHandler();
	void setMcast();
	void setMac();
	void getHWStats();
	int startXmit(TxBufDesc far *pDsc);
	int releaseRX(word pos);
	int xferRX(fpword pcopied, word frameOffset, TDBufDesc far * pd);
private:
	dword readl(word reg) const { dword val; GetDWORD(ba + reg, (fpdword)&val); return val;}
	void writel(dword val, word reg) const { SetDWORD(ba + reg, val); }
	word reads(word reg) const { word val; GetWORD(ba + reg, (fpword)&val); return val;}
	void writes(word val, word reg) const { SetWORD(ba + reg, val); }
	byte readb(word reg) const { word val; GetBYTE(ba + reg, (fpbyte)&val); return (byte)(val & 0xFF);}
	void writeb(byte val, word reg) const { SetBYTE(ba + reg, val); }

	int mgmtAcquireSema();
	dword phyRW(word addr, word miireg, dword value) const;
	int regDelay(word offset, dword mask, dword target, word delay, long delaymax, char far * msg) const;
	int phyInit();
	int phyReset(dword bmcr_setup) const;
	void macReset() const;
	void txrxReset() const;
	void enableHWInterrupts(dword mask) const;
	void disableHWInterrupts(dword mask) const;
	void initRX();
	void initTX();
	int initRing();
	int allocRX();
	int allocTX();
	void drainRX();
	void drainTX();
	void drainRing();	
	void setupHWrings(word rxtx_flags) const;
	int updateLinkspeed();
	void updatePause(dword pause_flags);
	void startRX();
	void stopRX();
	void startTX();
	void stopTX();
	void linkChange();
	void linkIRQ();
	long getlen(fpbyte packet, long datalen) const;
	int rxProcess();
};

#define GDT_READ                (gdt[0])
#define GDT_WRITE               (gdt[1])

extern NV_DEVICE dev;
int checkDevice(NV_DEVICE &dev);

extern word optimization_mode;
extern word poll_interval;
extern word fixed_mode;
extern word max_interrupt_work;
extern word notimer;
extern word nolinktimer;
extern word dorxchain;

#endif 
