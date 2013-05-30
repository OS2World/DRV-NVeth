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

NV_DEVICE dev;

word optimization_mode = NV_OPTIMIZATION_MODE_THROUGHPUT;
word poll_interval = -1;
word fixed_mode = 0;
word max_interrupt_work = 30;
word notimer = 1;
word nolinktimer = 1;
word dorxchain = 1;

static struct pci_device_id {
	fpchar	name;
	word	vendor;
	word	device;
	dword	flags;
} pci_tbl[] = {
	{	"nForce",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_1,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER,
	},
	{	"nForce2",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_2,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER,
	},
	{	"nForce3",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_3,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER,
	},
	{	"nForce3",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_4,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM,
	},
	{	"nForce3",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_5,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM,
	},
	{	"nForce3",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_6,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM,
	},
	{	"nForce3",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_7,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM,
	},
	{	"CK804",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_8,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_STATISTICS_V1,
	},
	{	"CK804",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_9,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_STATISTICS_V1,
	},
	{	"MCP04",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_10,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_STATISTICS_V1,
	},
	{	"MCP04",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_11,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_STATISTICS_V1,
	},
	{	"MCP51",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_12,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_STATISTICS_V1,
	},
	{	"MCP51",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_13,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_STATISTICS_V1,
	},
	{	"MCP55",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_14,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_VLAN|DEV_HAS_MSI|DEV_HAS_MSI_X|DEV_HAS_POWER_CNTRL|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT,
	},
	{	"MCP55",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_15,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_CHECKSUM|DEV_HAS_HIGH_DMA|DEV_HAS_VLAN|DEV_HAS_MSI|DEV_HAS_MSI_X|DEV_HAS_POWER_CNTRL|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT,
	},
	{	"MCP61",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_16,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP61",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_17,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP61",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_18,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP61",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_19,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP65",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_20,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP65",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_21,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP65",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_22,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP65",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_23,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_LARGEDESC|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP67",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_24,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP67",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_25,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP67",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_26,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP67",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_27,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR,
	},
	{	"MCP73",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_28,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP73",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_29,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP73",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_30,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP73",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_31,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP77",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_32,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP77",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_33,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP77",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_34,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP77",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_35,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP79",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_36,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP79",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_37,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP79",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_38,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP79",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_39,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{	"MCP89",
		PCI_VENDOR_ID_NVIDIA, PCI_DEVICE_ID_NVIDIA_NVENET_40,
		DEV_NEED_TIMERIRQ|DEV_NEED_LINKTIMER|DEV_HAS_HIGH_DMA|DEV_HAS_POWER_CNTRL|DEV_HAS_MSI|DEV_HAS_PAUSEFRAME_TX|DEV_HAS_STATISTICS_V2|DEV_HAS_TEST_EXTENDED|DEV_HAS_MGMT_UNIT|DEV_NEED_PHY_INIT_FIX|DEV_HAS_CORRECT_MACADDR|DEV_HAS_COLLISION_FIX,
	},
	{NULL, 0,0,0},
};

int checkDevice(NV_DEVICE &dev)
{
	for (int i = 0; pci_tbl[i].vendor; i++)
	{
		if (pci_tbl[i].vendor == dev.getVendor() && pci_tbl[i].device == dev.getDevice())
		{
			dev.setFlags(pci_tbl[i].flags);
			dev.setName("NVIDIA ", pci_tbl[i].name);
			return 1;
		}
	}
	return 0;
}

int NV_DEVICE::setup()
{
	int i;

	rxdma = 0;
	rxbufsize = 0;
	currentrxbuffer = NULL;
	currentlen = 0;

	txdma = 0;
	txbufsize = 0;

	AdapterSC.MscInterrupt = irq;

	vlanctl_bits = 0; 
	rxstarted = 0;

	{
		
		desc_ver = DESC_VER_1;
		txrxctl_bits = NVREG_TXRXCTL_DESC_1;
	}

	pkt_limit = NV_PKTLIMIT_1;
	if (flags & DEV_HAS_LARGEDESC)
	{
		pkt_limit = NV_PKTLIMIT_2;
	}

	if (flags & DEV_HAS_CHECKSUM) 
	{
		rx_csum = 1;
		txrxctl_bits |= NVREG_TXRXCTL_RXCHECK;
	}

	msi_flags = 0;
	if (flags & DEV_HAS_MSI) 
	{
		msi_flags |= NV_MSI_CAPABLE;
	}
	if (flags & DEV_HAS_MSI_X) 
	{
		msi_flags |= NV_MSI_X_CAPABLE;
	}

	pause_flags = NV_PAUSEFRAME_RX_CAPABLE | NV_PAUSEFRAME_RX_REQ | NV_PAUSEFRAME_AUTONEG;
	if (flags & DEV_HAS_PAUSEFRAME_TX) 
	{
		pause_flags |= NV_PAUSEFRAME_TX_CAPABLE | NV_PAUSEFRAME_TX_REQ;
	}

	if (flags & (DEV_HAS_VLAN|DEV_HAS_MSI_X|DEV_HAS_POWER_CNTRL|DEV_HAS_STATISTICS_V2))
	{
		register_size = NV_PCI_REGSZ_VER3;
	} else 
	if (flags & DEV_HAS_STATISTICS_V1)
	{
		register_size = NV_PCI_REGSZ_VER2;
	}
	else
	{
		register_size = NV_PCI_REGSZ_VER1;
	}

	ba = 0;
	for (i = 0; i < 6; i++)
	{
		if (bars[i].bar && !bars[i].io && !bars[i].type && bars[i].size >= register_size)
		{
			word size = min((word)bars[i].size, 0xF000);
			dword addr = mapPhysAddress(bars[i].start, size);
			if (!addr)
			{
				continue;
			}
			ba = (fpbyte)addr;
			break;
		}
	}
	if (!ba)
	{
		return -1;
	}

	rx_ring_size = RX_RING_DEFAULT;
	tx_ring_size = TX_RING_DEFAULT;
	dword ringsize = (rx_ring_size + tx_ring_size) * ((desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) ? sizeof(ring_desc) : sizeof(ring_desc_ex));
	ring_addr = allocPhysAddress(ringsize);
	if (!ring_addr)
	{
		return -1;
	}
	rx_ring.orig = (pring_desc)mapPhysAddress(ring_addr, ringsize);
	if (!rx_ring.orig)
	{
		return -1;
	}
	if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
	{
		tx_ring.orig = (pring_desc)&rx_ring.orig[rx_ring_size];
	} 
	else 
	{
		tx_ring.ex = (pring_desc_ex)&rx_ring.ex[rx_ring_size];
	}

	byte macaddr[6];
	for (i = 0; i < 6; i++)
	{
		macaddr[i] = ba[NvRegMacAddrA + i];
	}
	dword txreg = readl(NvRegTransmitPoll);
	if (flags & DEV_HAS_CORRECT_MACADDR || txreg & NVREG_TRANSMITPOLL_MAC_ADDR_REV) 
	{
		
		for (int i = 0; i < 6; i++)
		{
			mac[i] = macaddr[i];
		}
	} 
	else 
	{
		
		for (int i = 0; i < 6; i++)
		{
			mac[i] = macaddr[5 - i];
		}
		writel(txreg | NVREG_TRANSMITPOLL_MAC_ADDR_REV, NvRegTransmitPoll);
	}
	if (mac[0] & 0x01)
	{
		mac[0] = 0x00;
		mac[1] = 0x00;
		mac[2] = 0x6c;		
	}
	for (i = 0; i < 6; i++)
	{
		writeb(mac[i], NvRegMacAddrA + i);
		AdapterSC.MscPermStnAdr[i] = mac[i];
		AdapterSC.MscCurrStnAdr[i] = mac[i];
	}
	
	writel(NvRegWakeUpFlags, 0);
	if (flags & DEV_HAS_POWER_CNTRL) 
	{
		long rc = readConfig(PCIREG_REVISION, sizeof(byte));
		byte rev = 0;
		if (rc == -1)
		{
		}
		else
		{
			rev = (byte)rc;
		}

		dword powerstate = readl(NvRegPowerState2);
		powerstate &= ~NVREG_POWERSTATE2_POWERUP_MASK;
		if ((device == PCI_DEVICE_ID_NVIDIA_NVENET_12 ||
		     device == PCI_DEVICE_ID_NVIDIA_NVENET_13) &&
		    rev >= 0xA3)
		{
			powerstate |= NVREG_POWERSTATE2_POWERUP_REV_A3;
		}
		writel(powerstate, NvRegPowerState2);
	}
	if (desc_ver == DESC_VER_1) 
	{
		tx_flags = NV_TX_VALID;
	} 
	else 
	{
		tx_flags = NV_TX2_VALID;
	}
	if (optimization_mode == NV_OPTIMIZATION_MODE_THROUGHPUT) 
	{
		irqmask = NVREG_IRQMASK_THROUGHPUT;
		if (msi_flags & NV_MSI_X_CAPABLE) 
		{
			msi_flags |= 0x0003;
		}
	} 
	else 
	{
		irqmask = NVREG_IRQMASK_CPU;
		if (msi_flags & NV_MSI_X_CAPABLE) 
		{
			msi_flags |= 0x0001;
		}
	}

	if (!notimer && (flags & DEV_NEED_TIMERIRQ))
	{
		irqmask |= NVREG_IRQ_TIMER;
	}
	if (!notimer && !nolinktimer && (flags & DEV_NEED_LINKTIMER))
	{
		need_linktimer = 1;
	} 
	else 
	{
		need_linktimer = 0;
	}

	writel(0, NvRegMIIMask);
	dword phystate = readl(NvRegAdapterControl);
	if (phystate & NVREG_ADAPTCTL_RUNNING) 
	{
		phystate &= ~NVREG_ADAPTCTL_RUNNING;
		writel(phystate, NvRegAdapterControl);
	}
	writel(NVREG_MIISTAT_MASK, NvRegMIIStatus);

	word phyinitialized = 0;
	if (flags & DEV_HAS_MGMT_UNIT) 
	{
		
		dword tc = readl(NvRegTransmitterControl);
		if (tc & NVREG_XMITCTL_SYNC_PHY_INIT) 
		{
			mac_in_use = readl(NvRegTransmitterControl) & NVREG_XMITCTL_MGMT_ST;
			for (i = 0; i < 2 ; i++) 
			{
				msleep(32);
				if (mgmtAcquireSema()) 
				{
					
					if ((readl(NvRegTransmitterControl) & NVREG_XMITCTL_SYNC_MASK) == NVREG_XMITCTL_SYNC_PHY_INIT) 
					{
						
						phyinitialized = 1;
					}
					break;
				}
			}
		}
	}

	phyaddr = 0xFFFF;
	for (i = 1; i <= 32; i++) 
	{
		long res;
		word id1, id2;
		word addr = i & 0x1F;

		res = phyRW(addr, MII_PHYSID1, MII_READ);
		if (res == -1L || res == 0xffff)
		{
			continue;
		}
		id1 = (word)res;
		res = phyRW(addr, MII_PHYSID2, MII_READ);
		if (res == -1L || res == 0xffff)
		{
			continue;
		}
		id2 = (word)res;

		phy_model = (word)(id2 & PHYID2_MODEL_MASK);
		id1 = (id1 & PHYID1_OUI_MASK) << PHYID1_OUI_SHFT;
		id2 = (id2 & PHYID2_OUI_MASK) >> PHYID2_OUI_SHFT;
		phyaddr = addr;
		phy_oui = id1 | id2;

		if (phy_oui == PHY_OUI_REALTEK2)
		{
			phy_oui = PHY_OUI_REALTEK;
		}
		
		if (phy_oui == PHY_OUI_REALTEK && phy_model == PHY_MODEL_REALTEK_8211)
		{
			phy_rev = (word)phyRW(addr, MII_RESV1, MII_READ) & PHY_REV_MASK;
		}

		break;
	}
	if (phyaddr == 0xFFFF) 
	{
		return -1;
	}

	if (!phyinitialized) 
	{
		
		phyInit();
	} 
	else 
	{
		
		dword mii_status = phyRW(phyaddr, MII_BMSR, MII_READ);
		if (mii_status & PHY_GIGABIT) {
			gigabit = PHY_GIGABIT;
		}
	}

	linkspeed = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
	duplex = 0;
	autoneg = 1;
	
	return 0;
}

