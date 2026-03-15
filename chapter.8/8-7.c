/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-7.  Enable or Disable Print Screen Key
*/


/* Disable Print Screen Key */

#define pStat (unsigned char far *)0x00500000

void disablePrintScrn( void )
{
#define INPROGRESS 1         /* 1 = printing underway */

*pStat = INPROGRESS;         /* Makes BIOS ignore Print Screen */
}


/* Enable Print Screen Key */
void enablePrintScrn( void )
{
#define COMPLETE 0           /* 0 = printing is complete */

*pStat = COMPLETE;           /* Makes BIOS honor Print Screen */
}
