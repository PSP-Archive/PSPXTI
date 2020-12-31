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
void op_8000(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_8010(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_8018(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[srcreg] += areg_byteinc[srcreg];
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_8020(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_8028(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_8030(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_8038(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_8039(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_803a(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_803b(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	BYTE src = get_byte(srca);
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}}
void op_803c(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = nextiword();
{	BYTE dst = regs.d[dstreg];
	src |= dst;
test_flags_b(src);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (src) & 0xff;
}}}}
void op_8040(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = regs.d[srcreg];
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_8050(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_8058(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	regs.a[srcreg] += 2;
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}}
void op_8060(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= 2;
{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}}
void op_8068(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_8070(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}}
void op_8078(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_8079(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_807a(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}}
void op_807b(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}}
void op_807c(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = nextiword();
{	WORD dst = regs.d[dstreg];
	src |= dst;
test_flags_w(src);
	regs.d[dstreg] &= ~0xffff; regs.d[dstreg] |= (src) & 0xffff;
}}}}
void op_8080(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	LONG src = regs.d[srcreg];
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_8090(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_8098(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	LONG src = get_long(srca);
{	regs.a[srcreg] += 4;
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}}
void op_80a0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= 4;
{	CPTR srca = regs.a[srcreg];
	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}}
void op_80a8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_80b0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}}
void op_80b8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_80b9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_80ba(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}}
void op_80bb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	LONG src = get_long(srca);
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}}
void op_80bc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	LONG src = nextilong();
{	LONG dst = regs.d[dstreg];
	src |= dst;
test_flags_l(src);
	regs.d[dstreg] = (src);
}}}}
void op_80c0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = regs.d[srcreg];
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_80d0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_80d8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	regs.a[srcreg] += 2;
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_80e0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= 2;
{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_80e8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_80f0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_80f8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_80f9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_80fa(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_80fb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_80fc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = nextiword();
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	ULONG newv = (ULONG)dst / (UWORD)src;
	ULONG rem = (ULONG)dst % (UWORD)src;
	if (newv > 0xffff) { VFLG = NFLG = 1; } else
	{
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_8100(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	BYTE src = regs.d[srcreg];
{	BYTE dst = regs.d[dstreg];
{	UWORD newv_lo = (dst & 0xF) - (src & 0xF) - regs.x;
	UWORD newv_hi = (dst & 0xF0) - (src & 0xF0);
	UWORD newv;
	if (newv_lo > 9) { newv_lo-=6; newv_hi-=0x10; }
	newv = newv_hi + (newv_lo & 0xF);	CFLG = regs.x = (newv_hi & 0x1F0) > 0x90;
	if (CFLG) newv -= 0x60;
	if (((BYTE)(newv)) != 0) ZFLG = 0;
	NFLG = ((BYTE)(newv)) < 0;
{	int flgs = ((BYTE)(src)) < 0;
	int flgo = ((BYTE)(dst)) < 0;
	int flgn = ((BYTE)(newv)) < 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	regs.d[dstreg] &= ~0xff; regs.d[dstreg] |= (newv) & 0xff;
}}}}}}
void op_8108(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= areg_byteinc[srcreg];
{	CPTR srca = regs.a[srcreg];
	BYTE src = get_byte(srca);
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
	BYTE dst = get_byte(dsta);
{	UWORD newv_lo = (dst & 0xF) - (src & 0xF) - regs.x;
	UWORD newv_hi = (dst & 0xF0) - (src & 0xF0);
	UWORD newv;
	if (newv_lo > 9) { newv_lo-=6; newv_hi-=0x10; }
	newv = newv_hi + (newv_lo & 0xF);	CFLG = regs.x = (newv_hi & 0x1F0) > 0x90;
	if (CFLG) newv -= 0x60;
	if (((BYTE)(newv)) != 0) ZFLG = 0;
	NFLG = ((BYTE)(newv)) < 0;
{	int flgs = ((BYTE)(src)) < 0;
	int flgo = ((BYTE)(dst)) < 0;
	int flgn = ((BYTE)(newv)) < 0;
	VFLG = (flgs != flgo) && (flgn != flgo);
	put_byte(dsta,newv);
}}}}}}}}
void op_8110(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_8118(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	BYTE dst = get_byte(dsta);
{	regs.a[dstreg] += areg_byteinc[dstreg];
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_8120(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	BYTE src = regs.d[srcreg];
{	regs.a[dstreg] -= areg_byteinc[dstreg];
{	CPTR dsta = regs.a[dstreg];
	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_8128(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_8130(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
{	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}}
void op_8138(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = (LONG)(WORD)nextiword();
	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_8139(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	BYTE src = regs.d[srcreg];
{	CPTR dsta = nextilong();
	BYTE dst = get_byte(dsta);
	src |= dst;
test_flags_b(src);
	put_byte(dsta,src);
}}}}
void op_8150(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}
void op_8158(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	WORD dst = get_word(dsta);
{	regs.a[dstreg] += 2;
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}}
void op_8160(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	WORD src = regs.d[srcreg];
{	regs.a[dstreg] -= 2;
{	CPTR dsta = regs.a[dstreg];
	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}}
void op_8168(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}
void op_8170(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
{	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}}
void op_8178(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = (LONG)(WORD)nextiword();
	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}
void op_8179(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	WORD src = regs.d[srcreg];
{	CPTR dsta = nextilong();
	WORD dst = get_word(dsta);
	src |= dst;
test_flags_w(src);
	put_word(dsta,src);
}}}}
void op_8190(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}
void op_8198(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	LONG dst = get_long(dsta);
{	regs.a[dstreg] += 4;
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}}
void op_81a0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	LONG src = regs.d[srcreg];
{	regs.a[dstreg] -= 4;
{	CPTR dsta = regs.a[dstreg];
	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}}
void op_81a8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg] + (LONG)(WORD)nextiword();
	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}
void op_81b0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
	ULONG dstreg = (opcode & 7) >> 0;
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = regs.a[dstreg];
	dsta = get_disp_ea (dsta, nextiword());
{	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}}
void op_81b8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = (LONG)(WORD)nextiword();
	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}
void op_81b9(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 3584) >> 9);
{{	LONG src = regs.d[srcreg];
{	CPTR dsta = nextilong();
	LONG dst = get_long(dsta);
	src |= dst;
test_flags_l(src);
	put_long(dsta,src);
}}}}
void op_81c0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = regs.d[srcreg];
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_81d0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_81d8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	regs.a[srcreg] += 2;
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_81e0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	regs.a[srcreg] -= 2;
{	CPTR srca = regs.a[srcreg];
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_81e8(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg] + (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_81f0(ULONG opcode)
{
	ULONG srcreg = (LONG)(BYTE)((opcode & 7) >> 0);
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = regs.a[srcreg];
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_81f8(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = (LONG)(WORD)nextiword();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_81f9(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = nextilong();
	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
void op_81fa(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca += (LONG)(WORD)nextiword();
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_81fb(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	CPTR srca = m68k_getpc();
	srca = get_disp_ea (srca, nextiword());
{	WORD src = get_word(srca);
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}}
void op_81fc(ULONG opcode)
{
	ULONG dstreg = (opcode & 3584) >> 9;
{{	WORD src = nextiword();
{	LONG dst = regs.d[dstreg];
	if(src != 0){
	LONG newv = (LONG)dst / (WORD)src;
	UWORD rem = (LONG)dst % (WORD)src;
	if ((newv & 0xffff0000) && (newv & 0xffff0000) != 0xffff0000) { VFLG = NFLG = 1; } else
	{
	if (((WORD)rem < 0) != ((LONG)dst < 0)) rem = -rem;
test_flags_w(newv);
	newv = (newv & 0xffff) | ((ULONG)rem << 16);
	regs.d[dstreg] = (newv);
	}
	}
}}}}