int NV_DEVICE::open()
{

	if (flags & DEV_HAS_POWER_CNTRL)
	{
		macReset();
	}
	writel(NVREG_MCASTADDRA_FORCE, NvRegMulticastAddrA);
	writel(0, NvRegMulticastAddrB);
	writel(0, NvRegMulticastMaskA);
	writel(0, NvRegMulticastMaskB);
	writel(0, NvRegPacketFilterFlags);

	writel(0, NvRegTransmitterControl);
	writel(0, NvRegReceiverControl);

	writel(0, NvRegAdapterControl);

	if (pause_flags & NV_PAUSEFRAME_TX_CAPABLE)
	{
		writel(NVREG_TX_PAUSEFRAME_DISABLE,  NvRegTxPauseFrame);
	}

	rx_buf_sz = ETH_DATA_LEN + NV_RX_HEADERS;

	int ringinitrc = initRing();
	if (ringinitrc)
	{
		return 1;
	}

	writel(0, NvRegLinkSpeed);
	writel(readl(NvRegTransmitPoll) & NVREG_TRANSMITPOLL_MAC_ADDR_REV, NvRegTransmitPoll);
	txrxReset();
	writel(0, NvRegUnknownSetupReg6);

	in_shutdown = 0;

	setupHWrings(NV_SETUP_RX_RING | NV_SETUP_TX_RING);
	writel((((dword)rx_ring_size-1) << NVREG_RINGSZ_RXSHIFT) + (((dword)tx_ring_size-1) << NVREG_RINGSZ_TXSHIFT), NvRegRingSizes);

	writel(linkspeed, NvRegLinkSpeed);
	if (desc_ver == DESC_VER_1)
	{
		writel(NVREG_TX_WM_DESC1_DEFAULT, NvRegTxWatermark);
	}
	else
	{
		writel(NVREG_TX_WM_DESC2_3_DEFAULT, NvRegTxWatermark);
	}
	writel(txrxctl_bits, NvRegTxRxControl);
	writel(vlanctl_bits, NvRegVlanControl);
	readl(0);
	writel(NVREG_TXRXCTL_BIT1|txrxctl_bits, NvRegTxRxControl);
	regDelay(NvRegUnknownSetupReg5, NVREG_UNKSETUP5_BIT31, NVREG_UNKSETUP5_BIT31,
			NV_SETUP5_DELAY, NV_SETUP5_DELAYMAX,
			"open: SetupReg5, Bit 31 remained off\r\n");

	writel(0, NvRegMIIMask);
	writel(NVREG_IRQSTAT_MASK, NvRegIrqStatus);
	writel(NVREG_MIISTAT_MASK2, NvRegMIIStatus);

	writel(NVREG_MISC1_FORCE | NVREG_MISC1_HD, NvRegMisc1);
	writel(readl(NvRegTransmitterStatus), NvRegTransmitterStatus);
	writel(NVREG_PFF_ALWAYS, NvRegPacketFilterFlags);
	writel(rx_buf_sz, NvRegOffloadConfig);

	writel(readl(NvRegReceiverStatus), NvRegReceiverStatus);
	dword i = HANDLE(sleephandle);
	writel(NVREG_RNDSEED_FORCE | (i & NVREG_RNDSEED_MASK), NvRegRandomSeed);
	writel(NVREG_TX_DEFERRAL_DEFAULT, NvRegTxDeferral);
	writel(NVREG_RX_DEFERRAL_DEFAULT, NvRegRxDeferral);
	if (poll_interval == -1) 
	{
		if (optimization_mode == NV_OPTIMIZATION_MODE_THROUGHPUT)
		{
			writel(NVREG_POLL_DEFAULT_THROUGHPUT, NvRegPollingInterval);
		}
		else
		{
			writel(NVREG_POLL_DEFAULT_CPU, NvRegPollingInterval);
		}
	}
	else
	{
		writel(poll_interval & 0xFFFF, NvRegPollingInterval);
	}
	writel(NVREG_UNKSETUP6_VAL, NvRegUnknownSetupReg6);
	writel(((dword)phyaddr << NVREG_ADAPTCTL_PHYSHIFT)|NVREG_ADAPTCTL_PHYVALID|NVREG_ADAPTCTL_RUNNING,
			NvRegAdapterControl);
	writel(NVREG_MIISPEED_BIT8|NVREG_MIIDELAY, NvRegMIISpeed);
	writel(NVREG_MII_LINKCHANGE, NvRegMIIMask);

	i = readl(NvRegPowerState);
	if ( (i & NVREG_POWERSTATE_POWEREDUP) == 0)
	{
		writel(NVREG_POWERSTATE_POWEREDUP | i, NvRegPowerState);
	}

	readl(0);
	udelay(10);
	writel(readl(NvRegPowerState) | NVREG_POWERSTATE_VALID, NvRegPowerState);

	disableHWInterrupts(irqmask);
	readl(0);
	writel(NVREG_MIISTAT_MASK2, NvRegMIIStatus);
	writel(NVREG_IRQSTAT_MASK, NvRegIrqStatus);
	readl(0);

	DevCli();
	writel(NVREG_MCASTADDRA_FORCE, NvRegMulticastAddrA);
	writel(0, NvRegMulticastAddrB);
	writel(0, NvRegMulticastMaskA);
	writel(0, NvRegMulticastMaskB);
	writel(NVREG_PFF_ALWAYS|NVREG_PFF_MYADDR, NvRegPacketFilterFlags);
	{
		dword miistat = readl(NvRegMIIStatus);
		writel(NVREG_MIISTAT_MASK, NvRegMIIStatus);
	}
	linkspeed = 0;
	int ret = updateLinkspeed();
	startRX();
	startTX();

	enableHWInterrupts(irqmask);

	DevCli();
	word rc = DevIRQSet(irq, (short)(long)irqhandler, 1);
	DevSti();
	if (rc)
	{
		drainRing();
		return 1;			
	}

	return 0;
}

