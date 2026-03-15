/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-6.  Print Current Screen
*/


/*
   Print screen contents to LPT1 and return status from 50:00h
*/

#define pStat (unsigned char far *)0x00500000

unsigned char printScreen( void )
{
union REGS regs;                  /* No register setup needed */

int86( 0x05, &regs, &regs );      /* Call BIOS */
return( *pStat );                 /* Return status */
}
