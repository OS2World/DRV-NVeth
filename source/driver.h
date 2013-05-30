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
#include <ndis.h>
#include <devrp.h>
#include <devhelp.h>
#include <sas.h>

#ifndef __DRIVER_H
#define __DRIVER_H

#define COPYRIGHT       "Copyright (C) 2007 "
#define CRLF            "\r\n"

#define DEV_NAME                "NVETH$"
#define OEMHLP_NAME              "OEMHLP$ "

#define ST_CLOSED               1
#define ST_OPENED           	2

extern word sleephandle;

extern char cDevName[MAX_DEVNAME + 1];

extern word wIndications;
extern word wPktMask;

extern word wState;
extern char* cRCStr[];

extern CommChar    ProtCC;
extern ProtLwrDisp ProtDT;

extern CommChar   AdapterCC;
extern MACUprDisp AdapterDT;

extern MCastBuf AdapterMCB;

word ndiscall SysReq(dword, dword, word, word, word);

word ndiscall AdapterGenReq    (word, word, word, dword, word, word);
word ndiscall AdapterXmitChain (word, word, fpbyte, word);
word ndiscall AdapterXferData  (fpbyte, word, fpbyte, word);
word ndiscall AdapterRcvRelease(word, word);
word ndiscall AdapterIndOn     (word);
word ndiscall AdapterIndOff    (word);

void indOn(void);
void indOff(void);

extern MACSpecChar AdapterSC;
extern MACSpecStat AdapterSS;
extern MAC8023Stat MACSS;

void Say(char far*);
void FixFarPointers(void);

int NDISInit(void);

dword allocPhysAddress(dword size);
dword mapPhysAddress(dword address, dword length);
dword mapPhysAddress(word gdt, dword address, dword length);

void msleep(dword msecs);
void udelay(word usecs);

void far irqhandler();

void loadsas(void);

class Time
{
private:
	struct InfoSegGDT far * finfogdt;
	dword lastsec;
	dword lastms;
public:
	Time() : finfogdt(NULL), lastms(0), lastsec(0) {}
	~Time() {}
	void init();
	dword getsec() const { return finfogdt == NULL ? 0 : finfogdt->SIS_BigTime; }
	dword getms() const { return finfogdt == NULL ? 0 : finfogdt->SIS_MsCount; }
	void reset() { lastms = getms(); }
	dword diff();
};

extern Time time;

class Mutex
{
private:
	word handle;
public:
	Mutex() : handle(0) {}
	~Mutex() { release(); }
	word request(word block = 1);
	void release();
};

#define cpu_to_le32(a) (a)
#define le32_to_cpu(a) (a)

#endif  

#include <oem.h>
#include <nvidia.h>
#include <rm.h>

