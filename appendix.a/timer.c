/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Timer Functions
*/


/* Int 15h / 83h - Set/Clear Timer */

unsigned char doneFlag;

unsigned char startTimer( unsigned long timeValue )
{
union REGS regs;
struct SREGS sregs;

unsigned char *doneFlagP;

doneFlagP = &doneFlag;
*doneFlagP = 0;                        /* Not done yet... */

regs.x.ax = 0x8301;                    /* Clear timer in case one is */
int86( 0x15, &regs, &regs );           /* active - just a safeguard */

regs.x.ax = 0x8300;                    /* AH = 0x83, AL = 0 - set timer */
regs.x.dx = timeValue;                 /* DX = low 16 bits of timer value */
regs.x.cx = (timeValue >> 16);         /* CX = high 16 bits of timer value */
sregs.es = FP_SEG(doneFlagP);          /* ES = done flag address segment */
regs.x.bx = FP_OFF(doneFlagP);         /* BX = done flag address offset */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */

if ( regs.x.cflag )                    /* Carry true = error */
   return( 0 );                        /* Return 0 = failure */

return( 1 );                           /* Return 1 = success */
}

/* Macros to test whether timer is still running */
#define TIMER_HAS_EXPIRED( flag ) ( (flag & 128) == 1 )
#define TIMER_IS_RUNNING( flag )  ( (flag & 128) == 0 )



/* Int 15h / 86h - Unconditional Wait */

unsigned char suspendProgram( unsigned long timeValue )
{
union REGS regs;

regs.h.ah = 0x86;             /* AH = 0x86 - suspend program */
regs.x.dx = timeValue;        /* DX = low 16 bits of delay value */
timeValue >>= 16;             /* Isolate upper 16 bits */
regs.x.cx = timeValue;        /* CX = high 16 bits of delay value */
int86( 0x15, &regs, &regs );  /* Call BIOS */

if ( regs.x.cflag )           /* Carry true = timer already running */
   return( 0 );               /* Return 0 = failure */

return( 1 );                  /* Return 1 = success */
}



/* Int 1Ah / 00h - Read System Tick Counter */

unsigned char readTickCounter( unsigned long *out )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - read tick counter */
int86( 0x1a, &regs, &regs );  /* Call BIOS */
*out = regs.x.cx;             /* CX = tick count high 16 bits */
*out <<= 16;                  /* Shift into position */
*out |= regs.x.dx;            /* DX = tick count low 16 bits */
return( regs.h.al );          /* AL = 24-hour rollover flag */
}



/* Int 1Ah / 01h - Set System Tick Counter */

void setTickCounter( unsigned long tickValue )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - set tick counter */
regs.x.cx = tickValue >> 16;  /* CX = tick counter high 16 bits */
regs.x.dx = tickValue;        /* DX = tick counter low 16 bits */
int86( 0x1a, &regs, &regs );  /* Call BIOS */
}



/* Int 1Ah / 02h - Read Real-Time Clock Time */

void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec )
{
union REGS regs;

regs.h.ah = 2;                    /* AH = 2 - read RTC time */
int86( 0x1a, &regs, &regs );      /* Call BIOS */

*hour = ( regs.h.ch >> 4 ) * 10;  /* CH = hour */
*hour += ( regs.h.ch & 15 );      
*min = ( regs.h.cl >> 4 ) * 10;   /* CL = minute */
*min += ( regs.h.cl & 15 );       
*sec = ( regs.h.dh >> 4 ) * 10;   /* DH = second */
*sec += ( regs.h.dh & 15 );       
}



/* Int 1Ah / 03h - Set Real-Time Clock Time */

void setRTCTime( unsigned char hour, 
                 unsigned char min, 
                 unsigned char sec )
{
union REGS regs;

regs.h.ah  = 3;                    /* AH = 3 - set RTC time */
regs.h.ch  = ( hour / 10 ) << 4;   /* CH = hour */
regs.h.ch |= ( hour % 10 );        
regs.h.cl  = ( min / 10 ) << 4;    /* CL = minute */
regs.h.cl |= ( min % 10 );         
regs.h.dh  = ( sec / 10 ) << 4;    /* DH = second */
regs.h.dh |= ( sec % 10 );         
regs.h.dl  = 0;                    /* DL = standard / Daylight Savings Time */
int86( 0x1a, &regs, &regs );       /* Call BIOS */
}



/* Int 1Ah / 04h - Read Real-Time Clock Date */

void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day )
{
union REGS regs;

regs.h.ah = 4;                     /* AH = 4 - read RTC date */
int86( 0x1a, &regs, &regs );       /* Call BIOS */

*year += ( regs.h.ch >> 4 ) * 10;  /* CH = century */
*year += ( regs.h.ch & 15 );
*year *= 100;
*year += ( regs.h.cl >> 4 ) * 10;  /* CL = year */
*year += ( regs.h.cl & 15 );

*month = ( regs.h.dh >> 4 ) * 10;  /* DH = month */
*month += ( regs.h.dh & 15 );

*day = ( regs.h.dl >> 4 ) * 10;    /* DL = day */
*day += ( regs.h.dl & 15 );
}



/* Int 1Ah / 05h - Set Real-Time Clock Date */

void setRTCDate( unsigned int year, 
                 unsigned char month, 
                 unsigned char day )
{
union REGS regs;

regs.h.ah = 5;                             /* AH = 5 - set RTC date */
regs.h.ch  = ( (year / 100) / 10 ) << 4;   /* CH = century */
regs.h.ch |= ( (year / 100) % 10 );
regs.h.cl  = ( (year % 100) / 10 ) << 4;   /* CL = year */
regs.h.cl |= ( year % 10 );
regs.h.dh  = ( month / 10 ) << 4;          /* DH = month */
regs.h.dh |= ( month % 10 );
regs.h.dl  = ( day / 10 ) << 4;            /* DL = day */
regs.h.dl |= ( day % 10 );
int86( 0x1a, &regs, &regs );               /* Call BIOS */
}



/* Int 1Ah / 06h - Set Real-Time Clock Alarm */

void setRTCAlarm( unsigned char hour, 
                  unsigned char min, 
                  unsigned char sec )
{
union REGS regs;

regs.h.ah  = 6;                    /* AH = 6 - set RTC alarm */
regs.h.ch  = ( hour / 10 ) << 4;   /* CH = hour */
regs.h.ch |= ( hour % 10 );
regs.h.cl  = ( min / 10 ) << 4;    /* CL = minute */
regs.h.cl |= ( min % 10 );
regs.h.dh  = ( sec / 10 ) << 4;    /* DH = second */
regs.h.dh |= ( sec % 10 );
int86( 0x1a, &regs, &regs );       /* Call BIOS */
}



/* Int 1Ah / 07h - Reset Real-Time Clock Alarm */

void resetRTCAlarm( void )
{
union REGS regs;

regs.h.ah = 7;                /* AH = 7 - reset RTC alarm */
int86( 0x1a, &regs, &regs );  /* Call BIOS */
}
