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
#include "sysdeps.h"
#include "config.h"
#include "options.h"
#include "memory.h"
#include "newcpu.h"
#include "cputbl.h"
#include "noasm.h"
void op_1000(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_1010(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_1018(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_1020(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_1028(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_1030(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_1038(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_1039(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_103a(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_103b(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_103c(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_1080(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1090(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1098(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10a0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10a8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_10b0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10b8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_10b9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_10ba(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10bb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10bc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_10c0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10d0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10d8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_10e0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_10e8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10f0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_10f8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10f9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_10fa(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_10fb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_10fc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	CPTR dsta = regs.a[dstreg];
{	regs.a[dstreg] += areg_byteinc[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1100(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1110(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1118(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_1120(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_1128(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1130(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_1138(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1139(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_113a(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_113b(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}}
void op_113c(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1140(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1150(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1158(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1160(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1168(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1170(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_1178(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1179(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_117a(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_117b(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_117c(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1180(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1190(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_1198(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11a0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11a8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11b0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11b8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11b9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11ba(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11bb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11bc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11c0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11d0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11d8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11e0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11e8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11f0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11f8(ULONG opcode)
{
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11f9(ULONG opcode)
{
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_11fa(ULONG opcode)
{
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11fb(ULONG opcode)
{
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_11fc(ULONG opcode)
{
{{	BYTE src = nextiword();
{	CPTR dsta = (LONG)(WORD)nextiword();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13c0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13d0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13d8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_13e0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_13e8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13f0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_13f8(ULONG opcode)
{
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13f9(ULONG opcode)
{
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_13fa(ULONG opcode)
{
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_13fb(ULONG opcode)
{
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_13fc(ULONG opcode)
{
{{	BYTE src = nextiword();
{	CPTR dsta = nextilong();
test_flags_b(src);
	put_byte(dsta,src);
}}}}
