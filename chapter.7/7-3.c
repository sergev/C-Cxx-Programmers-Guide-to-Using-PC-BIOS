/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-3.  Test Whether a Key Has Been Pressed
*/


/*
   Returns 0 if typeahead buffer is empty, 
   or nonzero if data is present
*/

unsigned int testKbdCharReady( unsigned char *charCode,
                               unsigned char *scanCode )
{
unsigned char localAH, 
              localAL;
unsigned int returnValue;

/* Switch to inline assembly mode to do the interrupt */
_asm  mov AH, 1             /* AH = 1 or 11h - check keyboard */
_asm  int 16h               /* Call BIOS */
_asm  jnz endFunc           /* Zero false means character ready */
_asm  xor AX,AX             /* Else, buffer empty - return 0 */
endFunc:                    /* Move AH and AL to local variable */
_asm  mov localAH, AH       /* AH = scan code */
_asm  mov localAL, AL       /* AL = character code */
_asm  mov returnValue, AX   /* Save 16-bit composite for return code */

/* Leave inline assembly mode and finish the call in C */
*scanCode = localAH;        /* Copy scan code to caller's var */
*charCode = localAL;        /* Copy char code to caller's var */

return( returnValue );      /* Return >0 if data is ready */
}
