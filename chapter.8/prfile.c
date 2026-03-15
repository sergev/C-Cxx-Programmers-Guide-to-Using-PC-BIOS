/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-5.  Print Text File
*/


/*
   PRFILE.C
   Print text file
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define boolean unsigned char

#define lpt1Base    (unsigned int  far *)0x00400008
#define lpt2Base    (unsigned int  far *)0x0040000a
#define lpt3Base    (unsigned int  far *)0x0040000c
#define lpt1Timeout (unsigned char far *)0x00400078
#define lpt2Timeout (unsigned char far *)0x00400079
#define lpt3Timeout (unsigned char far *)0x0040007A

#define timeoutValue 20            /* About 20 seconds */

/* Printer status bits */
#define READY    128
#define ACK       64
#define PAPEROUT  32
#define SELECTED  16
#define IOERROR    8
#define TIMEOUT    1

#define FORMFEED 0x0c   /* ASCII form feed character */

/* Function prototypes */
unsigned char getPrinterStatus( unsigned int );
unsigned char printChar( int, unsigned char );
void diagnosePrinterProblem( unsigned char );
void printString( unsigned char *, unsigned char );


void main( int argc, char *argv[] )
{
unsigned char printer;
unsigned int far *ioPort;
unsigned char far *bdaTimeout;

FILE *inFile;
#define FILEBUFFERSIZE 128
char buffer[FILEBUFFERSIZE];

if ( argc != 3 )    /* Error unless two parameters provided */
{
   printf( "Syntax: %s <fileName> <LPT1,LPT2,LPT3> \7", argv[0] );
   exit( 1 );
   }

/* Get printer number and BDA locations */
printer = 0xff;
if ( stricmp( argv[2], "LPT1" ) == 0 )
{
   printer = 0;
   ioPort = lpt1Base;
   bdaTimeout = lpt1Timeout;
   }

if ( stricmp( argv[2], "LPT2" ) == 0 )
{
   printer = 1;
   ioPort = lpt2Base;
   bdaTimeout = lpt2Timeout;
   }

if ( stricmp( argv[2], "LPT3" ) == 0 )
{
   printer = 2;
   ioPort = lpt3Base;
   bdaTimeout = lpt3Timeout;
   }

/* Abort if printer name invalid */
if ( printer == 0xff )
{
   printf( "Printer must be LPT1, LPT2, or LPT3!\7" );
   exit( 1 );
   }

/* Abort if printer not installed */
if ( *ioPort == 0 )
{
   printf( "Sorry, printer %s is not installed.\7", argv[2] );
   exit( 1 );
   }

/* Prompt operator to bring printer online if needed */
if ( (getPrinterStatus( printer ) & READY) == 0 )
{
   printf( "Please bring the printer online and press Enter\7" );
   getch();
   }

inFile = fopen( argv[1], "rb" );
if ( inFile == NULL )
{
   printf( "Cannot open file %s!\7", argv[1] );
   exit( 1 );
   }

/* Set printer's timeout value */
*bdaTimeout = timeoutValue;

/* Print file */
while( fgets(buffer, FILEBUFFERSIZE, inFile) != 0 )
   printString( buffer, printer );

fclose( inFile );

/* Print trailing form feed to flush printer buffer */
printChar( printer, FORMFEED );
exit( 0 );
}

/* Returns printer status */
unsigned char getPrinterStatus( unsigned int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - get printer status */
regs.x.dx = whichPrinter;     /* DX = printer (0, 1, 2)  */
int86( 0x17, &regs, &regs );  /* Call BIOS */

return( regs.h.ah );          /* AH = status */
}

/* Print character */
unsigned char printChar( int whichPrinter, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = function 0         */
regs.h.al = chToPrint;        /* AL = character to print */
regs.x.dx = whichPrinter;     /* DX = printer (0, 1, 2)  */
int86( 0x17, &regs, &regs );  /* Call BIOS               */

return( regs.h.ah );          /* AH = printer status   */
}

/* Display meaning of printer status bits */
void diagnosePrinterProblem( unsigned char status )
{
printf( "\n*** Printer is " );

if ( (status & PAPEROUT) != 0 )
   printf( "out of paper-" );

if ( (status & SELECTED) == 0 )
   printf( "powered off-" );

if ( (status & TIMEOUT) == 0 )
   printf( "not ready to print" );

printf( "\nPress Enter to try again\7" );
getch();
}

/* Print null-terminated (ASCIIZ) string */
void printString( unsigned char *stringP, 
                  unsigned char whichPrinter )
{
unsigned char status;

status = getPrinterStatus( whichPrinter );
while( *stringP != 0 )
{
   if ( (status & (TIMEOUT+IOERROR+PAPEROUT)) != 0 )
      diagnosePrinterProblem( status );

   status = printChar( whichPrinter, *stringP );
   ++stringP;
   }
}