int NV_DEVICE::mgmtAcquireSema()
{
	int i;
	dword mgmt_sema;

	for (i = 0; i < 10; i++) 
	{
		mgmt_sema = readl(NvRegTransmitterControl);
		mgmt_sema &= NVREG_XMITCTL_MGMT_SEMA_MASK;
		if (mgmt_sema == NVREG_XMITCTL_MGMT_SEMA_FREE)
		{
			break;
		}
		msleep(500);
	}

	int rc = 0;
	if (mgmt_sema == NVREG_XMITCTL_MGMT_SEMA_FREE)
	{
		for (i = 0; i < 2; i++) 
		{
			dword tx_ctrl = readl(NvRegTransmitterControl);
			tx_ctrl |= NVREG_XMITCTL_HOST_SEMA_ACQ;
			writel(tx_ctrl, NvRegTransmitterControl);

			tx_ctrl = readl(NvRegTransmitterControl);
			if (((tx_ctrl & NVREG_XMITCTL_HOST_SEMA_MASK) == NVREG_XMITCTL_HOST_SEMA_ACQ) &&
			    ((tx_ctrl & NVREG_XMITCTL_MGMT_SEMA_MASK) == NVREG_XMITCTL_MGMT_SEMA_FREE))
			{
				rc = 1;
				break;
			}
			else
			{
				udelay(50);
			}
		}
	}
	return rc;
}

dword NV_DEVICE::phyRW(word addr, word miireg, dword value) const
{
	dword reg;
	dword retval;

	writel(NVREG_MIISTAT_MASK, NvRegMIIStatus);

	reg = readl(NvRegMIIControl);
	if (reg & NVREG_MIICTL_INUSE) 
	{
		writel(NVREG_MIICTL_INUSE, NvRegMIIControl);
		udelay(NV_MIIBUSY_DELAY);
	}

	reg = ((dword)addr << NVREG_MIICTL_ADDRSHIFT) + miireg;
	if (value != MII_READ) 
	{
		writel(value, NvRegMIIData);
		reg |= NVREG_MIICTL_WRITE;
	}
	writel(reg, NvRegMIIControl);

	if (regDelay(NvRegMIIControl, NVREG_MIICTL_INUSE, 0,
			NV_MIIPHY_DELAY, NV_MIIPHY_DELAYMAX, NULL)) 
	{
		retval = -1L;
	} else 
	if (value != MII_READ) 
	{
		
		retval = 0;
	} 
	else 
	{
		dword status = readl(NvRegMIIStatus);
		dword data = readl(NvRegMIIData);
		if (status & NVREG_MIISTAT_ERROR) 
		{
			retval = -1L;
		} 
		else 
		{
			retval = data;
		}
	}
	return retval;
}

int NV_DEVICE::regDelay(word offset, dword mask, dword target,
				word delay, long delaymax, char far * msg) const
{
	
	readl(0);
	do 
	{
		udelay(delay);
		delaymax -= delay;
		if (delaymax < 0)
		{
			if (msg)
			{
			}
			return 1;
		}
	} while ((readl(offset) & mask) != target);
	return 0;
}

