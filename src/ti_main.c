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
  */

#include <stdlib.h>
#include "sysdeps.h"
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <zlib.h>
 
#include "config.h"
#include "options.h"
#include "memory.h"
#include "newcpu.h"
#include "debug.h"
#include "hardware.h"
#include "specific.h"
#include "globinfo.h"
#include "global.h"

#include "psp_fmgr.h"

struct GlobalInformation globInf;

/*
CPTR nextpc;

int enterDebugger = 0;
*/

int breakPoints = 0;
ULONG breakAdr[16];
UWORD breakSave[16];
int activeBreak = -1;


int intlink = 0;
int romsize = 0;
int dontSaveState = 1;

#define DEFAULT_CFGFILE "/tiger.cfg"
#define DEFAULT_ROMFILE "/ti92.rom"
#define DEFAULT_MEMFILE "/ti92.mem"

#define DEFAULT_DEVFILE "/dev/ti"

int
ti92_save_state(char* filename)
{
  gzFile *fp = gzopen(filename, "wb");
  if (! fp) return 1;

  int m = MEM_SIZE;
  m68k_setpc(m68k_getpc());
  MakeSR();
  gzwrite(fp, &m, sizeof(int));
  gzwrite(fp, ti_mem, MEM_SIZE);
  gzwrite(fp, ti_io, IO_SIZE);
  gzwrite(fp, &regs, sizeof(regs)); 
  gzwrite(fp, &timerVal, sizeof(timerVal));
  gzwrite(fp, &specialflags, sizeof(specialflags));
  gzclose(fp); 

  return 0;
}

int
ti92_load_state(char* filename)
{
  gzFile *fpz = gzopen(filename, "rb");
  if (! fpz) return 1;

  int m;
  gzread(fpz, &m, sizeof(int));
  if(m != MEM_SIZE) {
    gzclose(fpz);
    return 1;
  }
  else {
    gzread(fpz,ti_mem, MEM_SIZE);
    gzread(fpz,ti_io, IO_SIZE);
    gzread(fpz,&regs, sizeof(regs));
    gzread(fpz,&timerVal, sizeof(timerVal));
    gzread(fpz,&specialflags, sizeof(specialflags));
    gzclose(fpz); 
    MakeFromSR();
    m68k_setpc(regs.pc);
    update_contrast();
    update_bitmap();
  }

  return 0;
}

int
ti92_save_emulator_stuff(void)
{
  if(!dontSaveState && strlen(globInf.memFile)) {
    return ti92_save_state( globInf.memFile );
  }
  return 0;
}

int
ti92_load_prog_file(char *prog_filename)
{
  struct stat    aStat;
  static UBYTE   *backBuf = NULL;

  if (stat(prog_filename, &aStat)) return 1;

  if (backBuf == NULL) {
    backBuf = (UBYTE*)malloc(128*1024);
  }
  send_ti_file(prog_filename,backBuf);

  return 0;
}

void
hard_reset_emulator()
{
  rom_at_0();
  MC68000_reset();
  ram_at_0();
}

int 
run_emulator(void)
{
  FILE *fp;
  gzFile *fpz;

  strcpy(globInf.cfgFile, TI92.ti92_home_dir);
  strcat(globInf.cfgFile, DEFAULT_CFGFILE);

  strcpy(globInf.romFile, TI92.ti92_home_dir);
  strcat(globInf.romFile, DEFAULT_ROMFILE);

  strcpy(globInf.memFile, TI92.ti92_home_dir);
  strcat(globInf.memFile, DEFAULT_MEMFILE);

  strcpy(globInf.devFile, TI92.ti92_home_dir);
  strcat(globInf.devFile, DEFAULT_DEVFILE);
  globInf.memSize = 256;
  globInf.grayPlanes = 2;
  cycleInstr = globInf.itick = 640;
  globInf.tickRate = 40000;
  globInf.cycleRate = 150;
  globInf.darkCol = 0x000034;
  globInf.lightCol = 0x989898;
  globInf.syncOne = 0;
  
  //load_cfg_file(globInf.cfgFile);
  
  if((fp = fopen(globInf.romFile, "rb"))) {
    fseek(fp, 0, SEEK_END);
    romsize = ftell(fp);
    fclose(fp);
  }
  else {
    fprintf(stdout,"**Error: Cant find \"%s\"!\n", globInf.romFile);
    psp_sdl_exit(10);
  }

  if(!(romsize == 1024*1024 || romsize == 1024*1024*2)) {
    fprintf(stdout,"**Error: Illegal romsize. Must be 1Mb or 2Mb.\n");
    psp_sdl_exit(10);
  }
    
  if(!(mem_init(globInf.memSize, romsize))) {
    fprintf(stdout,"**Error: Could not initialize memory.\n");
    psp_sdl_exit(10);
  }    
    
  init_m68k();

  if((fp = fopen(globInf.romFile, "rb"))) {
    fread(ti_rom, 1, romsize, fp);
    fclose(fp);
  }
  else
    psp_sdl_exit(100);

  init_keyboard();
  init_specific();
  set_screen_ptr(ti_mem);

  init_hardware(globInf.devFile);

  if (ti92_load_state( globInf.memFile)) {
    /* error */
    rom_at_0();
    MC68000_reset();
    ram_at_0();
  }

#ifdef PENT_COUNTER
  if(globInf.tickRate)
    calibrate_pcounter();
#endif

  dontSaveState = 0;

  ti92_clear_screen();

  MC68000_run();

  ti92_save_emulator_stuff();

  return 0;
}
