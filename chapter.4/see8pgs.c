/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-3.  Write to Each Text-Mode Video Page
*/


/*
   SEE8PGS.C
   Writes to all 8 video pages and then 
   switches to each in turn
*/

#include <dos.h>
#include <stdio.h>
#include <conio.h>

#define ESCAPE 27                /* Code for Esc key */

void makeVideoPageActive( unsigned char );

void main( void )
{
int page;

/* Write message to each video page */
for( page=0; page<8; page++ )
{
   makeVideoPageActive( page );
   printf( "\n\nThis is video page %u", page );
   printf( "  \nPress Enter to continue, or Esc to quit" );
   }

/* Loop displaying pages until user says stop */
for( page=0; ; page++ )          /* Start with page 0 */
{
   if ( page == 8 )              /* When page == 8 */
      page = 0;                  /* start over with page 0 */

   makeVideoPageActive( page );  /* Activate page */

/* Page is visible and you can read its message */
   if ( getch() == ESCAPE )
      break;                     /* Quit if user presses Escape */
   }                             /* and continue looping */

makeVideoPageActive( 0 );        /* Restore default page (0) */
}

/* Activates specified video page */
void makeVideoPageActive( unsigned char whichPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - set active video page */
regs.h.al = whichPage;        /* AL = page (0...7) to activate */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}
