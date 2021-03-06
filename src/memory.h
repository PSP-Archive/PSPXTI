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

extern int mem_init(int, int);
extern void mem_exit(void);
extern UBYTE get_byte(CPTR adr);
extern UWORD get_word(CPTR adr);
extern ULONG get_long(CPTR adr);
extern void put_long(CPTR adr, ULONG arg);
extern void put_word(CPTR adr, UWORD arg);
extern void put_byte(CPTR adr, UBYTE arg);

extern UBYTE io_get_byte(CPTR adr);
extern UWORD io_get_word(CPTR adr);
extern ULONG io_get_long(CPTR adr);
extern void io_put_long(CPTR adr, ULONG arg);
extern void io_put_word(CPTR adr, UWORD arg);
extern void io_put_byte(CPTR adr, UBYTE arg);

extern UBYTE *get_real_address(CPTR adr);
extern int valid_address(CPTR adr, ULONG size);

extern int buserr;

extern UBYTE *ti_mem;
extern UBYTE *ti_rom;
extern UBYTE *ti_io;

extern UBYTE * mem_tab[8];
extern ULONG mem_mask[8];


/* Banks sizes. Must be a power of two */
extern int  MEM_SIZE;
extern int ROM_SIZE;
#define IO_SIZE 32

#define rom_at_0() {mem_tab[0] = ti_rom; mem_mask[0] = ROM_SIZE-1;}
#define ram_at_0() {mem_tab[0] = ti_mem; mem_mask[0] = MEM_SIZE-1;}
