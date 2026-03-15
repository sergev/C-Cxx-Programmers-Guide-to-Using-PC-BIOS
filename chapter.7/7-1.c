/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-1.  Read Keyboard Character
*/


/*
   Returns next scan code/character code 
   from typeahead buffer
*/

void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 or 10h - read keyboard */
int86( 0x16, &regs, &regs );  /* Call BIOS */

*scanCode = regs.h.ah;        /* AH = scan code */
*charCode = regs.h.al;        /* AL = character code */
}                             /* Returns nothing */
