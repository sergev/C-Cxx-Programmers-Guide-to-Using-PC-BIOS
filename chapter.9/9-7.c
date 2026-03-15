/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-7.  Start or Cancel a Countdown Timer
*/


/* Cancel timer */
void cancelTimer( void )
{
union REGS regs;

regs.h.ah = 0x83;               /* AH = 83h - start/cancel timer */
regs.h.al = 1;                  /* AL = 1 to cancel any active timer */
int86( 0x15, &regs, &regs );    /* Call BIOS */
}                               /* Returns nothing */


unsigned char doneFlag;         /* Completion flag (global data) */

/*
   Start countdown timer for n microseconds.  
   Returns 0 = timer started, or >0 = timer not 
   started because another is already running.
 */
unsigned int startTimer( unsigned long timeValue,
                         unsigned char *doneFlagP )
{
union REGS regs;
struct SREGS sregs;

*doneFlagP = 0;                        /* Flag starts clear */

regs.h.ah = 0x83;                      /* AH = 83h - start/cancel timer */
regs.h.al = 0;                         /* AL = 0 to set a timer */
regs.x.cx = (timeValue >> 16);         /* CX = upper 16 bits of timeout */
regs.x.dx = timeValue;                 /* DX = lower 16 bits of timeout */
regs.x.bx = FP_OFF(doneFlagP);         /* BX = flag offset */
sregs.es  = FP_SEG(doneFlagP);         /* ES = flag segment */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.x.cflag );                /* Return Carry flag */
}

/* 
   Macro generates 1 if specified timer has expired, 
   or 0 if still running.  Use it as follows:
      if ( timerExpired(doneFlag) )
      {
         }
*/
#define timerExpired( timerVariable ) ( timerVariable != 0 )
