/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-4.  Read Real-Time Clock Date
*/


/* Read BCD date from RTC and convert it to integers */
void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day )
{
#define tensDigit( bcdValue ) ( (bcdValue >> 4) * 10 )
#define onesDigit( bcdValue ) ( bcdValue & 15 )

union REGS regs;
int temp;

regs.h.ah = 4;                     /* AH = 4 - return RTC date */
int86( 0x1a, &regs, &regs );       /* Call BIOS */

                                   /* CH = current century in BCD */
*year = tensDigit( regs.h.ch );    /* Convert tens digit */
*year += onesDigit( regs.h.ch );   /* Add ones digit */
*year *= 100;                      /* Times 100 = century */

                                   /* CL = current year in BCD */
temp = tensDigit( regs.h.cl );     /* Convert tens digit */
temp += onesDigit( regs.h.cl );    /* Add ones digit */
*year += temp;                     /* Add to year */

                                   /* DH = current month in BCD */
*month = tensDigit( regs.h.dh );   /* Convert tens digit */
*month += onesDigit( regs.h.dh );  /* Add ones digit */

                                   /* DL = current day in BCD */
*day = tensDigit( regs.h.dl );     /* Convert tens digit */
*day += onesDigit( regs.h.dl );    /* Add ones digit */
}
