/* 
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-1.  Display System ROM Fields
*/


/*
   ROM-INFO.C
   Display ROM manufacturer fields in segment F000
*/

#include <stdio.h>
#include <string.h>

#define eisaSig      (unsigned char far *) 0xf000ffd9
#define romFamily    (unsigned char far *) 0xf000ffe4
#define romRelease   (unsigned char far *) 0xf000ffe5
#define romRev       (unsigned char far *) 0xf000ffe6
#define romType      (unsigned char far *) 0xf000ffe8
#define manufacturer (unsigned char far *) 0xf000ffea
#define romDate      (unsigned char far *) 0xf000fff5
#define pcType       (unsigned char far *) 0xf000fffe

char eisa[]    = "EISA",
     nonEisa[] = "ISA or PS/2",
    *architecture;

void main( void )
{
static unsigned char far *EISA_SIG = eisaSig;
int i;

/* Determine whether machine is EISA or ISA */
for( i = 0; i < strlen( eisa ); i++ )
{
   if ( EISA_SIG[i] != eisa[i] )
      break;
   }

if ( i != strlen( eisa ) )
   architecture = nonEisa;
else
   architecture = eisa;

printf( "\nSystem ROM Identification" );
printf( "\nManufacturer  : %.6Fs",   manufacturer );
printf( "\nFamily code   : %Fc",     *romFamily );
printf( "\nVersion       : %Fc.%Fc", *romRev, *romRelease );
printf( "\nROM type      : %.2Fs",   romType );
printf( "\nCreation date : %.8Fs",   romDate );
printf( "\nSystem type   : %.2FX",   *pcType );
printf( "\nArchitecture  : %s",      architecture );
}
