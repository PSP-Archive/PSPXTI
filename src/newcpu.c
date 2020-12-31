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
  * MC68000 emulation
  *
  * (c) 1995 Bernd Schmidt
  */

#include <stdlib.h>
#include "sysdeps.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "config.h"
#include "options.h"
#include "memory.h"
#include "debug.h"
#include "newcpu.h"
#include "readcpu.h"
#include "hardware.h"
#include "specific.h"
#include "globinfo.h"

int areg_byteinc[] = { 1,1,1,1,1,1,1,2 };
int imm8_table[] = { 8,1,2,3,4,5,6,7 };

cpuop_func *cpufunctbl[65536];


unsigned long specialflags;

int speedyLink = 0;

int currIntLev = -1;

int broken_in;

int delayTime = 0;


#ifdef INTEL_FLAG_OPT
union flagu intel_flag_lookup[256];
#endif

#ifdef PENT_COUNTER


#define readpec(l) __asm__(".byte 0x0f, 0x31" :"=a" (l), "=d" (highCnt))

/* Dummy */
unsigned long highCnt;

/* Pentium ticks between do_period() calls */
unsigned long timeDiff = 0;

/* Pentium ticks betweein instructions */
unsigned long cycleDiff = 0;

/* Pentium ticks on one millisecond */
unsigned long permSec = 0;

unsigned long lastTime = 0;
unsigned long cycleRate = 0;
unsigned long tickRate = 0;

void calibrate_pcounter(void) {

  unsigned long  i,j;
  
  unsigned long long k;
  
  readpec(i);
  usleep(1000000);
  readpec(j);
    
  if(i >  j) {
    readpec(i);
    usleep(1000000);
    readpec(j);
  }
  
  permSec = (j-i)/1000;

  tickRate = globInf.tickRate;
  cycleRate = globInf.cycleRate;

  timeDiff = ((long long)permSec * (long long)tickRate)/100000;
  cycleDiff = (permSec*cycleRate)/100000;

}


#endif



void init_m68k (void)
{
    long int opcode;
    int i;

#ifdef INTEL_FLAG_OPT
    for (i = 0; i < 256; i++) {
    intel_flag_lookup[i].flags.c = !!(i & 1);
    intel_flag_lookup[i].flags.z = !!(i & 64);
    intel_flag_lookup[i].flags.n = !!(i & 128);
    intel_flag_lookup[i].flags.v = 0;
    }
#endif
    read_table68k ();
    do_merges ();
    for (opcode = 0; opcode < 65536; opcode++)
    cpufunctbl[opcode] = op_illg;
    for (i = 0; smallcputbl[i].handler != NULL; i++) {
    if (!smallcputbl[i].specific)
        cpufunctbl[smallcputbl[i].opcode] = smallcputbl[i].handler;
    }
    for (opcode = 0; opcode < 65536; opcode++) {
    cpuop_func *f;
    
    if (table68k[opcode].mnemo == i_ILLG)
        continue;
    
    if (table68k[opcode].handler != -1) {
        f = cpufunctbl[table68k[opcode].handler];
        if (f == op_illg)
        abort();
        cpufunctbl[opcode] = f;
    }
    }    
    for (i = 0; smallcputbl[i].handler != NULL; i++) {
    if (smallcputbl[i].specific)
        cpufunctbl[smallcputbl[i].opcode] = smallcputbl[i].handler;
    }
}

