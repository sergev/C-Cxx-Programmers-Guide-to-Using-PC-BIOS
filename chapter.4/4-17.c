/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-17.  Change CGA Background/Border Color
*/ 


/*
   For CGA displays:
      - set screen border color for text modes
      - set background color for 320x200 graphics modes
      - set foreground color for 640x200 graphics modes
*/

void setCgaColor( unsigned char newColorValue )
{
union REGS regs;

regs.h.ah = 0x0b;             /* AH = function 0Bh */
regs.h.bh = 0;                /* BH = subfunction 0 */
regs.h.bl = newColorValue;    /* BL = new color value */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
