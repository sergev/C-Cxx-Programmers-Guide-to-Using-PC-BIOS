/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 8-3.  Display Printer Status
*/


/*
   PRNTSTAT.C
   Display current status for each parallel printer
*/

#include <dos.h>
#include <stdio.h>

#define boolean unsigned char

#define bitOnOff( value, bit ) (value & bit) ? '1' : '0'

#define lpt1Base (unsigned int far *)0x00400008
#define lpt2Base (unsigned int far *)0x0040000a
#define lpt3Base (unsigned int far *)0x0040000c

/* Printer status bits */
#define READY    128
#define ACK       64
#define PAPEROUT  32
#define SELECTED  16
#define IOERROR    8
#define TIMEOUT    1

/* Function prototypes */
boolean checkPrinter( char *, unsigned int far *);
unsigned char getPrinterStatus( unsigned int );
void showPrinterStatus( unsigned int );

void main( void )
{
if ( checkPrinter( "LPT1", lpt1Base ) )
   showPrinterStatus( 0 );

if ( checkPrinter( "LPT2", lpt2Base ) )
   showPrinterStatus( 1 );

if ( checkPrinter( "LPT3", lpt3Base ) )
   showPrinterStatus( 2 );
}


/* Returns 1 = printer port installed, or 
           0 = printer port NOT installed 
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

/* Returns a printer's status */
unsigned char getPrinterStatus( unsigned int whichPrinter )
{
union REGS regs;

regs.h.ah = 2;                /* AH = function 2         */
regs.x.dx = whichPrinter;     /* DX = printer (0, 1, 2)  */
int86( 0x17, &regs, &regs );  /* Call BIOS               */

return( regs.h.ah );          /* AH = status */
}


/* Decode and display printer status */
void showPrinterStatus( unsigned int printer )
{
unsigned char status;

status = getPrinterStatus( printer );

printf( "\n\tBit 7 = %c = Printer Ready",      
        bitOnOff(status, READY) );

printf( "\n\tBit 6 = %c = Acknowledge",        
        bitOnOff(status, ACK) );

printf( "\n\tBit 5 = %c = Out of paper",       
        bitOnOff(status, PAPEROUT) );

printf( "\n\tBit 4 = %c = Printer selected",   
        bitOnOff(status, SELECTED) );

printf( "\n\tBit 3 = %c = I/O error occurred", 
        bitOnOff(status, IOERROR) );

printf( "\n\tBit 2 = %c = Reserved",           
        bitOnOff(status, 4) );

printf( "\n\tBit 1 = %c = Reserved",           
        bitOnOff(status, 2) );

printf( "\n\tBit 0 = %c = Printer timed out",  
        bitOnOff(status, TIMEOUT) );
}
