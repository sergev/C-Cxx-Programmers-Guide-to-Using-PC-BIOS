/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-7.  Keyboard Delay and Repeat Rate
*/


/*
   KBDSPEED.C 
   Adjusts keyboard delay and repeat rate
   based on command line values

   To run, type KBDSPEED <delay> <repeat rate>
      Delay is 0 (fastest) to 3 (slowest).
      Repeat rate is 0 (fastest) to 31 (slowest).
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void main( int argc, char *argv[] )
{
union REGS regs;
char delay, repeatRate;

#define MIN_DELAY   0        /* Delay must be 0<=n<=3 */
#define MAX_DELAY   3
#define MIN_REPEAT  0        /* Repeat rate must be 0<=n<=31 */
#define MAX_REPEAT 31

/* Warn user if invalid command line */
if ( argc != 3 )
{
   printf( "Syntax: %s <delay> <repeat rate>\n", argv[0] );
   printf( "   Delay = 0 (fastest) to 3 (slowest)\n" );
   printf( "   Rate  = 0 (fastest) to 31 (slowest)\n\7" );
   exit( 1 );
   }

/* Get and validate delay value */
delay = atoi( argv[1] );
if( delay > MAX_DELAY )
{
   printf( "Delay must be between %u and %u!\n\7",
           MIN_DELAY, MAX_DELAY );
   exit( 1 );
   }

/* Get and validate repeat rate value */
repeatRate = atoi( argv[2] );
if( repeatRate > MAX_REPEAT )
{
   printf( "Repeat rate must be between %u and %u!\n\7",
           MIN_REPEAT, MAX_REPEAT );
   exit( 1 );
   }

/* Set delay and repeat rate */
regs.h.ah = 3;                /* AH = 3 - set repeat rate and delay */ 
regs.h.al = 5;                /* AL = 5 (because BIOS wants it!) */ 
regs.h.bh = delay;            /* BH = delay */ 
regs.h.bl = repeatRate;       /* BL = repeat rate */ 
int86( 0x16, &regs, &regs );  /* Call BIOS */ 

printf( "Done" );
exit( 0 );
}
