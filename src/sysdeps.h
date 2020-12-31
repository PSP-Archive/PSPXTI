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

# ifndef _SYS_DEPS_H
# define _SYS_DEPS_H
/* Set ULONG, LONG, UWORD, WORD, UBYTE & BYTE
 * 
 */

#define SIZEOF_SHORT 2
#define SIZEOF_INT 4
#define SIZEOF_LONG 4

/* If char has more then 8 bits, good night. */
typedef unsigned char UBYTE;
typedef signed char BYTE;

#if SIZEOF_SHORT == 2
typedef unsigned short UWORD;
typedef short WORD;
#elif SIZEOF_INT == 2
typedef unsigned int UWORD;
typedef int WORD;
#else
#error No 2 byte type, you lose.
#endif

#if SIZEOF_INT == 4
typedef unsigned int ULONG;
typedef int LONG;
#elif SIZEOF_LONG == 4
typedef unsigned long ULONG;
typedef long LONG;
#else
#error No 4 byte type, you lose.
#endif
typedef ULONG CPTR;



# endif