int NV_DEVICE::phyInit()
{
	dword phyinterface, phy_reserved, mii_status, mii_control, mii_control_1000, reg, powerstate;

	if (phy_model == PHY_MODEL_MARVELL_E3016) 
	{
		reg = phyRW(phyaddr, MII_NCONFIG, MII_READ);
		reg &= ~PHY_MARVELL_E3016_INITMASK;
		if (phyRW(phyaddr, MII_NCONFIG, reg)) 
		{
			return 1;
		}
	}

	if (phy_oui == PHY_OUI_REALTEK) 
	{
		if (phy_model == PHY_MODEL_REALTEK_8211 && phy_rev == PHY_REV_REALTEK_8211B) 
		{
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT1)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG2, PHY_REALTEK_INIT2)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT3)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG3, PHY_REALTEK_INIT4)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG4, PHY_REALTEK_INIT5)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG5, PHY_REALTEK_INIT6)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT1)) 
			{
				return 1;
			}
		}
		if (phy_model == PHY_MODEL_REALTEK_8211 && phy_rev == PHY_REV_REALTEK_8211C) 
		{

			powerstate = readl(NvRegPowerState2);

			powerstate |= NVREG_POWERSTATE2_PHY_RESET;
			writel(powerstate, NvRegPowerState2);
			msleep(25);

			powerstate &= ~NVREG_POWERSTATE2_PHY_RESET;
			writel(powerstate, NvRegPowerState2);
			msleep(25);

			reg = phyRW(phyaddr, PHY_REALTEK_INIT_REG6, MII_READ);
			reg |= PHY_REALTEK_INIT9;
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG6, reg)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT10)) 
			{
				return 1;
			}
			reg = phyRW(phyaddr, PHY_REALTEK_INIT_REG7, MII_READ);
			if (!(reg & PHY_REALTEK_INIT11)) 
			{
				reg |= PHY_REALTEK_INIT11;
				if (phyRW(phyaddr, PHY_REALTEK_INIT_REG7, reg)) 
				{
					return 1;
				}
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT1)) 
			{
				return 1;
			}
		}
		if (phy_model == PHY_MODEL_REALTEK_8201) 
		{
			if (flags & DEV_NEED_PHY_INIT_FIX) 
			{
				phy_reserved = phyRW(phyaddr, PHY_REALTEK_INIT_REG6, MII_READ);
				phy_reserved |= PHY_REALTEK_INIT7;
				if (phyRW(phyaddr, PHY_REALTEK_INIT_REG6, phy_reserved)) 
				{
					return 1;
				}
			}
		}
	}

	reg = phyRW(phyaddr, MII_ADVERTISE, MII_READ);
	reg |= (ADVERTISE_10HALF|ADVERTISE_10FULL|ADVERTISE_100HALF|ADVERTISE_100FULL|ADVERTISE_PAUSE_ASYM|ADVERTISE_PAUSE_CAP);
	if (phyRW(phyaddr, MII_ADVERTISE, reg)) 
	{
		return 1;
	}

	phyinterface = readl(NvRegPhyInterface);

	mii_status = phyRW(phyaddr, MII_BMSR, MII_READ);
	if (mii_status & PHY_GIGABIT) 
	{
		gigabit = PHY_GIGABIT;
		mii_control_1000 = phyRW(phyaddr, MII_CTRL1000, MII_READ);
		mii_control_1000 &= ~ADVERTISE_1000HALF;
		if (phyinterface & PHY_RGMII)
		{
			mii_control_1000 |= ADVERTISE_1000FULL;
		}
		else
		{
			mii_control_1000 &= ~ADVERTISE_1000FULL;
		}

		if (phyRW(phyaddr, MII_CTRL1000, mii_control_1000)) 
		{
			return 1;
		}
	}
	else
	{
		gigabit = 0;
	}

	mii_control = phyRW(phyaddr, MII_BMCR, MII_READ);
	mii_control |= BMCR_ANENABLE;

	if (phy_oui == PHY_OUI_REALTEK 
	    && phy_model == PHY_MODEL_REALTEK_8211
	    && phy_rev == PHY_REV_REALTEK_8211C) 
	{
		
		mii_control |= BMCR_ANRESTART;
		if (phyRW(phyaddr, MII_BMCR, mii_control)) 
		{
			return 1;
		}
	} 
	else 
	{

		if (phyReset(mii_control)) 
		{
			return 1;
		}
	}

	if (phy_oui == PHY_OUI_CICADA && phyinterface & PHY_RGMII) 
	{
		phy_reserved = phyRW(phyaddr, MII_RESV1, MII_READ);
		phy_reserved &= ~(PHY_CICADA_INIT1 | PHY_CICADA_INIT2);
		phy_reserved |= (PHY_CICADA_INIT3 | PHY_CICADA_INIT4);
		if (phyRW(phyaddr, MII_RESV1, phy_reserved)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, MII_NCONFIG, MII_READ);
		phy_reserved |= PHY_CICADA_INIT5;
		if (phyRW(phyaddr, MII_NCONFIG, phy_reserved)) 
		{
			return 1;
		}
	}
	if (phy_oui == PHY_OUI_CICADA) 
	{
		phy_reserved = phyRW(phyaddr, MII_SREVISION, MII_READ);
		phy_reserved |= PHY_CICADA_INIT6;
		if (phyRW(phyaddr, MII_SREVISION, phy_reserved)) 
		{
			return 1;
		}
	}
	if (phy_oui == PHY_OUI_VITESSE) 
	{
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG1, PHY_VITESSE_INIT1)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT2)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG4, MII_READ);
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG4, phy_reserved)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG3, MII_READ);
		phy_reserved &= ~PHY_VITESSE_INIT_MSK1;
		phy_reserved |= PHY_VITESSE_INIT3;
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG3, phy_reserved)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT4)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT5)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG4, MII_READ);
		phy_reserved &= ~PHY_VITESSE_INIT_MSK1;
		phy_reserved |= PHY_VITESSE_INIT3;
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG4, phy_reserved)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG3, MII_READ);
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG3, phy_reserved)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT6)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT7)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG4, MII_READ);
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG4, phy_reserved)) 
		{
			return 1;
		}
		phy_reserved = phyRW(phyaddr, PHY_VITESSE_INIT_REG3, MII_READ);
		phy_reserved &= ~PHY_VITESSE_INIT_MSK2;
		phy_reserved |= PHY_VITESSE_INIT8;
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG3, phy_reserved)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG2, PHY_VITESSE_INIT9)) 
		{
			return 1;
		}
		if (phyRW(phyaddr, PHY_VITESSE_INIT_REG1, PHY_VITESSE_INIT10)) 
		{
			return 1;
		}
	}
	if (phy_oui == PHY_OUI_REALTEK) 
	{
		if (phy_model == PHY_MODEL_REALTEK_8211 && phy_rev == PHY_REV_REALTEK_8211B) 
		{
			
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT1)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG2, PHY_REALTEK_INIT2)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT3)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG3, PHY_REALTEK_INIT4)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG4, PHY_REALTEK_INIT5)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG5, PHY_REALTEK_INIT6)) 
			{
				return 1;
			}
			if (phyRW(phyaddr, PHY_REALTEK_INIT_REG1, PHY_REALTEK_INIT1)) 
			{
				return 1;
			}
		}
		if (phy_model == PHY_MODEL_REALTEK_8201) 
		{
			if (flags & DEV_NEED_PHY_INIT_FIX) 
			{
				phy_reserved = phyRW(phyaddr, PHY_REALTEK_INIT_REG6, MII_READ);
				phy_reserved |= PHY_REALTEK_INIT7;
				if (phyRW(phyaddr, PHY_REALTEK_INIT_REG6, phy_reserved)) 
				{
					return 1;
				}
			}
		}
	}

	phyRW(phyaddr, MII_ADVERTISE, reg);

	mii_control = phyRW(phyaddr, MII_BMCR, MII_READ);
	mii_control |= (BMCR_ANRESTART | BMCR_ANENABLE);
	if (phyRW(phyaddr, MII_BMCR, mii_control)) 
	{
		return 1;
	}

	return 0;
}

int NV_DEVICE::phyReset(dword bmcr_setup) const
{
	dword miicontrol;
	word tries = 0;

	miicontrol = BMCR_RESET | bmcr_setup;
	if (phyRW(phyaddr, MII_BMCR, miicontrol)) 
	{
		return 1;
	}

	msleep(500);

	while (miicontrol & BMCR_RESET) 
	{
		msleep(32);
		miicontrol = phyRW(phyaddr, MII_BMCR, MII_READ);
		
		if (tries++ > 100)
		{
			return 1;
		}
	}
	return 0;
}

void NV_DEVICE::macReset() const
{
	writel(NVREG_TXRXCTL_BIT2 | NVREG_TXRXCTL_RESET | txrxctl_bits, NvRegTxRxControl);
	readl(0);
	writel(NVREG_MAC_RESET_ASSERT, NvRegMacReset);
	readl(0);
	udelay(NV_MAC_RESET_DELAY);
	writel(0, NvRegMacReset);
	readl(0);
	udelay(NV_MAC_RESET_DELAY);
	writel(NVREG_TXRXCTL_BIT2 | txrxctl_bits, NvRegTxRxControl);
	readl(0);
}

void NV_DEVICE::startRX()
{
	dword rx_ctrl = readl(NvRegReceiverControl);

	if ((readl(NvRegReceiverControl) & NVREG_RCVCTL_START) && !mac_in_use) 
	{
		rx_ctrl &= ~NVREG_RCVCTL_START;
		writel(rx_ctrl, NvRegReceiverControl);
		readl(0);
	}
	writel(linkspeed, NvRegLinkSpeed);
	readl(0);
        rx_ctrl |= NVREG_RCVCTL_START;
        if (mac_in_use)
	{
		rx_ctrl &= ~NVREG_RCVCTL_RX_PATH_EN;
	}
	writel(rx_ctrl, NvRegReceiverControl);
	rxstarted = 1;
	readl(0);
}

void NV_DEVICE::stopRX()
{
	dword rx_ctrl = readl(NvRegReceiverControl);

	if (!mac_in_use)
	{
		rx_ctrl &= ~NVREG_RCVCTL_START;
	}
	else
	{
		rx_ctrl |= NVREG_RCVCTL_RX_PATH_EN;
	}
	writel(rx_ctrl, NvRegReceiverControl);
	regDelay(NvRegReceiverStatus, NVREG_RCVSTAT_BUSY, 0,
			NV_RXSTOP_DELAY1, NV_RXSTOP_DELAY1MAX,
			"stop rx: ReceiverStatus remained busy");

	udelay(NV_RXSTOP_DELAY2);
	if (!mac_in_use)
	{
		writel(0, NvRegLinkSpeed);
	}
	rxstarted = 0;
}

void NV_DEVICE::startTX()
{
	dword tx_ctrl = readl(NvRegTransmitterControl);

	tx_ctrl |= NVREG_XMITCTL_START;
	if (mac_in_use)
	{
		tx_ctrl &= ~NVREG_XMITCTL_TX_PATH_EN;
	}
	writel(tx_ctrl, NvRegTransmitterControl);
	readl(0);
}

