/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-12.  Determine Change Line Signal Support
*/


/*
   Report whether specified drive is available 
   and if it supports change line

   Returns:  0 = drive NOT installed
             1 = drive installed WITHOUT change line signal
             2 = drive installed WITH change line signal
             3 = drive is a fixed disk
*/

unsigned char testDriveInstalled( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x15;             /* AH = 15h - test if drive installed */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return ( regs.h.ah );         /* AH = 0, 1, 2, 3 as above */
}
