/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-2.  Read Keyboard Status Byte
*/


/*
   READBDA.C
   Continually displays Keyboard Status Byte at BDA location 40:17
*/

#include <stdio.h>

#define kbdStatByte (unsigned char far *)0x00400017

/* Define bits used in kbdStatByte */
#define K_INSERT    128
#define K_CAPSLOCK   64
#define K_NUMLOCK    32
#define K_SCROLLLOCK 16
#define K_ALT         8
#define K_CTRL        4
#define K_LSHIFT      2
#define K_RSHIFT      1

/* Macro returns ASCII 1 if bit is set, or ASCII 0 if clear */
#define bitOnOff( flag, bit ) ((flag) & bit) ? '1' : '0'

void main( void )
{
printf( "\nBits 76543210 at location 40:17\n" );

for(;;)
{
   printf( "\015     %c%c%c%c%c%c%c%c",
           bitOnOff( *kbdStatByte, K_INSERT ),
           bitOnOff( *kbdStatByte, K_CAPSLOCK ),
           bitOnOff( *kbdStatByte, K_NUMLOCK ),
           bitOnOff( *kbdStatByte, K_SCROLLLOCK ),
           bitOnOff( *kbdStatByte, K_ALT ),
           bitOnOff( *kbdStatByte, K_CTRL ),
           bitOnOff( *kbdStatByte, K_LSHIFT ),
           bitOnOff( *kbdStatByte, K_RSHIFT ) );
   }
}
