/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Keyboard Functions
*/


/* Int 16h / 00h - Get Keyboard Character */

unsigned int getKbdChar( void )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - get keyboard character */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = scan code, AL = character code */
}


/* Int 16h / 01h - Check for Keyboard Data Ready */

unsigned int testKbdCharReady( void )
{
unsigned int  returnValue;
_asm  mov AH, 1            /* AH = 1 - check for key */
_asm  int 16h              /* Call BIOS */
_asm  jnz endFunc1         /* Zero false = character ready */
_asm  xor AX,AX            /* No character - return 0 */
endFunc1:
_asm  mov returnValue, AX  /* AH = scan code, AL = character code */
return( returnValue );     /* Return 0 or scan/char code */
}



/* Int 16h / 02h - Read Keyboard Shift Status */

unsigned char readShiftStatus( void )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - read keyboard shift state */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = shift codes */
}



/* Int 16h / 03h - Set Keyboard Repeat Rate */

void setRepeatRate( unsigned char delay, 
                    unsigned char repeatRate )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - set delay and repeat rate */
regs.h.al = 5;                /* AL must be 5 */
regs.h.bh = delay;            /* BH = new delay value */
regs.h.bl = repeatRate;       /* BL = new repeat rate */
int86( 0x16, &regs, &regs );  /* Call BIOS */
}



/* Int 16h / 05h - Write to Typeahead Buffer */

unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - write to typeahead buffer */
regs.h.ch = scanCode;         /* CH = scan code */
regs.h.cl = charCode;         /* CL = ASCII character */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = 0 (success) or 1 (buffer full) */
}



/* Int 16h / 10h - Get Enhanced Keyboard Character */

unsigned int getEnhancedKbdChar( void )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 0x10 - get enhanced kbd character */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = scan code, AL = character code */
}



/* Int 16h / 11h - Check for Enhanced Keyboard Data Ready */

unsigned int testEnhancedKbdCharReady( void )
{
unsigned int returnValue;
_asm  mov AH, 11h          /* AH = 0x11 - test for enh. kbd key */
_asm  int 16h              /* Call BIOS */
_asm  jnz endFunc11        /* Zero false = character ready */
_asm  xor AX,AX            /* No data ready - return 0 */
endFunc11:
_asm  mov returnValue, AX  /* AH = scan code, AL = character code */
return( returnValue );     /* Return 0 or scan/char code */
}



/* Int 16h / 12h - Read Extended Keyboard Shift Status */

unsigned int readExtendedShiftStatus( void )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 0x12 - read extended kbd shift state */
int86( 0x16, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = extended shift state */
                              /* AL = conventional shift state */
}
