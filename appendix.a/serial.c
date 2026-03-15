/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Serial Port Functions
*/


/* Int 14h / 00h - Initialize Serial Port */

unsigned int initSerialPort( unsigned int port, 
                             unsigned char setupVal )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - initialize serial port */
regs.h.al = setupVal;         /* AL = port characteristics */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = line status, AL = modem status */
}



/* Int 14h / 01h - Transmit Character via Serial Port */

unsigned char comTx( unsigned int port, 
                     unsigned char charToXmit )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - transmit character */
regs.h.al = charToXmit;       /* AL = character to transmit */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = line status */
}



/* Int 14h / 02h - Receive Character from Serial Port */

unsigned int comRx( unsigned int port )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - receive character */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = line status, AL = character */
}



/* Int 14h / 03h - Read Serial Port Status */

unsigned int comReadStatus( unsigned int port )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - read serial port status */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = line status, AL = modem status */
}



/* Int 14h / 04h - Extended Initialize Serial Port */

unsigned int comExtInit( unsigned int port, 
                         unsigned char breakCode, 
                         unsigned char parity, 
                         unsigned char stopBits, 
                         unsigned char wordLgt, 
                         unsigned char baudRate )
{
union REGS regs;

regs.h.ah = 4;                /* AH = 4 - extended initialize */
regs.h.al = breakCode;        /* AL = new break code */
regs.h.bh = parity;           /* BH = new parity setting */
regs.h.bl = stopBits;         /* BL = number of stop bits */
regs.h.ch = wordLgt;          /* CH = new word length */
regs.h.cl = baudRate;         /* CL = new baud rate */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AH = line status, AL = modem status */
}



/* Int 14h / 05h - Extended Modem Control */

unsigned int comModemCtl( unsigned int port, 
                          unsigned char rwCode, 
                          unsigned char *modemCtlReg )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - extended modem control */
regs.h.al = rwCode;           /* AL = read/write code */
regs.h.bl = *modemCtlReg;     /* BL = new port control value */
regs.x.dx = port;             /* DX = port number */
int86( 0x14, &regs, &regs );  /* Call BIOS */
*modemCtlReg = regs.h.bl;     /* BL = modem control value */
return( regs.x.ax );          /* AH = line status, AL = modem status */
}