void NV_DEVICE::stopTX()
{
	dword tx_ctrl = readl(NvRegTransmitterControl);

	if (!mac_in_use)
	{
		tx_ctrl &= ~NVREG_XMITCTL_START;
	}
	else
	{
		tx_ctrl |= NVREG_XMITCTL_TX_PATH_EN;
	}
	writel(tx_ctrl, NvRegTransmitterControl);
	regDelay(NvRegTransmitterStatus, NVREG_XMITSTAT_BUSY, 0,
			NV_TXSTOP_DELAY1, NV_TXSTOP_DELAY1MAX,
			"stop tx: TransmitterStatus remained busy");

	udelay(NV_TXSTOP_DELAY2);
	if (!mac_in_use)
	{
		writel(readl(NvRegTransmitPoll) & NVREG_TRANSMITPOLL_MAC_ADDR_REV,
		       NvRegTransmitPoll);
	}
}

void NV_DEVICE::txrxReset() const
{
	writel(NVREG_TXRXCTL_BIT2 | NVREG_TXRXCTL_RESET | txrxctl_bits, NvRegTxRxControl);
	readl(0);
	udelay(NV_TXRX_RESET_DELAY);
	writel(NVREG_TXRXCTL_BIT2 | txrxctl_bits, NvRegTxRxControl);
	readl(0);
}

void NV_DEVICE::enableHWInterrupts(dword mask) const
{
	writel(mask, NvRegIrqMask);
}

void NV_DEVICE::disableHWInterrupts(dword mask) const
{
	if (msi_flags & NV_MSI_X_ENABLED) 
	{
		writel(mask, NvRegIrqMask);
	} 
	else 
	{
		if (msi_flags & NV_MSI_ENABLED)
		{
			writel(0, NvRegMSIIrqMask);
		}
		writel(0, NvRegIrqMask);
	}
}

void NV_DEVICE::initRX()
{
	int i;
	get_rx = put_rx = first_rx = rx_ring;
	if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2)
	{
		last_rx.orig = &rx_ring.orig[rx_ring_size-1];
	}
	else
	{
		last_rx.ex = &rx_ring.ex[rx_ring_size-1];
	}

	for (i = 0; i < rx_ring_size; i++) 
	{
		if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
		{
			rx_ring.orig[i].flaglen = 0;
			rx_ring.orig[i].buf = 0;
		} 
		else 
		{
			rx_ring.ex[i].flaglen = 0;
			rx_ring.ex[i].txvlan = 0;
			rx_ring.ex[i].bufhigh = 0;
			rx_ring.ex[i].buflow = 0;
		}
	}
}

void NV_DEVICE::initTX()
{
	rxpos = 0;
	txpos = 0;
	get_tx = put_tx = first_tx = tx_ring;
	if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2)
	{
		last_tx.orig = &tx_ring.orig[tx_ring_size-1];
	}
	else
	{
		last_tx.ex = &tx_ring.ex[tx_ring_size-1];
	}

	word i;
	for (i = 0; i < tx_ring_size; i++) 
	{
		if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
		{
			tx_ring.orig[i].flaglen = 0;
			tx_ring.orig[i].buf = 0;
		} 
		else 
		{
			tx_ring.ex[i].flaglen = 0;
			tx_ring.ex[i].txvlan = 0;
			tx_ring.ex[i].bufhigh = 0;
			tx_ring.ex[i].buflow = 0;
		}
	}
}

int NV_DEVICE::allocRX()
{
	word rc = DevAllocGDTSel((word far*)rxgdt, RX_GDT_COUNT);
	if (rc)
	{
		return 1;
	}
	rxbufsize = (dword)rx_ring_size * NV_RX_ALLOCATE;
	rxdma = allocPhysAddress(rxbufsize);
	word i, j;
	dword p;
	for (i = 0; i < rx_ring_size; i++)
	{
		rx_virt_buf[i] = NULL;
	}

	if (!rxdma)
	{
		rc = 1;
	}
	else
	for (p = rxdma, i = 0, j = 0; i < RX_GDT_COUNT && p < rxdma + rxbufsize; i++, p += RX_MAPSIZE)
	{
		dword mapsize = min((dword)RX_MAPSIZE, rxbufsize + rxdma - p);
		fpbyte virt = (fpbyte)mapPhysAddress(rxgdt[i], p, mapsize);
		if (!virt)
		{
			rc = 1;
			break;
		}
		for (dword vp = (dword)virt; vp < (dword)virt + mapsize; j++, vp += NV_RX_ALLOCATE)
		{
			rx_virt_buf[j] = (fpbyte)vp;
		}
	}
	if (rc)
	{
		if (rxdma)
		{
			DevPhysFree(rxdma);
			rxdma = 0;
		}
		for (i = 0; i < RX_GDT_COUNT; i++)
		{
			DevFreeGDTSel(rxgdt[i]);
		}
		return rc;
	}
	dword flaglen = cpu_to_le32((rx_buf_sz + NV_RX_AVAIL));
	for (i = 0; i < rx_ring_size; i++)
	{
		rx_ring.orig[i].buf = cpu_to_le32(rxdma + (dword)i * NV_RX_ALLOCATE);
		rx_ring.orig[i].flaglen = flaglen;
	}
	return 0;
}

int NV_DEVICE::allocTX()
{
	word rc = DevAllocGDTSel((word far*)txgdt, TX_GDT_COUNT);
	if (rc)
	{
		return 1;
	}
	word i,j;
	dword p;
	for (i = 0; i < tx_ring_size; i++)
	{
		tx_virt_buf[i] = NULL;
	}
	txbufsize = (dword)tx_ring_size * NV_TX_ALLOCATE;
	txdma = allocPhysAddress(txbufsize);
	if (!txdma)
	{
		rc = 1;
	}
	else
	for (p = txdma, i = 0, j = 0; i < TX_GDT_COUNT && p < txdma + txbufsize; i++, p += TX_MAPSIZE)
	{
		dword mapsize = min((dword)TX_MAPSIZE, txbufsize + txdma - p);
		fpbyte virt = (fpbyte)mapPhysAddress(txgdt[i], p, mapsize);
		if (!virt)
		{
			rc = 1;
			break;
		}
		for (dword vp = (dword)virt; vp < (dword)virt + mapsize; j++, vp += NV_TX_ALLOCATE)
		{
			tx_virt_buf[j] = (fpbyte)vp;
		}
	}
	if (rc)
	{
		if (txdma)
		{
			DevPhysFree(txdma);
			txdma = 0;
		}
		for (i = 0; i < TX_GDT_COUNT; i++)
		{
			DevFreeGDTSel(txgdt[i]);
		}
		return rc;
	}
	return 0;
}

int NV_DEVICE::initRing()
{
	int rc = DevAllocGDTSel(gdt, 2);
	if (rc)
	{
		return 1;
	}
	initTX();
	initRX();
	return (allocRX() + allocTX());
}

void NV_DEVICE::drainTX()
{
	for (int i = 0; i < tx_ring_size; i++) 
	{
		if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
		{
			tx_ring.orig[i].flaglen = 0;
			tx_ring.orig[i].buf = 0;
		} 
		else 
		{
			tx_ring.ex[i].flaglen = 0;
			tx_ring.ex[i].txvlan = 0;
			tx_ring.ex[i].bufhigh = 0;
			tx_ring.ex[i].buflow = 0;
		}
	}
}

void NV_DEVICE::drainRX()
{
	dword dma = rx_ring.orig[0].buf;
	for (int i = 0; i < rx_ring_size; i++) 
	{
		if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
		{
			rx_ring.orig[i].flaglen = 0;
			rx_ring.orig[i].buf = 0;
		} 
		else 
		{
			rx_ring.ex[i].flaglen = 0;
			rx_ring.ex[i].txvlan = 0;
			rx_ring.ex[i].bufhigh = 0;
			rx_ring.ex[i].buflow = 0;
		}
	}
	if (dma)
	{
		DevPhysFree(dma);
	}
}

void NV_DEVICE::drainRing()
{
	drainTX();
	drainRX();
}

void NV_DEVICE::setupHWrings(word rxtx_flags) const
{
	if (desc_ver == DESC_VER_1 || desc_ver == DESC_VER_2) 
	{
		if (rxtx_flags & NV_SETUP_RX_RING) 
		{
			writel(cpu_to_le32(ring_addr), NvRegRxRingPhysAddr);
		}
		if (rxtx_flags & NV_SETUP_TX_RING) 
		{
			writel(cpu_to_le32(ring_addr + rx_ring_size*sizeof(ring_desc)), NvRegTxRingPhysAddr);
		}
	} 
	else 
	{
		if (rxtx_flags & NV_SETUP_RX_RING) 
		{
			writel(cpu_to_le32(ring_addr), NvRegRxRingPhysAddr);
			writel(0, NvRegRxRingPhysAddrHigh);
		}
		if (rxtx_flags & NV_SETUP_TX_RING) 
		{
			writel(cpu_to_le32(ring_addr + rx_ring_size*sizeof(ring_desc_ex)), NvRegTxRingPhysAddr);
			writel(0, NvRegTxRingPhysAddrHigh);
		}
	}
}

