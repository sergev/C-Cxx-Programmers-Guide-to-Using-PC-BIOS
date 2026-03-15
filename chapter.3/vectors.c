/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-4.  Read Interrupt Vector Table
*/


/*
   VECTORS.C 
   Display interrupt vector table contents
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define IRET     0xcf         /* IRET machine instruction    */
#define ROMSEG 0xf000         /* System ROM starting segment */
#define OPTROM 0xa000         /* Option ROM starting segment */

unsigned int intNr,
             segment,
             offset;

/* vect is a pointer into the interrupt vector table */
unsigned char far * far *vect = NULL;

void main( void )
{
printf( "\n            Vector       Vector      Type of" );
printf( "\nInterrupt   Address     Contents     Handler" );
printf( "\n________________________________________________" );

for( intNr=0; intNr!=256; intNr++ )
{
   segment = FP_SEG( *vect );
   offset  = FP_OFF( *vect );

   printf( "\n  %.2Xh     0000:%.4Xh   %.4X:%.4Xh   ",
            intNr,
            intNr * 4,
            segment,
            offset );

   if ( *vect == NULL )
      printf( "Unused" );
   else
   {
      if ( (intNr==0x1d) || (intNr==0x1e) || 
           (intNr==0x1f) || (intNr==0x41) || 
           (intNr==0x43) || (intNr==0x46) )
         printf( "Pointer to a Table" );
      else
      {
         if ( **vect == IRET )
            printf( "Do Nothing" );
         else
         {
            if ( segment >= ROMSEG )
               printf( "System ROM" );
            else
            {
               if ( segment >= OPTROM )
                  printf( "Option ROM" );
               else
                  printf( "Base Memory" );
               }
            }
         }
      }
   ++vect;
   }
exit( 0 );
}
