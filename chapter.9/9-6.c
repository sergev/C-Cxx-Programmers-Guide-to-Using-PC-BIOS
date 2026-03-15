/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-6.  Set Real-Time Clock Date
*/


/* Convert date integers to BCD and set Real-Time Clock.
   Values are:  Month = 1 (January) to 12 (December)
                Day   = 1 to 31
                Year includes century (e.g., 1992)
   Returns 0 = success, or 1 = invalid parameter specified.
*/

unsigned char setRTCDate( int year, 
                          char month, 
                          char day )
{
static daysInMonths[] = { 00, 31,28,31,30,31,30,
                              31,31,30,31,30,31 };
union REGS regs;

/* Check date ranges */
if( (month > 12)  || (month == 0) ||
    (day > 31)    || (day == 0) )
   return( 1 );                            /* Return 1 = invalid parameter */

/* Adjust number of days in February for leap years */
daysInMonths[2] = 28;
daysInMonths[2] += ( (year%4) == 0 );

if( day > daysInMonths[month] )
   return( 1 );                            /* Return 1 = invalid parameter */

regs.h.ah = 5;                             /* AH = 5 - set RTC date */

                                           /* CH = new century in BCD */
regs.h.ch  = ( ( year / 100) / 10 ) << 4;  /* Convert tens digit */
regs.h.ch |= ( ( year / 100) % 10 );       /* Convert ones digit */

                                           /* CL = new year in BCD */
regs.h.cl  = ( (year % 100) / 10 ) << 4;   /* Convert tens digit */
regs.h.cl |= ( year % 10 );                /* Convert ones digit */

                                           /* DH = new month in BCD */
regs.h.dh  = ( month / 10 ) << 4;          /* Convert tens digit */
regs.h.dh |= ( month % 10 );               /* Convert ones digit */

                                           /* DL = new day in BCD */
regs.h.dl  = ( day / 10 ) << 4;            /* Convert tens digit */
regs.h.dl |= ( day % 10 );                 /* Convert ones digit */

int86( 0x1a, &regs, &regs );               /* Call BIOS */
return( 0 );                               /* Return 0 = success */
}
