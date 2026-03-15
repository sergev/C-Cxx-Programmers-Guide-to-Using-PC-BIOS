/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-4.  Print File until a Specified Key Is Pressed
*/


/*
   STOPPRNT.C
   Print file on LPT1, stopping if user presses S or s.
   Syntax is: STOPPRNT <file to print>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>
#include <errno.h>

#define BUFFER_SIZE 81

void         getKbdChar       ( unsigned char *, 
                                unsigned char * );
unsigned int testKbdCharReady ( unsigned char *, 
                                unsigned char * );

void main( int argc, char *argv[] )
{
FILE *in, *out;
unsigned char scanCode,
              charCode,
              buffer[BUFFER_SIZE];

/* Abort if file name missing from command line */
if ( argc == 1 )
{
   printf( "Syntax: %s <file to print>\7", argv[0] );
   exit( 1 );
   }

in = fopen( argv[1], "r+t" );               /* Open input file */
if ( in == NULL )                           /* Abort if open fails */
{
   perror( "Cannot open input file " );
   exit( errno );
   }

out = fopen( "LPT1", "w+t" );               /* Open printer */
if ( in == NULL )                           /* Abort if open fails */
{
   perror( "Cannot open LPT1 " );
   fclose( in );
   exit( errno );
   }

printf( "\nPress S to stop printing." );
while( fgets( buffer, BUFFER_SIZE, in ) )   /* Loop until EOF */
{
   fputs( buffer, out );                    /* Print next record */
   if ( testKbdCharReady( &charCode, 
                          &scanCode ) )     /* Was key pressed? */
   {                                        /* Yes */
      getKbdChar( &charCode, &scanCode );   /* Read key from buffer */
      if ( toupper( charCode ) == 'S' )     /* Abort if S/s pressed */
      {
         printf( "\n*** ABORTED BY OPERATOR ***" );
         break;
         }
      }
   }

fclose( in );
fclose( out );
exit( 0 );
}

/* 
   Returns 0 if typeahead buffer is empty, 
   or >0 if data is present
*/
unsigned int testKbdCharReady( unsigned char *charCode,
                               unsigned char *scanCode )
{
unsigned char localAH, 
              localAL;
unsigned int returnValue;

/* Switch to inline assembly mode */
_asm  mov AH, 1             /* AH = 1 or 11h - check keyboard */
_asm  int 16h               /* Call BIOS */
_asm  jnz endFunc           /* Zero false means character ready */
_asm  xor AX,AX             /* Else, buffer empty - return 0 */
endFunc:                    /* Move AH and AL to local variable */
_asm  mov localAH, AH       /* AH = scan code */
_asm  mov localAL, AL       /* AL = character code */
_asm  mov returnValue, AX   /* Save 16-bit composite for return code */

/* Leave inline assembly mode and finish the call in C */
*scanCode = localAH;        /* Copy scan code to caller's var */
*charCode = localAL;        /* Copy char code to caller's var */

return( returnValue );      /* Return >0 if data is ready */
}

/* 
   Returns next scan code / character code from 
   typeahead buffer.
*/
void getKbdChar( unsigned char *charCode,
                 unsigned char *scanCode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 or 10h - read keyboard */
int86( 0x16, &regs, &regs );  /* Call BIOS */

*scanCode = regs.h.ah;        /* AH = scan code */
*charCode = regs.h.al;        /* AL = character code */
}                             /* Returns nothing */
