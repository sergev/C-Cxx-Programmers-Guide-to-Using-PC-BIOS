/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-2.  Add Serial Port to the BDA
*/


/*
   ADDCOM.C
   Adds serial port address to BIOS Data Area
*/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define com1Base (unsigned int far *)0x00400000
#define com2Base (unsigned int far *)0x00400002
#define com3Base (unsigned int far *)0x00400004
#define com4Base (unsigned int far *)0x00400006

/* Function prototypes */
unsigned char addSerialPort ( unsigned int );
unsigned int  ASCIItoHex    ( char * );
unsigned char toHex         ( unsigned char );

void main( int argc, char *argv[] )
{
unsigned char portUsed;

if ( (argc != 2) || (strlen(argv[1]) != 4) )
{
   printf( "Syntax: %s <Base I/O address in 4-byte hex format>\7", 
          argv[0] );
   printf( "\nStandard addresses are 03F8, 02F8, 03E8, and 02E8" );
   exit( 1 );
   }

portUsed = addSerialPort( ASCIItoHex(argv[1]) );
if ( portUsed == 0 )
{
   printf( "\nAll four serial ports are occupied !\7" );
   exit( 2 );
   }

printf( "Port was added as COM%c", portUsed+'0' );
exit( 0 );
}

/* Returns 0 = no room in BDA for another serial port, or
           1,2,3,4 = added as COM1, COM2, COM3, or COM4.
*/
unsigned char addSerialPort( unsigned int ioAddress )
{
unsigned int far *comAddrP;
int portNr;

comAddrP = com1Base;

for( portNr = 1;  portNr < 5;  portNr++ )
{
   if ( *comAddrP == 0 )
   {                           /* Found an empty entry */
      *comAddrP = ioAddress;
      return( portNr );
      }
   ++comAddrP;
   }

/* All four ports are occupied ! */
return( 0 );
}

/* Returns binary equivalent of hex digit */
unsigned char toHex( unsigned char ch )
{
unsigned char binVal;

if ( isdigit(ch) )
   binVal = ch - '0';
else
   binVal = 10 + (toupper(ch) - 'A');

binVal &= 15;
return( binVal );
}

/* Converts ASCII hex string into unsigned int */
unsigned int ASCIItoHex( char *str )
{
unsigned int val;

val  = toHex(*(str+0)) << 12;
val |= (toHex(*(str+1)) << 8);
val |= (toHex(*(str+2)) << 4);
val |=  toHex(*(str+3));

return( val );
}
