/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-2.  Determine Numbers of Screen Rows and Columns
*/


/*
   Returns:
        - current video mode
        - active video page
        - number of rows and columns
*/
void getScreenParameters( unsigned char *videoMode,
                          unsigned char *activePage,
                          unsigned char *textRows,
                          unsigned char *textColumns )
{
#define BDA_rows (unsigned int far *)0x00400084

union REGS regs;

regs.h.ah = 0x0f;               /* AH = 0x0F - get screen parameters */
int86( 0x10, &regs, &regs );    /* Call BIOS */

*videoMode = regs.h.al;         /* AL = current video mode */
*activePage = regs.h.bh;        /* BH = active video page */
*textColumns = regs.h.ah;       /* AH = number of screen columns */
*textRows = *BDA_rows + 1;      /* Number of screen rows from BDA */
}
