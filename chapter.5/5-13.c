/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-13.  Report Whether Diskette Drive Door Has Been Opened
*/


/*
   Determine whether specified drive's door 
   has been opened (on systems that support 
   change line signal)

   Returns:  0 = door HAS NOT been opened
             1 = invalid drive specified
             6 = door HAS been opened
*/

unsigned char getChangeSignal( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x16;             /* AH = 16h - get change line status */
regs.h.dl = drive;            /* DL = drive. 0 for A or 1 for B */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return ( regs.h.ah );         /* AH = 0, 1, or 6 as above */
}
