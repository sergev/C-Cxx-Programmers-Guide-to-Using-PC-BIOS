/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-5.  Read or Set Cursor Size
*/


/*
   Returns current text mode cursor's 
   top and bottom scan lines
*/
void getCursorSize( unsigned char *topScanLine, 
                    unsigned char *bottomScanLine )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - get cursor position */
regs.h.bh = 0;                /* BH = video page (use any page) */
int86( 0x10, &regs, &regs );  /* Call BIOS */

*topScanLine = regs.h.ch;     /* CH = top scan line */
*bottomScanLine = regs.h.cl;  /* CL = bottom scan line */
}


/* Sets size of text mode cursor */
void setCursorSize( unsigned char topScanLine, 
                    unsigned char bottomScanLine )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - set cursor size */
regs.h.ch = topScanLine;      /* CH = top scan line in bits 4-0 */
regs.h.ch &= 31;              /* Mask unused bits */
regs.h.cl = bottomScanLine;   /* CL = bottom scan line in bits 4-0 */
regs.h.cl &= 31;              /* Mask unused bits */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
