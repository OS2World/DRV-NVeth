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
#ifndef __BASE_H
#define __BASE_H

#define HANDLE(a)       ((dword)(void far *)&(a))

#ifndef min
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

typedef unsigned short  word;
typedef unsigned long   dword;
typedef unsigned char   byte;
typedef unsigned size_t;

typedef byte far * fpbyte;
typedef char far * fpchar;
typedef void far * fptr;
typedef dword far * fpdword;
typedef word far * fpword;

#define ntohs(x)   (((word)x<<8)|((word)x>>8))

extern "C" {
extern void far *_fmemchr( const void far *s, int c, size_t n );
extern int         _fmemcmp( const void far *s1, const void far *s2, size_t n );
extern void far *_fmemcpy( void far *s1, const void far *s2, size_t n );
extern void far *_fmemset( void far *s, int c, size_t n );

extern char far *_fstrcat( char far *s1, const char far *s2 );
extern int         _fstrcmp( const char far *s1, const char far *s2 );
extern char far *_fstrcpy( char far *s1, const char far *s2 );
extern size_t      _fstrlen( const char far *s );

extern int stricmp(char far *str0, char far *str1);
extern int aton(char far * str);
}

#pragma intrinsic(_fmemchr,_fmemcmp,_fmemcpy,_fmemset)
#pragma intrinsic(_fstrcat,_fstrcmp,_fstrcpy,_fstrlen)

#define memchr _fmemchr
#define memcmp _fmemcmp
#define memcpy _fmemcpy
#define memset _fmemset
#define strcat _fstrcat
#define strcmp _fstrcmp
#define strcpy _fstrcpy
#define strlen _fstrlen

#define ucase(a)    (((a) >= 'a' && (a) <= 'z') ? ((a) -= ('a'-'A')) : (a))

#endif  

