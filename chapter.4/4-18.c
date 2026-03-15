/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-18.  Change CGA Graphics Palette
*/ 


/* Set CGA graphics mode 4 or 5 palette */

#define MODE0  0    /* Colors green, red, and brown/yellow */
#define MODE1  1    /* Colors cyan, magenta, and white */

void setCgaGraphicsPalette( unsigned char newPalette )
{
union REGS regs;

assert( (newPalette == MODE0) || (newPalette == MODE1) );

regs.h.ah = 0x0b;             /* AH = function 0Bh */
regs.h.bh = 1;                /* BH = subfunction 1 */
regs.h.bl = newPalette;       /* BL = new palette value */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
