/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-8.  Exchange Printers
*/


/*
   EXCHANGE.C
   Exchange two printers' BDA data
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define firstPrinterAddress (unsigned int far *)0x00400008
#define firstPrinterTimeout (unsigned char far *)0x00400078

void main( int argc, char *argv[] )
{
static char lpt[] = "lpt";

unsigned int tempAddress;
unsigned char tempTimeout;

unsigned char *printer1,
              *printer2;

unsigned int printer1Index,
             printer2Index;

unsigned int far *printer1Address,
             far *printer2Address;

unsigned char far *printer1Timeout,
              far *printer2Timeout;

/* Error unless two parameters appear */
if ( (argc != 3) ||
     (memicmp(argv[1],lpt,strlen(lpt)) != 0) ||
     (memicmp(argv[2],lpt,strlen(lpt)) != 0) )
{
   printf( "Syntax: %s <LPT1/2/3> <LPT1/2/3>\7", argv[0] );
   exit( 1 );
   }

/* Point to printer numbers */
printer1 = argv[1];
printer1 += strlen( lpt );

printer2 = argv[2];
printer2 += strlen( lpt );

if ( (*printer1 != '1') &&
     (*printer1 != '2') &&
     (*printer1 != '3') )
{
   printf( "Printer 1 must be LPT1, LPT2, or LPT3!\7" );
   exit( 1 );
   }

if ( (*printer2 != '1') &&
     (*printer2 != '2') &&
     (*printer2 != '3') )
{
   printf( "Printer 2 must be LPT1, LPT2, or LPT3!\7" );
   exit( 1 );
   }

if ( (*printer1) == (*printer2) )
{
   printf( "Printers 1 and 2 cannot be the same!\7" );
   exit( 1 );
   }

/* Convert ASCII printer numbers into integers */
printer1Index = ((*printer1) - '0');
printer2Index = ((*printer2) - '0');

/* Bias both for base 0 indexing */
printer1Index -= 1;
printer2Index -= 1;

/* Point to printer1's BDA address fields */
printer1Address = firstPrinterAddress;
printer1Timeout = firstPrinterTimeout;

while ( printer1Index )
{
   ++printer1Address;
   ++printer1Timeout;
   --printer1Index;
   }

/* Point to printer2's BDA address fields */
printer2Address = firstPrinterAddress;
printer2Timeout = firstPrinterTimeout;

while ( printer2Index )
{
   ++printer2Address;
   ++printer2Timeout;
   --printer2Index;
   }

/* Warn user if either printer not installed */
if ( *printer1Address == 0 )
   printf( "Warning - Printer 1 is not installed!\7" );

if ( *printer2Address == 0 )
   printf( "Warning - Printer 2 is not installed!\7" );

/* Swap printers via temporary storage */
tempAddress = *printer1Address;       /* Printer 1 to temp */
tempTimeout = *printer1Timeout;

*printer1Address = *printer2Address;  /* Printer 2 to printer 1 */
*printer1Timeout = *printer2Timeout;

*printer2Address = tempAddress;       /* temp to Printer 2 */
*printer2Timeout = tempTimeout;

printf( "\nPrinters have been swapped." );
exit( 0 );
}
