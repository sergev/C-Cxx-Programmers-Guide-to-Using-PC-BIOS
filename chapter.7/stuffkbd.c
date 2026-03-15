/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-9.  Write String into Typeahead Buffer
*/


/*
   STUFFKBD.C
   Loads characters into typeahead buffer.
   To run, type STUFFKBD "key string".
   Recognizes \E as Enter key, and \T as Tab key.
*/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define TAB    9                    /* Tab Key   */
#define ENTER 13                    /* Enter Key */

union REGS regs;

void main( int argc, char *argv[] )
{
char *pCh;

/* Make sure command line is complete */
if( argc == 1 )
   printf( "Syntax: %s \"keysToStuff\"\7",argv[0] );
else
{
   pCh = argv[1];
/* Load each character from command line string */
   do
   {
/* Let user specify \E for Enter and \T for Tab */
      if ( *pCh == '\\' )
      {                                /* Found a slash */
         ++pCh;                        /* Look at next character */
         if ( *pCh == 0 )              /* If string ended early, */
            break;                     /*    exit loop */
         switch ( *pCh )
         {
/* If character is \E or \e, buffer an Enter key */
            case 'e' :
            case 'E' :
               regs.h.cl = ENTER;
               break;
/* If character is \T or \t, buffer a Tab key */
            case 't' :
            case 'T' :
               regs.h.cl = TAB;
               break;
/* Anything else, just buffer character following the slash */
            default  :
               regs.h.cl = *pCh;
               break;
            }
         }
/* Character is not a slash - add it to buffer */
      else
         regs.h.cl = *pCh;

/* Call BIOS to write character to typeahead buffer */
      regs.h.ah = 5;      /* Interrupt 16h function 5 */
      regs.h.ch = 0;      /* Scan code = 0 (normal character) */
      int86( 0x16, &regs, &regs );

/* Alert user if buffer is full */
      if ( regs.h.al != 0 )
      {
         printf( "Sorry, the typeahead buffer is full!\7" );
         break;
         }

/* Point to next character and continue until end-of-string */
      ++pCh;
      } while( *pCh != 0 );
   }
}