struct regstruct regs;
union flagu regflags;
static void ShowEA(int reg, amodes mode, wordsizes size)
{
    UWORD dp;
    BYTE disp8;
    WORD disp16;
    int r;
    ULONG dispreg;
    CPTR addr;
    
    switch(mode){
     case Dreg:
    printf("D%d", reg);
    break;
     case Areg:
    printf("A%d", reg);
    break;
     case Aind:
    printf("(A%d)", reg);
    break;
     case Aipi:
    printf("(A%d)+", reg);
    break;
     case Apdi:
    printf("-(A%d)", reg);
    break;
     case Ad16:
    disp16 = nextiword();
    addr = regs.a[reg] + (WORD)disp16;
    printf("(A%d,$%04lx) == $%08lx", reg, disp16, (long unsigned int)addr);
    break;
     case Ad8r:
    dp = nextiword();
    disp8 = dp & 0xFF;
    r = (dp & 0x7000) >> 12;
    dispreg = dp & 0x8000 ? regs.a[r] : regs.d[r];    
    if (!(dp & 0x800)) dispreg = (LONG)(WORD)(dispreg);
    
    addr = regs.a[reg] + disp8 + dispreg;
    printf("(A%d, %c%d.%c, $%02x) == $%08lx", reg, 
           dp & 0x8000 ? 'A' : 'D', (int)r, dp & 0x800 ? 'L' : 'W', disp8,
           (long unsigned int)addr);
    break;
     case PC16:
    addr = m68k_getpc();
    disp16 = nextiword();
    addr += (WORD)disp16;
    printf("(PC,$%08lx) == $%08lx", disp16, (long unsigned int)addr);
    break;
     case PC8r:
    addr = m68k_getpc();
    dp = nextiword();
    disp8 = dp & 0xFF;
    r = (dp & 0x7000) >> 12;
    dispreg = dp & 0x8000 ? regs.a[r] : regs.d[r];
    
    if (!(dp & 0x800)) dispreg = (LONG)(WORD)(dispreg);
    addr += disp8 + dispreg;
    printf("(PC, %c%d.%c, $%02x) == $%08lx", dp & 0x8000 ? 'A' : 'D', 
           (int)r, dp & 0x800 ? 'L' : 'W', disp8, (long unsigned int)addr);
    break;
     case absw:
    printf("$%08lx", (LONG)(WORD)nextiword());
    break;
     case absl:
    printf("$%08lx", nextilong());
    break;
     case imm:
    switch(size){
     case sz_byte:
        printf("#$%02x", nextiword() & 0xff); break;
     case sz_word:
        printf("#$%04x", nextiword()); break;
     case sz_long:
        printf("#$%08lx", nextilong()); break;
     default:
        abort();
    }
    break;
     case imm0:
    printf("#$%02lx", nextiword() & 0xff);
    break;
     case imm1:
    printf("#$%04lx", nextiword());
    break;
     case imm2:
    printf("#$%08lx", nextilong());
    break;
     case immi:
    printf("#$%04lx", reg);
    break;
     default:
    abort();
    }
}


int intlev(void) {
  int rc = currIntLev;
  currIntLev = -1;
  return rc;
}

void MakeSR(void)
{

    regs.sr = ((regs.t << 15) | (regs.s << 13) | (regs.intmask << 8)
           | (regs.x << 4) | (NFLG << 3) | (ZFLG << 2) | (VFLG << 1) 
           |  CFLG);
}

void MakeFromSR(void)
{
  int olds = regs.s;

  regs.t = (regs.sr >> 15) & 1;
  regs.s = (regs.sr >> 13) & 1;
  regs.intmask = (regs.sr >> 8) & 7;
  regs.x = (regs.sr >> 4) & 1;
  NFLG = (regs.sr >> 3) & 1;
  ZFLG = (regs.sr >> 2) & 1;
  VFLG = (regs.sr >> 1) & 1;
  CFLG = regs.sr & 1;
  if (olds != regs.s) {
    CPTR temp = regs.usp;
    regs.usp = regs.a[7];
    regs.a[7] = temp;
  }

  if(speedyLink && regs.intmask < 4 && (ti_io[0xc]&0x2 || link_checkread())) {
    ti_io[0xc] |= 0x2;
    specialflags |= SPCFLAG_INT;
    currIntLev = 4;
  }

  specialflags |= SPCFLAG_INT;
  if (regs.t)
    specialflags |= SPCFLAG_TRACE;
  else
    specialflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);
}

