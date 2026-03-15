/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 9-2.  Calculate Program Execution Time
*/


/*
   TIMEPGM.C
   Uses system tick counter to calculate time 
   needed to execute a specified command line
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define TICKS_PER_SECOND 18.2
#define TICKS_PER_DAY 1573040L

unsigned char readTickCounter( unsigned long * );

void main( int argc, char *argv[] )
{
float elapsedSeconds;
unsigned long startTick, endTick, elapsedTicks;

/* Error if no parameters */
if ( argc != 2 )
{
   printf( "Syntax is %s \"command line\"\7",argv[0] );
   exit( 1 );
   }

readTickCounter( &startTick );    /* Get tick counter          */

system( argv[1] );                /* Run user's command line   */

readTickCounter( &endTick );      /* Get tick counter again    */

if ( endTick < startTick )        /* If clock rolled over,     */
   endTick += TICKS_PER_DAY;      /* adjust ending counter     */

elapsedTicks = endTick-startTick; /* Calculate elapsed ticks   */

                                  /* Calculate elapsed seconds */
elapsedSeconds = elapsedTicks / TICKS_PER_SECOND;

printf( "\nCommand completed in %f seconds.", elapsedSeconds );
exit( 0 );
}

/* Return system tick counter and midnight rollover flag */
unsigned char readTickCounter( unsigned long *retval )
{
union REGS regs;
unsigned long result;

regs.h.ah = 0;                /* AH = 0 - read tick counter */
int86( 0x1a, &regs, &regs );  /* Call BIOS */

result = regs.x.cx;           /* CX = upper 16 bits */
result <<= 16;                /* Shift them into place */
result |= regs.x.dx;          /* DX = lower 16 bits */
*retval = result;             /* Copy 32 bits to caller's long */

return( regs.h.al );          /* AL = rollover flag */
}
