/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-3.  Read CMOS Configuration RAM
*/


/*
   READCMOS.C
   Read and display CMOS RAM contents
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/* Macro generates 1 if specified bit is set, or 0 if clear */
#define bitOnOff( flag, bit ) ((flag) & bit) ? '1' : '0'

unsigned char readCmos( unsigned char index )
{
outp( 0x70, index );
return( inp(0x71) );
}

void main( void )
{
unsigned int cmosIndex;
unsigned char cmosValue;

printf( "\nCMOS    -- Contents In --" );
printf( "\nIndex    Hex      Binary" );

for( cmosIndex = 0;  cmosIndex != 64;  cmosIndex++ )
{
   cmosValue = readCmos( cmosIndex );
   printf( "\n %.2Xh     %.2Xh     %c%c%c%c%c%c%c%c",
             cmosIndex,
             cmosValue,
             bitOnOff( cmosValue, 128 ),
             bitOnOff( cmosValue,  64 ),
             bitOnOff( cmosValue,  32 ),
             bitOnOff( cmosValue,  16 ),
             bitOnOff( cmosValue,   8 ),
             bitOnOff( cmosValue,   4 ),
             bitOnOff( cmosValue,   2 ),
             bitOnOff( cmosValue,   1 ) );
   }
}
