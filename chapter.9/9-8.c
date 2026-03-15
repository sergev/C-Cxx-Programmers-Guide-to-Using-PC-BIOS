/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-8.  Suspend Program Execution
*/


/*
   Suspends program execution for n microseconds.  
   Returns:
      0 = program suspended
      1 = program NOT suspended because another timer is running
*/

unsigned int suspendProgram( unsigned long timeValue )
{
union REGS regs;

regs.h.ah = 0x86;                /* AH = 86h - suspend program */
regs.x.cx = (timeValue >> 16);   /* CX = high 16 bits of timeout */
regs.x.dx = timeValue;           /* DX = low 16 bits of timeout */
int86( 0x15, &regs, &regs );     /* Call BIOS */

return( regs.x.cflag );          /* Return Carry flag as indicator */
}
