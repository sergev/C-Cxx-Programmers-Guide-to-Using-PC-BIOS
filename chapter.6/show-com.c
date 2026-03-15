/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-1.  Detect Serial Ports
*/


/*
   SHOW-COM.C
   Reports which serial ports are installed
*/

#include <stdio.h>

#define boolean unsigned char

#define com1Base (unsigned int far *)0x00400000
#define com2Base (unsigned int far *)0x00400002
#define com3Base (unsigned int far *)0x00400004
#define com4Base (unsigned int far *)0x00400006

boolean testSerialPortInstalled( char *, unsigned int far * );

void main( void )
{
testSerialPortInstalled( "COM1", com1Base );
testSerialPortInstalled( "COM2", com2Base );
testSerialPortInstalled( "COM3", com3Base );
testSerialPortInstalled( "COM4", com4Base );
}

/* 
   Returns 1=serial port is installed, 0=not installed.  First
   parameter is the port's name as an ASCII string.  Second 
   is a pointer to its base I/O address (in the BDA).
*/
boolean testSerialPortInstalled( char *portName,
                                 unsigned int far *ioAddress )
{
if( *ioAddress == 0 )
   printf( "\n%s : is not installed.", portName );
else
   printf( "\n%s : is installed at I/O address %.4X.",
           portName, *ioAddress );

return( ( *ioAddress != 0 ) );
}
