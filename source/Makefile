VERSION=0.0.6
BUILDDATE=20100204

# To build this driver you must have installed :
#    Watcom 11c or OpenWatcom installed,
#    GNU make (i use 3.81beta1)
#    MAKEDEP V4.0d
#    some misc tools, like rm, cp, zip - not nessessary but making will be smooth
#    DDK (for RM stuff, if you have no DDK, just delete rm.cpp from your Makefile and do some dummy defines in rm.h
#
# %PATH%, %BEGINLIBPATH%, %WATCOM%, %INCLUDE%, %LIB%, %DDK% must include 
# correponding paths to your WATCOM and DDK (for RM) installation
# the other tools mentioned must be in your %PATH%



VPATH = .

.SUFFIXES: .lst .obj .c .cpp .asm .lib .def

TARGET=nveth


CC	= wpp -i=. -bt=os2 -e60 -5 -omrli -r -s -wx -xd -zp=1 -ze -zdp -zff -zgf -zl -zq -zu -zm -DDFILE="$(TARGET).os2" -DDNAME="NVIDIA MAC Driver" -DDVENDOR="nickk" -DDDATE=${BUILDDATE}L  -DDEBUG=0

AFLAGS  = -q
ASM     = WASM $(AFLAGS)

LFLAGS  = system os2 physdevice option int, map, eliminate, mang sort global
LINK    = WLINK $(LFLAGS)

.def.lib:
	implib $*.lib $*.def

.c.obj:
	$(CC) $*.c

.cpp.obj:
	$(CC) $*.cpp

.asm.obj:
	$(ASM) $*.asm

OBJS = \
	adevhdr.obj \
	driver.obj \
	strat.obj \
	mac.obj \
	ndis.obj \
	oem.obj \
	nvidia.obj \
	rm.obj \
        devseg.obj

SRC = \
	adevhdr.asm \
	driver.cpp \
	strat.cpp \
	mac.cpp \
	ndis.cpp \
	oem.cpp \
	nvidia.cpp \
	rm.cpp \
	devseg.asm

SRCDIST = \
	$(SRC) \
	base.h \
	devhelp.h \
	devrp.h \
	driver.h \
	mii.h \
	ndis.h \
	nvidia.h \
	oem.h \
	pci.h \
	sas.h \
	rm.h \
	makenif.pl \
	impdos.def

all: $(TARGET).rar $(TARGET)-$(VERSION).zip deldepend

deldepend:
	@del .depend

.depend: Makefile $(SRC)
	makedep -f$@ -s -i.  $(SRC)

$(TARGET).lnk: Makefile 
	@echo name $(TARGET).os2> $@
	@for %f in ($(OBJS)) do @echo file %f>> $@
	@echo SEGMENT TYPE DATA SHARED>> $@
	@echo option DESCRIPTION '@#NICKK:$(VERSION)#@NVETH'>> $@

version.h: Makefile
	@echo ^#undef VERSION> version.h
	@echo ^#define VERSION	" $(VERSION) " >> version.h

impdos.lib: impdos.def

$(TARGET).os2: version.h $(TARGET).lnk $(OBJS) Makefile impdos.lib
	$(LINK) library impdos.lib library $(DDK)\BASE\lib\rmcalls.lib @$(TARGET).lnk

$(TARGET).sym: $(TARGET).os2
	wat2map.cmd $(TARGET).map $(TARGET).ma1
	mapsym $(TARGET).ma1
	@rm -f $(TARGET).ma1

$(TARGET).rar:	$(TARGET).os2 $(TARGET).sym $(TARGET).nif readme
	@rm -f $(TARGET).rar
	rar32 a  $(TARGET).rar $(TARGET).os2 $(TARGET).sym $(TARGET).nif readme

$(TARGET).nif: Makefile
	perl makenif.pl $(VERSION) $(TARGET).nif

$(TARGET)-$(VERSION).zip:	$(TARGET).os2 $(TARGET).sym $(TARGET).nif readme src.zip LICENSE
	@rm -f $(TARGET)-$(VERSION).zip
	zip -9  $(TARGET)-$(VERSION).zip $(TARGET).os2 $(TARGET).sym $(TARGET).nif readme src.zip LICENSE

src.zip:	Makefile $(SRCDIST)
	@rm -f src.zip
	@cp nveth.nif readme ./src
	cd src && zip -9 ..\src.zip *	

clean:
	@rm -f ./src/*
	@rm -f *.obj
	@rm -f *.map
	@rm -f *.lnk
	@rm -f *.lst
	@rm -f *.lib
	@rm -f *.err
	@rm -f *.sym
	@rm -f src.zip
	@rm -f $(TARGET)-$(VERSION).zip
	@rm -f $(TARGET).os2
	@del .depend

include .depend
