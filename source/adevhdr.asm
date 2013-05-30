;/*
; * This source is the part of nveth - NVIDIA ndis driver for OS/2
; *
; * Copyright (C) 2007 nickk
; *
; * This program is free software; you can redistribute it and/or modify
; * it under the terms of the GNU General Public License as published by
; * the Free Software Foundation; either version 2 of the License, or
; * (at your option) any later version.
; *
; * This program is distributed in the hope that it will be useful,
; * but WITHOUT ANY WARRANTY; without even the implied warranty of
; * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; * GNU General Public License for more details.
; *
; * You should have received a copy of the GNU General Public License
; * along with this program; if not, write to the Free Software
; * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
; */
              .386p

_HEADER       segment word public use16 'DATA'

              extrn Strategy_ : near

              public _DevName
              public _DevProtCS
              public _DevProtDS
              public _DevRealCS
              public _DevRealDS

DEV_IOCTL       EQU     4000H
DEV_CHAR_DEV    EQU     8000H
DEVLEV_3        EQU     0180H
DEV_30      	EQU     0800H

              dd 0FFFFFFFFh               ;; Link to next header
              dw DEV_IOCTL+DEV_CHAR_DEV+DEVLEV_3+DEV_30
              dw Strategy_                ;; Strategy entry point
              dw 0                        ;; IDC entry point
_DevName      db '        '               ;; Device driver name
_DevProtCS    dw 0                        ;; Protected mode CS
_DevProtDS    dw 0                        ;; Protected mode DS
_DevRealCS    dw 0                        ;; Real mode CS (not used)
_DevRealDS    dw 0                        ;; Real mode DS (not used)

              dd 00000011h                ;; Capabilities bit strip

_HEADER       ends

              end