void NV_DEVICE::updatePause(dword pause_flags)
{
	pause_flags &= ~(NV_PAUSEFRAME_TX_ENABLE | NV_PAUSEFRAME_RX_ENABLE);

	if (pause_flags & NV_PAUSEFRAME_RX_CAPABLE) 
	{
		dword pff = readl(NvRegPacketFilterFlags) & ~NVREG_PFF_PAUSE_RX;
		if (pause_flags & NV_PAUSEFRAME_RX_ENABLE) 
		{
			writel(pff|NVREG_PFF_PAUSE_RX, NvRegPacketFilterFlags);
			pause_flags |= NV_PAUSEFRAME_RX_ENABLE;
		} 
		else 
		{
			writel(pff, NvRegPacketFilterFlags);
		}
	}
	if (pause_flags & NV_PAUSEFRAME_TX_CAPABLE) 
	{
		dword regmisc = readl(NvRegMisc1) & ~NVREG_MISC1_PAUSE_TX;
		if (pause_flags & NV_PAUSEFRAME_TX_ENABLE) 
		{
			writel(NVREG_TX_PAUSEFRAME_ENABLE,  NvRegTxPauseFrame);
			writel(regmisc|NVREG_MISC1_PAUSE_TX, NvRegMisc1);
			pause_flags |= NV_PAUSEFRAME_TX_ENABLE;
		} 
		else 
		{
			writel(NVREG_TX_PAUSEFRAME_DISABLE,  NvRegTxPauseFrame);
			writel(regmisc, NvRegMisc1);
		}
	}
}

int NV_DEVICE::updateLinkspeed()
{
	dword adv = 0;
	dword lpa = 0;
	dword adv_lpa, adv_pause, lpa_pause;
	dword newls = linkspeed;
	word newdup = duplex;
	long mii_status;
	int retval = 0;
	dword control_1000, status_1000, phyreg, pause_flags, txreg, phy_exp;

mii_status = phyRW(phyaddr, MII_BMSR, MII_READ);
long status2 = phyRW(phyaddr, MII_BMSR, MII_READ);
if (status2 != -1L) mii_status = status2;

	do {
		if (autoneg == 0) 
		{
			if (fixed_mode & LPA_100FULL) 
			{
				newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_100;
				newdup = 1;
			} else 
			if (fixed_mode & LPA_100HALF) 
			{
				newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_100;
				newdup = 0;
			} else 
			if (fixed_mode & LPA_10FULL) 
			{
				newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
				newdup = 1;
			} 
			else 
			{
				newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
				newdup = 0;
			}
			retval = 1;
			break;
		} else
		if (mii_status == -1L || !(mii_status & BMSR_LSTATUS)) 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_100;
			newdup = 0;
			retval = 0;
			break;
		}

		if (!(mii_status & BMSR_ANEGCOMPLETE)) 
		{
			
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
			newdup = 0;
			retval = 0;
			break;
		}

		adv = phyRW(phyaddr, MII_ADVERTISE, MII_READ);
		lpa = phyRW(phyaddr, MII_LPA, MII_READ);

		retval = 1;
		if (gigabit == PHY_GIGABIT) 
		{
			control_1000 = phyRW(phyaddr, MII_CTRL1000, MII_READ);
			status_1000 = phyRW(phyaddr, MII_STAT1000, MII_READ);
	
			if ((control_1000 & ADVERTISE_1000FULL) && (status_1000 & LPA_1000FULL)) 
			{
				newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_1000;
				newdup = 1;
				break;
			}
		}

		adv_lpa = lpa & adv;
		if (adv_lpa & LPA_100FULL) 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_100;
			newdup = 1;
		} else 
		if (adv_lpa & LPA_100HALF) 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_100;
			newdup = 0;
		} else 
		if (adv_lpa & LPA_10FULL) 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
			newdup = 1;
		} else 
		if (adv_lpa & LPA_10HALF) 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
			newdup = 0;
		} 
		else 
		{
			newls = NVREG_LINKSPEED_FORCE|NVREG_LINKSPEED_10;
			newdup = 0;
		}
	} while (0);

	if (newls & NVREG_LINKSPEED_10)
	{
		AdapterSC.MscLinkSpd = 10000000;
	} else
	if (newls & NVREG_LINKSPEED_100)
	{
		AdapterSC.MscLinkSpd = 100000000;
	} else
	if (newls & NVREG_LINKSPEED_1000)
	{
		AdapterSC.MscLinkSpd = 1000000000;
	}
	if (duplex == newdup && linkspeed == newls)
	{
		return retval;
	}

	duplex = newdup;
	linkspeed = newls;

	if (gigabit == PHY_GIGABIT) 
	{
		phyreg = readl(NvRegRandomSeed);
		phyreg &= ~(0x3FF00);
		if ((linkspeed & 0xFFF) == NVREG_LINKSPEED_10)
		{
			phyreg |= NVREG_RNDSEED_FORCE3;
		} else 
		if ((linkspeed & 0xFFF) == NVREG_LINKSPEED_100)
		{
			phyreg |= NVREG_RNDSEED_FORCE2;
		} else 
		if ((linkspeed & 0xFFF) == NVREG_LINKSPEED_1000)
		{
			phyreg |= NVREG_RNDSEED_FORCE;
		}
		writel(phyreg, NvRegRandomSeed);
	}

	phyreg = readl(NvRegPhyInterface);
	phyreg &= ~(PHY_HALF|PHY_100|PHY_1000);
	if (duplex == 0)
	{
		phyreg |= PHY_HALF;
	}
	if ((linkspeed & NVREG_LINKSPEED_MASK) == NVREG_LINKSPEED_100)
	{
		phyreg |= PHY_100;
	} else 
	if ((linkspeed & NVREG_LINKSPEED_MASK) == NVREG_LINKSPEED_1000)
	{
		phyreg |= PHY_1000;
	}
	writel(phyreg, NvRegPhyInterface);

	phy_exp = phyRW(phyaddr, MII_EXPANSION, MII_READ) & EXPANSION_NWAY; 
	if (phyreg & PHY_RGMII) 
	{
		if ((linkspeed & NVREG_LINKSPEED_MASK) == NVREG_LINKSPEED_1000)
		{
			txreg = NVREG_TX_DEFERRAL_RGMII_1000;
		}
		else
		{
			if (!phy_exp && !duplex && (flags & DEV_HAS_COLLISION_FIX))
			{
				if ((linkspeed & NVREG_LINKSPEED_MASK) == NVREG_LINKSPEED_10)
				{
					txreg = NVREG_TX_DEFERRAL_RGMII_STRETCH_10;
				}
				else
				{
					txreg = NVREG_TX_DEFERRAL_RGMII_STRETCH_100;
				}
			}
			else 
			{
				txreg = NVREG_TX_DEFERRAL_RGMII_10_100;
			}
		}
	} 
	else 
	{
		if (!phy_exp && !duplex && (flags & DEV_HAS_COLLISION_FIX))
		{
			txreg = NVREG_TX_DEFERRAL_MII_STRETCH;
		}
		else
		{
			txreg = NVREG_TX_DEFERRAL_DEFAULT;
		}
	}
	writel(txreg, NvRegTxDeferral);

	if (desc_ver == DESC_VER_1) 
	{
		txreg = NVREG_TX_WM_DESC1_DEFAULT;
	} 
	else 
	{
		if ((linkspeed & NVREG_LINKSPEED_MASK) == NVREG_LINKSPEED_1000)
		{
			txreg = NVREG_TX_WM_DESC2_3_1000;
		}
		else
		{
			txreg = NVREG_TX_WM_DESC2_3_DEFAULT;
		}
	}
	writel(txreg, NvRegTxWatermark);

	writel(NVREG_MISC1_FORCE | ( duplex ? 0 : NVREG_MISC1_HD), NvRegMisc1);
	readl(0);
	writel(linkspeed, NvRegLinkSpeed);
	readl(0);

	pause_flags = 0;
	
	if (duplex != 0) 
	{
		if (autoneg && pause_flags & NV_PAUSEFRAME_AUTONEG) 
		{
			adv_pause = adv & (ADVERTISE_PAUSE_CAP| ADVERTISE_PAUSE_ASYM);
			lpa_pause = lpa & (LPA_PAUSE_CAP| LPA_PAUSE_ASYM);

			switch (adv_pause) 
			{
				case ADVERTISE_PAUSE_CAP:
				{
					if (lpa_pause & LPA_PAUSE_CAP) 
					{
						pause_flags |= NV_PAUSEFRAME_RX_ENABLE;
						if (pause_flags & NV_PAUSEFRAME_TX_REQ)
						{
							pause_flags |= NV_PAUSEFRAME_TX_ENABLE;
						}
					}
				} break;
				case ADVERTISE_PAUSE_ASYM:
				{
					if (lpa_pause == (LPA_PAUSE_CAP| LPA_PAUSE_ASYM))
					{
						pause_flags |= NV_PAUSEFRAME_TX_ENABLE;
					}
				} break;
				case ADVERTISE_PAUSE_CAP| ADVERTISE_PAUSE_ASYM:
				{
					if (lpa_pause & LPA_PAUSE_CAP)
					{
						pause_flags |=  NV_PAUSEFRAME_RX_ENABLE;
						if (pause_flags & NV_PAUSEFRAME_TX_REQ)
						{
							pause_flags |= NV_PAUSEFRAME_TX_ENABLE;
						}
					}
					if (lpa_pause == LPA_PAUSE_ASYM)
					{
						pause_flags |= NV_PAUSEFRAME_RX_ENABLE;
					}
				} break;
			}
		} 
		else 
		{
			pause_flags = pause_flags;
		}
	}
	updatePause(pause_flags);

	return retval;
}

