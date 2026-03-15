/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-1.  Detect Installed Printer Ports
*/


/* SHOW-LPT.C
   Determine which parallel printers are installed by 
   looking for their base I/O addresses in the BIOS Data Area
*/

#include <stdio.h>

#define boolean unsigned char

#define lpt1Base (unsigned int far *)0x00400008
#define lpt2Base (unsigned int far *)0x0040000a
#define lpt3Base (unsigned int far *)0x0040000c

boolean checkPrinter( char *, unsigned int far * );

void main( void )
{
checkPrinter( "LPT1", lpt1Base );
checkPrinter( "LPT2", lpt2Base );
checkPrinter( "LPT3", lpt3Base );
}

/* Return 1 = printer port installed or 
          0 = not installed
*/
boolean checkPrinter( char *printerName,
                      unsigned int far *ioAddress )
{
if( *ioAddress == 0 )
   printf( "\n%s : is not installed.", printerName );
else
   printf( "\n%s : is installed at I/O address %.4X.",
           printerName,*ioAddress );

return( ( *ioAddress != 0 ) );
}
