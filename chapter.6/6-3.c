/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 6-3.  Read or Set Serial Port Timeout
*/ 


/*
   readSerialPortTimeout() reads BDA serial port timeout
   setSerialPortTimeout() sets BDA serial port timeout
*/

#define com1Timeout (unsigned char far *)0x0040007c
#define com2Timeout (unsigned char far *)0x0040007d
#define com3Timeout (unsigned char far *)0x0040007e
#define com4Timeout (unsigned char far *)0x0040007f

/* Return specified port's timeout value */
unsigned char readSerialPortTimeout( unsigned char whichPort )
{
whichPort &= 3;
return( *(com1Timeout + whichPort) );
}

/* Set specified port's timeout value */
void setSerialPortTimeout( unsigned char whichPort,
                           unsigned char timeoutValue )
{
whichPort &= 3;
*(com1Timeout + whichPort) = timeoutValue;
}