void NV_DEVICE::linkChange()
{
	if (updateLinkspeed()) 
	{
		if (!rxstarted)
		{
			startRX();
		}
	} 
	else 
	{
		if (rxstarted)
		{
		}
	}
}

void NV_DEVICE::linkIRQ()
{
	dword miistat = readl(NvRegMIIStatus);
	writel(NVREG_MIISTAT_MASK, NvRegMIIStatus);

	if (miistat & (NVREG_MIISTAT_LINKCHANGE))
	{
		linkChange();
	}
}

#define TX_WORK_PER_LOOP  64
#define RX_WORK_PER_LOOP  64
word NV_DEVICE::IRQHandler()
{
	dword events;
	word i;
	word timer = 0;

	for (i=0; ; i++) 
	{
		if (!(msi_flags & NV_MSI_X_ENABLED)) 
		{
			events = readl(NvRegIrqStatus) & NVREG_IRQSTAT_MASK;
			writel(NVREG_IRQSTAT_MASK, NvRegIrqStatus);
		}
		else 
		{
			events = readl(NvRegMSIXIrqStatus) & NVREG_IRQSTAT_MASK;
			writel(NVREG_IRQSTAT_MASK, NvRegMSIXIrqStatus);
		}
		if (!(events & irqmask))
		{
			break;
		}
		if (events & NVREG_IRQ_TIMER)
		{
			timer++;
			if (timer > 1 && !(events & ~NVREG_IRQ_TIMER))
			{
				continue;
			}
		}

		if (desc_ver == DESC_VER_3)
		{
			dword flaglen = le32_to_cpu(tx_ring.ex[txpos].flaglen);
			if (!(flaglen & NV_TX2_VALID) && (flaglen & LEN_MASK_V2))
			{
				DevRun(txdma);
			}
		}
		else
		{
			dword flaglen = le32_to_cpu(tx_ring.orig[txpos].flaglen);
			if (!(flaglen & NV_TX_VALID) && (flaglen & LEN_MASK_V1))
			{
				DevRun(txdma);
			}
		}

		{
			if (!mutex.request(0))
			{
				rxProcess();
				mutex.release();
			}
		}
		if (events & NVREG_IRQ_LINK) 
		{
			if (!mutex.request(0))
			{
				linkIRQ();
				mutex.release();
			}
		}
		if (need_linktimer && time.diff() >= LINK_TIMEOUT) 
		{
			if (!mutex.request(0))
			{
				linkChange();
				time.reset();
				mutex.release();
			}
		}
		if (events & (NVREG_IRQ_TX_ERR)) 
		{
		}
		if (events & (NVREG_IRQ_UNKNOWN)) 
		{
		}
		if (events & NVREG_IRQ_RECOVER_ERROR) 
		{
			break;
		}
		if (i > max_interrupt_work) 
		{
			break;
		}

	}

	return i;
}

void NV_DEVICE::setMcast()
{
	byte addr[6] = {0};
	byte mask[6] = {0};
	word j;
	dword pff = ((readl(NvRegPacketFilterFlags) & NVREG_PFF_PAUSE_RX) & ~((dword)NVREG_PFF_PROMISC | NVREG_PFF_MYADDR)) | NVREG_PFF_ALWAYS;
	if (wPktMask & FLTR_PRMSCS)	
	{
		pff |= NVREG_PFF_PROMISC;
	} else 
	if (AdapterMCB.McbCnt > 0) 
	{
		pff |= NVREG_PFF_MYADDR;
		byte alwaysOn[6] = {0xFF};
		byte alwaysOff[6] = {0xFF};
		for(word i = 0; i < AdapterMCB.McbCnt; i++)
		{
			for (j = 0; j < 6; j++)
			{
				alwaysOn[j] &= AdapterMCB.McbAddrs[i].mAddr[j];
				alwaysOff[j] &= ~AdapterMCB.McbAddrs[i].mAddr[j];
			}
		}
		for (j = 0; j < 6; j++)
		{
			addr[j] = alwaysOn[j];
			mask[j] = alwaysOn[j] | alwaysOff[j];
		}
	}
	addr[0] |= NVREG_MCASTADDRA_FORCE;
	mutex.request();
	DevCli();
	stopRX();
	for (j = 0; j < 6; j++)
	{
		writeb(addr[j], NvRegMulticastAddrA + j);
	}
	for (j = 0; j < 6; j++)
	{
		writeb(mask[j], NvRegMulticastMaskA + j);
	}
	writel(pff, NvRegPacketFilterFlags);
	startRX();
	mutex.release();
	DevSti();
}

void NV_DEVICE::setMac()
{
	if (rxstarted)
	{
		mutex.request();
		DevCli();
		stopRX();
		for (word i = 0; i < 6; i++)
		{
			writeb(AdapterSC.MscCurrStnAdr[i], NvRegMacAddrA + i);
		}
		startRX();
		mutex.release();
		DevSti();
	}
	else 
	{
		for (word i = 0; i < 6; i++)
		{
			writeb(AdapterSC.MscCurrStnAdr[i], NvRegMacAddrA + i);
		}
	}
}

void NV_DEVICE::getHWStats()
{
	AdapterSS.MssFSByt += readl(NvRegTxCnt);
	AdapterSS.MssSFHW += readl(NvRegTxLossCarrier);
	AdapterSS.MssSFTime += readl(NvRegTxExcessDef);
	AdapterSS.MssRFMin += readl(NvRegRxFrameErr);
	AdapterSS.MssRFMax += readl(NvRegRxFrameTooLong);
	AdapterSS.MssRFLack += readl(NvRegRxOverflow);
	AdapterSS.MssRFCRC += readl(NvRegRxFCSErr);
	AdapterSS.MssRFErr += readl(NvRegRxFrameAlignErr) + readl(NvRegRxLenErr) + readl(NvRegRxLateCol);
	AdapterSS.MssFRMCByt += readl(NvRegRxMulticast);
	AdapterSS.MssFRBCByt += readl(NvRegRxBroadcast);
	AdapterSS.MssFRByt += readl(NvRegRxMulticast) + readl(NvRegRxBroadcast) + readl(NvRegRxUnicast);
}

