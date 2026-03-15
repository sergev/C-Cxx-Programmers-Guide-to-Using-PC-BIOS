/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-3.  Read Real-Time Clock Time
*/


/* Read BCD time from RTC and convert it to integers */
void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec )
{
#define tensDigit( bcdValue ) ( (bcdValue >> 4) * 10 )
#define onesDigit( bcdValue ) ( bcdValue & 15 )

union REGS regs;

regs.h.ah = 2;                     /* AH = 2 - return RTC time */
int86( 0x1a, &regs, &regs );       /* Call BIOS */

                                   /* CH = current hour in BCD */
*hour = tensDigit( regs.h.ch );    /* Convert tens digit */
*hour += onesDigit( regs.h.ch );   /* Add ones digit */

                                   /* CL = current minute in BCD */
*min = tensDigit( regs.h.cl );     /* Convert tens digit */
*min += onesDigit( regs.h.cl );    /* Add ones digit */

                                   /* DH = current second in BCD */
*sec = tensDigit( regs.h.dh );     /* Convert tens digit */
*sec += onesDigit( regs.h.dh );    /* Add ones digit */
}
