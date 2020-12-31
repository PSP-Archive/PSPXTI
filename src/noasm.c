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

#include <stdlib.h>
#include <stdio.h>
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "memory.h"
#include "newcpu.h"
#include "noasm.h"
#include "cputbl.h"


void
test_flags_b(BYTE src)
{
# if 0
	__asm__("testb %0,%0; lahf; movzbl %%ah,%%eax; movl intel_flag_lookup(,%%eax,4),%%eax; movl %%eax,regflags": : "r" (src) : "%eax", "cc");
  {
    union flagu tmp_regflags = regflags;
# endif
    CLEARFLGS;
    if ( src <= 0 )
    {
      if ( src ==  0 )
      {
        ZFLG = ZTRUE;
      }
      else
      {
        NFLG = NTRUE;
      }
    }
# if 0
    if (tmp_regflags.longflags != regflags.longflags) {
      fprintf( stdout,"ERROR %s:%d  %ld %ld\n", __FILE__, __LINE__, tmp_regflags.longflags, regflags.longflags);
    }
  }
# endif
}

void
test_flags_w(WORD src)
{
# if 0
	__asm__("testw %0,%0; lahf; movzbl %%ah,%%eax; movl intel_flag_lookup(,%%eax,4),%%eax; movl %%eax,regflags": : "r" (src) : "%eax", "cc");
  {
    union flagu tmp_regflags = regflags;
# endif
    CLEARFLGS;
    if ( src <= 0 )
    {
      if ( src ==  0 )
      {
        ZFLG = ZTRUE;
      }
      else
      {
        NFLG = NTRUE;
      }
    }
# if 0
    if (tmp_regflags.longflags != regflags.longflags) {
      fprintf( stdout,"ERROR %s:%d  %ld %ld\n", __FILE__, __LINE__, tmp_regflags.longflags, regflags.longflags);
    }
  }
# endif
}

void
test_flags_l(LONG src)
{
# if 0
	__asm__("testl %0,%0; lahf; movzbl %%ah,%%eax; movl intel_flag_lookup(,%%eax,4),%%eax; movl %%eax,regflags": : "r" (src) : "%eax", "cc");	
  {
    union flagu tmp_regflags = regflags;
# endif
    CLEARFLGS;
    if ( src <= 0 )
    {
      if ( src ==  0 )
      {
        ZFLG = ZTRUE;
      }
      else
      {
        NFLG = NTRUE;
      }
    }
# if 0
    if (tmp_regflags.longflags != regflags.longflags) {
      fprintf( stdout,"ERROR %s:%d  %lx %lx\n", __FILE__, __LINE__, tmp_regflags.longflags, regflags.longflags);
      fprintf( stdout, "asm: v=%x c=%x n=%x z=%x\n", tmp_regflags.flags.v, tmp_regflags.flags.c, tmp_regflags.flags.n, tmp_regflags.flags.z);
      fprintf( stdout, "c  : v=%x c=%x n=%x z=%x\n", regflags.flags.v, regflags.flags.c, regflags.flags.n, regflags.flags.z);
    }
  }
# endif
}

BYTE
add_flags_b(BYTE src, BYTE dst)
{
	BYTE newv = ((BYTE)(dst)) + ((BYTE)(src));
  int flgs = ((BYTE)(src)) < 0;
	int flgo = ((BYTE)(dst)) < 0;
	int flgn = ((BYTE)(newv)) < 0;
	NFLG = flgn != 0;
	ZFLG = ((BYTE)(newv)) == 0;
	VFLG = (flgs == flgo) && (flgn != flgo);
	CFLG = regs.x = ((UBYTE)(~dst)) < ((UBYTE)(src));
  return newv;
}