int NV_DEVICE::startXmit(TxBufDesc far *pDsc)
{

	word fragments = pDsc->TxDataCount;
	word size = pDsc->TxImmedLen;
	word i;
	for(i = 0 ; i < fragments; i++)
	{
		size += pDsc->TxDataBlk[i].TxDataLen;
	}
	if (size < ETH_HEADER_LEN || size > AdapterCC.CcSCp->MscMaxFrame)
	{
		return INVALID_PARAMETER;
	}

	pring_desc p = &(tx_ring.orig[txpos]);
	dword flags = le32_to_cpu(p->flaglen);
	while (flags & NV_TX_VALID)
	{
		word rc = DevBlock(txdma, -1, 0);
		if (rc)
		{
			return OUT_OF_RESOURCE;
		}
		p = &(tx_ring.orig[txpos]);
		flags = le32_to_cpu(p->flaglen);	
	}

	dword phys = cpu_to_le32(txdma + (dword)txpos * NV_TX_ALLOCATE);
	p->buf = phys;
	fpbyte pPtr = tx_virt_buf[txpos];

	if (!pPtr)
	{
		return OUT_OF_RESOURCE;
	}
	
	if (pDsc->TxImmedLen)
	{
		memcpy(pPtr, pDsc->TxImmedPtr, pDsc->TxImmedLen);
		pPtr += pDsc->TxImmedLen;
	}

	for (i = 0 ; i < fragments; i++)
	{
		word ptrtype = pDsc->TxDataBlk[i].TxPtrType;
		word wLen = pDsc->TxDataBlk[i].TxDataLen;
		if (ptrtype == 2)
		{
			memcpy(pPtr, pDsc->TxDataBlk[i].TxDataPtr, wLen);
		}
		else
		{
			fptr pData;
			DevPhysToGDTSelector(dword(pDsc->TxDataBlk[i].TxDataPtr), wLen, GDT_READ);
			SELECTOROF(pData) = GDT_READ;
			OFFSETOF  (pData) = 0;
			memcpy(pPtr, pData, wLen);
		}
		pPtr += pDsc->TxDataBlk[i].TxDataLen;
	}

	dword tx_flags_extra = tx_flags | (desc_ver == DESC_VER_1 ? NV_TX_LASTPACKET : NV_TX2_LASTPACKET);
	p->flaglen = cpu_to_le32((size - 1) | tx_flags_extra);
	writel(NVREG_TXRXCTL_KICK|txrxctl_bits, NvRegTxRxControl);

	txpos++;
	if (txpos == tx_ring_size)
	{
		txpos = 0;
	}

	return SUCCESS;
}

long NV_DEVICE::getlen(fpbyte packet, long datalen) const
{
	word hdrlen;	
	word protolen;	

	{
		protolen = ntohs(((struct ethhdr far *)packet)->h_proto);
		hdrlen = ETH_HLEN;
	}
	if (protolen > ETH_DATA_LEN)
	{
		return datalen; 
	}

	protolen += hdrlen;
	
	if (datalen > ETH_ZLEN) 
	{
		if (datalen >= protolen) 
		{
			return protolen;
		} 
		else 
		{
			return -1;
		}
	} 
	else 
	{
		
		if (protolen > ETH_ZLEN) 
		{
			return -1;
		}
		return datalen;
	}
}

int NV_DEVICE::rxProcess()
{
	dword flags;
	long len;
	word release;

	int i;
	for (i = 0; !((flags = le32_to_cpu(rx_ring.orig[rxpos].flaglen)) & NV_RX_AVAIL) && i < rx_ring_size; rxpos = ((rxpos + 1 >= rx_ring_size) ? 0 : rxpos + 1), i++)
	{
		fpbyte data = rx_virt_buf[rxpos];
		if (!data)
		{
			return -1;
		}
		
		do 
		{
			release = 1;
			
			if (desc_ver == DESC_VER_1) 
			{
				if (flags & NV_RX_DESCRIPTORVALID) 
				{
					len = flags & LEN_MASK_V1;
					if (flags & NV_RX_ERROR) 
					{
						if (flags & NV_RX_ERROR4) 
						{
							len = getlen(data, len);
							if (len < 0) 
							{
								break;
							}
						} else
						
						if (flags & NV_RX_FRAMINGERR) 
						{
							if (flags & NV_RX_SUBSTRACT1) 
							{
								len--;
							}
						}
						
						else 
						{
							break;
						}
					}
				} 
				else 
				{
					break;
				}
			} 
			else  
			{
				if (flags & NV_RX2_DESCRIPTORVALID) 
				{
					len = flags & LEN_MASK_V2;
					if (flags & NV_RX2_ERROR) 
					{
						if (flags & NV_RX2_ERROR4) 
						{
							len = getlen(data, len);
							if (len < 0) 
							{
								break;
							}
						} else
						
						if (flags & NV_RX2_FRAMINGERR) 
						{
							if (flags & NV_RX2_SUBSTRACT1) 
							{
								len--;
							}
						}
						
						else 
						{
							break;
						}
					}
				} 
				else 
				{
					break;
				}
			}
			
			if (len > 0)
			{			
				if (wIndications)
				{
					return i;
				}
				indOff();
				data[NV_RX_ALLOCATE - 1] = 0xFF;

				if (dorxchain)
				{
					RxBufOne far * rxbuf = (RxBufOne far *)(data + NV_RX_ALLOCATE - 1 - sizeof(RxBufOne));
					rxbuf->RxDataCount = 1;
					rxbuf->RxDataBlk.RxDataLen = (word)len;
					rxbuf->RxDataBlk.RxDataPtr = (fpbyte)data;
					data[NV_RX_ALLOCATE - 1] = 0xFF;
					word rc = ProtDT.PldRcvChain(AdapterCC.CcModuleID, (word)len, rxpos, (fpbyte)rxbuf, (fpbyte)(data + NV_RX_ALLOCATE - 1), ProtCC.CcDataSeg);
					if (rc == 1)
					{	
						release = 0;
					}
				}
				else
				{

					currentrxbuffer = data;
					currentlen = (word)len;
					ProtDT.PldRcvLkAhead(AdapterCC.CcModuleID, currentlen, currentlen, currentrxbuffer, (fpbyte)(data + NV_RX_ALLOCATE - 1), ProtCC.CcDataSeg);
					currentrxbuffer = NULL;
					currentlen = 0;

				}

				if (data[NV_RX_ALLOCATE - 1])
				{
					indOn();
				}
				ProtDT.PldIndComplete(AdapterCC.CcModuleID, ProtCC.CcDataSeg);
			}
		} while (0);
		if (release)
		{
			rx_ring.orig[rxpos].flaglen = cpu_to_le32(rx_buf_sz | NV_RX_AVAIL);
		}
	}

	return i;
}

int NV_DEVICE::releaseRX(word pos)
{
	if (pos < rx_ring_size)
	{
		rx_ring.orig[pos].flaglen = cpu_to_le32(rx_buf_sz | NV_RX_AVAIL);
		return SUCCESS;
	}
	return INVALID_PARAMETER;
}

int NV_DEVICE::xferRX(fpword pcopied, word frameOffset, TDBufDesc far * pd)
{
	if (!currentlen || !currentrxbuffer)
	{
		return GENERAL_FAILURE;
	}
	if (currentlen < frameOffset)
	{
		return INVALID_PARAMETER;
	} else
	if (currentlen == frameOffset)
	{
		return SUCCESS;
	}
	
	fpbyte start = currentrxbuffer + frameOffset;
	currentlen -= frameOffset;

	word copied = 0;
	for (word i = 0; i < pd->TDDataCount && currentlen > 0; i++)
	{
		word ptrtype = pd->TDDataBlk[i].TDPtrType;
		word bufsize = pd->TDDataBlk[i].TDDataLen;
		word tocopy = min((word)currentlen, (word)bufsize);
		if (ptrtype == 2)
		{
			memcpy(pd->TDDataBlk[i].TDDataPtr, start, tocopy);
		}
		else
		{
			fptr pData;
			DevPhysToGDTSelector(dword(pd->TDDataBlk[i].TDDataPtr), bufsize, GDT_WRITE);
			SELECTOROF(pData) = GDT_WRITE;
			OFFSETOF  (pData) = 0;
			memcpy(pData, start, tocopy);
		}
		start += tocopy;
		copied += tocopy;
		currentlen -= tocopy;
	}
	*pcopied = copied;
	return SUCCESS;
}

