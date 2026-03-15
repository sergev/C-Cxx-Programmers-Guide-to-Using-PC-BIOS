/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Printer Functions
*/


/* Int 05h - Print Screen Key Software Interface */

#define pStat (unsigned char far *)0x00500000
unsigned char printScreen( void )
{
union REGS regs;

int86( 0x05, &regs, &regs );     /* Call BIOS */
return( *pStat );                /* Return status from 0x00500000 */
}



/* Int 17h / 00h - Print Character */

unsigned char printChar( unsigned int printer, 
                         unsigned char chToPrint )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - print character */
regs.h.al = chToPrint;        /* AL = character to print */
regs.x.dx = printer;          /* DX = printer number */
int86( 0x17, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 17h / 01h - Initialize Printer Port */

unsigned char initPrinterPort( unsigned int port )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - initialize printer port */
regs.x.dx = port;             /* DX = printer number */
int86( 0x17, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 17h / 02h - Get Printer Status */

unsigned char getPrinterStatus( unsigned int printer )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - read printer status */
regs.x.dx = printer;          /* DX = printer number */
int86( 0x17, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}
