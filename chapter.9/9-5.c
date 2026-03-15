/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-5.  Set Real-Time Clock Time
*/


/* Convert time integers to BCD and set Real-Time Clock.
   Values are expressed in (and validated for) 24-hour format.
   Returns 0 = success, or 1 = invalid parameter specified.
*/

unsigned char setRTCTime( char hour, 
                          char minute, 
                          char second )
{
union REGS regs;

if( (hour > 23) || (minute > 59) || (second > 59) )
   return( 1 );                     /* Return 1 = invalid parameter */

regs.h.ah  = 3;                     /* AH = 3 - set RTC time */

                                    /* CH = new hour in BCD */
regs.h.ch  = ( hour / 10 ) << 4;    /* Convert tens digit */
regs.h.ch |= ( hour % 10 );         /* Convert ones digit */

                                    /* CL = new minute in BCD */
regs.h.cl  = ( minute / 10 ) << 4;  /* Convert tens digit */
regs.h.cl |= ( minute % 10 );       /* Convert ones digit */

                                    /* DH = new second in BCD */
regs.h.dh  = ( second / 10 ) << 4;  /* Convert tens digit */
regs.h.dh |= ( second % 10 );       /* Convert ones digit */

                                    /* DL = 0 for standard time  or */
                                    /*      1 for Daylight Savings Time */
regs.h.dl  = 0;                     /* Set standard time */

int86( 0x1a, &regs, &regs );        /* Call BIOS */
return( 0 );                        /* Return 0 = success */
}
