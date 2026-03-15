/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-19.  Change EGA/VGA Palette and Border Colors
*/


/* Set EGA/VGA palette entry */
void setEgaVgaPaletteEntry( unsigned char entry,
                            unsigned char newColor )
{
union REGS regs;

regs.x.ax = 0x1000;           /* AX = function 10h subfunction 0 */
regs.h.bh = newColor;         /* BH = new color value */
regs.h.bl = entry;            /* BL = palette entry to change */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */


/* Set EGA/VGA border color */
void setEgaVgaBorderColor( unsigned char newColor )
{
union REGS regs;

regs.x.ax = 0x1001;           /* AX = function 10h subfunction 1 */
regs.h.bh = newColor;         /* BH = new color value */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