WORD
add_flags_w(WORD src, WORD dst)
{
	WORD newv = ((WORD)(dst)) + ((WORD)(src));
  int flgs = ((WORD)(src)) < 0;
	int flgo = ((WORD)(dst)) < 0;
	int flgn = ((WORD)(newv)) < 0;
	NFLG = flgn != 0;
	ZFLG = ((WORD)(newv)) == 0;
	VFLG = (flgs == flgo) && (flgn != flgo);
	CFLG = regs.x = ((UWORD)(~dst)) < ((UWORD)(src));
  return newv;
}

LONG
add_flags_l(LONG src, LONG dst)
{
	LONG newv = ((LONG)(dst)) + ((LONG)(src));
  int flgs = ((LONG)(src)) < 0;
	int flgo = ((LONG)(dst)) < 0;
	int flgn = ((LONG)(newv)) < 0;
	NFLG = flgn != 0;
	ZFLG = ((LONG)(newv)) == 0;
	VFLG = (flgs == flgo) && (flgn != flgo);
	CFLG = regs.x = ((ULONG)(~dst)) < ((ULONG)(src));
  return newv;
}


ULONG
sub_flags_b(BYTE src, BYTE dst)
{
	ULONG newv = ((BYTE)(dst)) - ((BYTE)(src));
 	int flgs = ((BYTE)(src)) < 0;
	int flgo = ((BYTE)(dst)) < 0;
	int flgn = ((BYTE)(newv)) < 0;
	NFLG = flgn != 0;
	ZFLG = ((BYTE)(newv)) == 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = regs.x = ((UBYTE)(src)) > ((UBYTE)(dst));
  return newv;
}

ULONG
sub_flags_w(WORD src, WORD dst)
{
	ULONG newv = ((WORD)(dst)) - ((WORD)(src));
 	int flgs = ((WORD)(src)) < 0;
	int flgo = ((WORD)(dst)) < 0;
	int flgn = ((WORD)(newv)) < 0;
	ZFLG = ((WORD)(newv)) == 0;
	NFLG = flgn != 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = regs.x = ((UWORD)(src)) > ((UWORD)(dst));
  return newv;
}

ULONG
sub_flags_l(LONG src, LONG dst)
{
	ULONG newv = ((LONG)(dst)) - ((LONG)(src));
 	int flgs = ((LONG)(src)) < 0;
	int flgo = ((LONG)(dst)) < 0;
	int flgn = ((LONG)(newv)) < 0;
	ZFLG = ((LONG)(newv)) == 0;
	NFLG = flgn != 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = regs.x = ((ULONG)(src)) > ((ULONG)(dst));
  return newv;
}

ULONG
sub2_flags_b(BYTE src, BYTE dst)
{
	ULONG newv = ((BYTE)(dst)) - ((BYTE)(src));
 	int flgs = ((BYTE)(src)) < 0;
	int flgo = ((BYTE)(dst)) < 0;
	int flgn = ((BYTE)(newv)) < 0;
	ZFLG = ((BYTE)(newv)) == 0;
	NFLG = flgn != 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = ((UBYTE)(src)) > ((UBYTE)(dst));
  return newv;
}

ULONG
sub2_flags_w(WORD src, WORD dst)
{
	ULONG newv = ((WORD)(dst)) - ((WORD)(src));
 	int flgs = ((WORD)(src)) < 0;
	int flgo = ((WORD)(dst)) < 0;
	int flgn = ((WORD)(newv)) < 0;
	ZFLG = ((WORD)(newv)) == 0;
	NFLG = flgn != 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = ((UWORD)(src)) > ((UWORD)(dst));
  return newv;
}

ULONG
sub2_flags_l(LONG src, LONG dst)
{
	ULONG newv = ((LONG)(dst)) - ((LONG)(src));
 	int flgs = ((LONG)(src)) < 0;
	int flgo = ((LONG)(dst)) < 0;
	int flgn = ((LONG)(newv)) < 0;
	ZFLG = ((LONG)(newv)) == 0;
	NFLG = flgn != 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	CFLG = ((ULONG)(src)) > ((ULONG)(dst));
  return newv;
}
