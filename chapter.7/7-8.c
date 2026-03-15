/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-8.  Write Character into Typeahead Buffer
*/


/* 
   Writes scan code/character code into typeahead buffer.  
   Returns 0 if successful, 1 if buffer full
*/
unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - write to typeahead buffer */
regs.h.ch = scanCode;         /* CH = scan code */
regs.h.cl = charCode;         /* CL = character code */
int86( 0x16, &regs, &regs );  /* Call BIOS */

return( regs.h.al );          /* AL = 0 (success) or 1 (buffer full) */
}
