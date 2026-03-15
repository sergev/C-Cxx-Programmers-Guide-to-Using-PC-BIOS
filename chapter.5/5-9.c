/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-9.  Reset Drive and Controller
*/


unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset drive and controller */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = 0 and Carry false if success */
}                             /* AH = code and Carry true if error */