void Exception(int nr)
{
  MakeSR();
  if (!regs.s) {
    CPTR temp = regs.usp;
    regs.usp = regs.a[7];
    regs.a[7] = temp;
    regs.s = 1;
  }

  regs.a[7] -= 4;
  put_long (regs.a[7], m68k_getpc ());
  regs.a[7] -= 2;
  put_word (regs.a[7], regs.sr);
  m68k_setpc(get_long(regs.vbr + 4*nr));
  regs.t = 0;
  specialflags &= ~(SPCFLAG_TRACE | SPCFLAG_DOTRACE);

}

int uscycle = 0;

static inline void 
Interrupt(int nr)
{
# if 0 //LUDO:
  assert(nr < 8 && nr >= 0);

  
  if(nr == 1 && globInf.syncOne && ++uscycle == 4) {
    ti92_update_screen();
    uscycle = 0;
  }
# endif
  Exception(nr+24);

  regs.intmask = nr;
  specialflags |= SPCFLAG_INT;

}

static int caar, cacr;

void m68k_move2c (int regno, ULONG *regp)
{
  switch (regno) {
  case 0: regs.sfc = *regp; break;
  case 1: regs.dfc = *regp; break;
  case 2: cacr = *regp & 0xFF; break;
  case 0x800: regs.usp = *regp; break;
  case 0x801: regs.vbr = *regp; break;
  case 0x802: caar = *regp & 0xFF; break;
  default:
    op_illg (0x4E7B);
    break;
  }
}

void m68k_movec2 (int regno, ULONG *regp)
{
  switch (regno) {
  case 0: *regp = regs.sfc; break;
  case 1: *regp = regs.dfc; break;
  case 2: *regp = cacr; break;
  case 0x800: *regp = regs.usp; break;
  case 0x801: *regp = regs.vbr; break;
  case 0x802: *regp = caar; break;
  default:
    op_illg (0x4E7A);
    break;
  }
}

extern void m68k_divl (UWORD opcode, ULONG src, UWORD extra)
{
    if (src == 0)
    return;
#ifdef INT_64BIT
    if (extra & 0x800) {
    /* signed variant */
    INT_64BIT a = regs.d[(extra >> 12) & 7];
    INT_64BIT quot, rem;
    
    if (extra & 0x400)
        a |= (INT_64BIT)regs.d[extra & 7] << 32;
    rem = a % src;
    quot = a / src;
    if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
        regs.d[extra & 7] = rem;
    regs.d[(extra >> 12) & 7] = quot;
    } else {
    /* unsigned */
    unsigned INT_64BIT a = regs.d[(extra >> 12) & 7];
    unsigned INT_64BIT quot, rem;
    
    if (extra & 0x400)
        a |= (INT_64BIT)regs.d[extra & 7] << 32;
    rem = a % src;
    quot = a / src;
    if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
        regs.d[extra & 7] = rem;
    regs.d[(extra >> 12) & 7] = quot;
    }
#endif
}

extern void m68k_mull (UWORD opcode, ULONG src, UWORD extra)
{
#ifdef INT_64BIT
    if (extra & 0x800) {
    /* signed variant */
    INT_64BIT a = (LONG)regs.d[(extra >> 12) & 7];

    a *= (LONG)src;
    if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
        regs.d[extra & 7] = a >> 32;
    regs.d[(extra >> 12) & 7] = (ULONG)a;
    } else {
    /* unsigned */
    unsigned INT_64BIT a = (ULONG)regs.d[(extra >> 12) & 7];
    unsigned INT_64BIT quot, rem;
    
    a *= src;
    if ((extra & 0x400) && (extra & 7) != ((extra >> 12) & 7))
        regs.d[extra & 7] = a >> 32;
    regs.d[(extra >> 12) & 7] = (ULONG)a;
    }
#endif
}

