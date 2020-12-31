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

#ifndef _NOASM_H_
#define _NOASM_H_

#define CLEARVC   regflags.quickclear.vc=0;
#define CLEARFLGS regflags.longflags=0;

#define CTRUE 0x01
#define VTRUE 0x01
#define ZTRUE 0x01
#define NTRUE 0x01
#define XTRUE 0x01

extern void test_flags_b(BYTE src);
extern void test_flags_w(WORD src);
extern void test_flags_l(LONG src);
extern BYTE add_flags_b(BYTE src, BYTE dst);
extern WORD add_flags_w(WORD src, WORD dst);
extern LONG add_flags_l(LONG src, LONG dst);

extern ULONG sub_flags_b(BYTE src, BYTE dst);
extern ULONG sub_flags_w(WORD src, WORD dst);
extern ULONG sub_flags_l(LONG src, LONG dst);

extern ULONG sub2_flags_b(BYTE src, BYTE dst);
extern ULONG sub2_flags_w(WORD src, WORD dst);
extern ULONG sub2_flags_l(LONG src, LONG dst);

#endif
