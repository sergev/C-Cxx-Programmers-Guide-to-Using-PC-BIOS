/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-14.  Scroll Screen Up or Down
*/ 


/* Scrolls part of screen up or down */

#define scrollUp(nLines,color,tr,lc,br,rc)   \
        scrollScreen(6,nLines,color,tr,lc,br,rc)  /* BIOS function 6 */

#define scrollDown(nLines,color,tr,lc,br,rc) \
        scrollScreen(7,nLines,color,tr,lc,br,rc)  /* BIOS function 7 */

void scrollScreen( unsigned char BIOSFunc6or7,
                   unsigned char nLines, 
                   unsigned char attribute, 
                   unsigned char topRow, 
                   unsigned char leftColumn, 
                   unsigned char bottomRow, 
                   unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = BIOSFunc6or7;     /* AH = 6 (scroll up), 7 (scroll down) */
regs.h.al = nLines;           /* AL = number of lines to scroll, */
                              /*      or 0 to clear entire area */
regs.h.bh = attribute;        /* BH = attribute for new lines */
regs.h.ch = topRow;           /* CH = top row */
regs.h.cl = leftColumn;       /* CL = left column */
regs.h.dh = bottomRow;        /* DH = bottom row */
regs.h.dl = rightColumn;      /* DL =  right column */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
