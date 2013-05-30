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
              .seq

_HEADER       segment word public use16 'DATA'
_HEADER       ends

CONST         segment word public use16 'DATA'
CONST         ends

CONST2        segment word public use16 'DATA'
CONST2        ends

_DATA         segment word public use16 'DATA'
_DATA         ends

_BSS          segment word public use16 'BSS'
_BSS          ends

_ENDDS        segment word public use16 'ENDDS'
_ENDDS        ends

_INITDATA     segment word public use16 'INITDATA'
_INITDATA     ends

_TEXT         segment word public use16 'CODE'
_TEXT         ends

; resource manager code
RMCODE        segment dword public use16 'CODE'
RMCODE        ends

_ENDCS        segment word public use16 'CODE'
_ENDCS        ends

_INITTEXT     segment word public use16 'CODE'
_INITTEXT     ends

DGROUP        group _HEADER, CONST, CONST2, _DATA, _BSS, _ENDDS, _INITDATA
CGROUP        group _TEXT, RMCODE, _ENDCS, _INITTEXT

;; DevHelp support routines and data

_DATA         segment word public use16 'DATA'

              extrn DOSIODELAYCNT    :abs 

              public _Device_Help
              public _OemHlpProc
              public _OemHlpDS
              public _Dev_StdEntry
              public _Dev_DbgEntry
              public _Delay500
              public __wcpp_2_pure_error__
              public __wcpp_2_undef_vfun__
              public __wcpp_4_undefed_cdtor__
              public __wcpp_4_lcl_register__
              public ___wcpp_4_data_module_dtor_ref_

_DATA         ends

_TEXT         segment word public use16 'CODE'
              assume cs:CGROUP, ds:DGROUP, es:NOTHING, ss:NOTHING

Entry:
__wcpp_2_pure_error__:
__wcpp_2_undef_vfun__:
__wcpp_4_undefed_cdtor__:
__wcpp_4_lcl_register__:
___wcpp_4_data_module_dtor_ref_:
DevHelpDebug  proc far
              int 3
              call dword ptr _Dev_StdEntry
              int 3
              ret
DevHelpDebug  endp

_TEXT         ends

_DATA         segment word public use16 'DATA'

_Device_Help    dd DevHelpDebug
_Dev_StdEntry dd DevHelpDebug
_Dev_DbgEntry dd DevHelpDebug
_OemHlpProc   dd 0
_OemHlpDS     dw 0
_Delay500     dw DOSIODELAYCNT

_DATA         ends

_ENDDS        segment word public use16 'ENDDS'

              public ___OffFinalDS

___OffFinalDS db 0

_ENDDS        ends

_ENDCS        segment word public use16 'CODE'

              public ___OffFinalCS

___OffFinalCS proc near
              int 3
___OffFinalCS endp

_ENDCS        ends

              end      Entry