static char* ccnames[] =
{ "T ","F ","HI","LS","CC","CS","NE","EQ",
  "VC","VS","PL","MI","GE","LT","GT","LE" };

void MC68000_reset(void)
{
  /* LUDO: */
  CPTR scan_rom_addr;

  regs.a[7] = get_long(0x0);
  if (ROM_SIZE <= 0x100000) {
    m68k_setpc(get_long(0x4));
  } else {
    for (scan_rom_addr = 0x12000; scan_rom_addr < ROM_SIZE; scan_rom_addr += 4) {
      if (get_long(scan_rom_addr) == 0xcccccccc) {
        scan_rom_addr += 8;
        m68k_setpc(get_long(scan_rom_addr));
        break;
      }
    }
    if (scan_rom_addr == ROM_SIZE) {
      /* Could be a 92 ROM ? */
      m68k_setpc(get_long(0x4));
    }
  }
  /* END LUDO */
  regs.s = 1;
  regs.stopped = 0;
  regs.t = 0;
  specialflags = 0;
  regs.intmask = 7;
  regs.vbr = regs.sfc = regs.dfc = 0;
}

void op_illg(ULONG opcode)
{
    regs.pc_p-=2;

    if ((opcode & 0xF000) == 0xF000) {
    if ((opcode & 0xE00) == 0x200)
        Exception(0xB);
    else 
        switch (opcode & 0x1FF) {
         case 0x17:
        regs.pc_p+=4;
        break;
         default:
        regs.pc_p+=2;
        }
    return;
    }
    if ((opcode & 0xF000) == 0xA000) {
        Exception(0xA);
    return;
    }

    /*    fprintf(stderr, "Illegal instruction: %04x\n", opcode);*/
    Exception(4);

}



void MC68000_run(void)
{
  int t, p;
  unsigned long l, k;
  
  for(;;) {
    UWORD opcode;

    opcode = nextiword();

#ifdef PENT_COUNTER    

    readpec(l);

    (*cpufunctbl[opcode])(opcode);

    if(l - lastTime >= timeDiff) {
      lastTime = l;
      do_period();
      if(globInf.tickRate != tickRate) {
        tickRate = globInf.tickRate;
        timeDiff = ((long long)permSec * (long long)tickRate)/100000;
      }
      if(globInf.cycleRate != cycleRate) {
        cycleRate = globInf.cycleRate;
        cycleDiff = (permSec*cycleRate)/100000;    
      }
    }
    else
      do { 
        readpec(k);
      } while( (k - l) < cycleDiff);

#else
    (*cpufunctbl[opcode])(opcode);
    do_cycles();
#endif

    if(specialflags) {

      if(specialflags & SPCFLAG_ADRERR) {
        Exception(3);
        specialflags &= ~SPCFLAG_ADRERR;
      }
 
      if (specialflags & SPCFLAG_DOTRACE) {
        Exception(9);
      }

      while (specialflags & SPCFLAG_STOP) {
        do_cycles();
        if (specialflags & (SPCFLAG_INT | SPCFLAG_DOINT)) {
          int intr = intlev();
          specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
          specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
          if (intr != -1 && intr > regs.intmask) {
            Interrupt(intr);
            regs.stopped = 0;
            specialflags &= ~SPCFLAG_STOP;
          }        
        }        
      }

      if (specialflags & SPCFLAG_TRACE) {
        specialflags &= ~SPCFLAG_TRACE;
        specialflags |= SPCFLAG_DOTRACE;
      }
      
      if (specialflags & SPCFLAG_DOINT) {
        int intr = intlev();
        specialflags &= ~(SPCFLAG_INT | SPCFLAG_DOINT);
        if (intr != -1 && intr > regs.intmask) {
          Interrupt(intr);
          regs.stopped = 0;
        }        
      }
      if (specialflags & SPCFLAG_INT) {
        specialflags &= ~SPCFLAG_INT;
        specialflags |= SPCFLAG_DOINT;
      }
    }  
  }
}

