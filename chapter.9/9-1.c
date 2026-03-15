/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-1.  Read System Tick Counter
*/


/*
   Store system tick counter in caller's variable 
   and return midnight rollover flag
*/

unsigned char readTickCounter( unsigned long *callvar )
{
union REGS regs;
unsigned long result;

regs.h.ah = 0;                /* AH = 0 - read tick counter */
int86( 0x1a, &regs, &regs );  /* Call BIOS */

result = regs.x.cx;           /* CX = upper 16 bits */
result <<= 16;                /* Shift them into place */
result |= regs.x.dx;          /* DX = lower 16 bits */
*callvar = result;            /* Copy 32 bits to caller's long */

return( regs.h.al );          /* AL = rollover flag */
}
