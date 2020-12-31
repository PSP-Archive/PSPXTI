/*
 *  xtiger port on PSP 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

 /*
  * UAE - The Un*x Amiga Emulator
  *
  * cpp magic
  *
  * Copyright 1995, 1996 Bernd Schmidt, Ed Hanway
  */
# ifndef _OPTIONS_H_
# define _OPTIONS_H_

#include <string.h>

#define CPU_EMU_SIZE 0
#define IN_UNIX
#define REGPARAM

#define fast_memcmp memcmp
#define memcmpy generic_memcmpy

static __inline__ int generic_memcmpy(void *foo, const void *bar, int len)
{
    int res = memcmp(foo, bar, len);
    if (res)
	memcpy(foo, bar, len);
    return res;
}



# endif
