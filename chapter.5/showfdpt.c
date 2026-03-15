/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-14.  Display Fixed Disk Parameter Table
*/


/*
   SHOWFDPT.C
   Displays Fixed Disk Parameter Table (FDPT).  Shows 
   number of entries specified on command line, 
   or 47 if no limit specified.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_DRIVE   47
#define ONE_MILLION 1048576

/* 
   This structure defines the format of Fixed Disk
   Parameter Table entries.  Be sure to compile with 
   structure packing ON.
*/
struct FDPT_entry
{
   unsigned int  cylinders;
   unsigned char heads;
   unsigned char reserved1[2];
   unsigned int  precompCylinder;
   unsigned char maxEccBurst;
   unsigned char retryOnError       :2;
   unsigned char defectMapPresent   :1;
   unsigned char reserved2          :1;
   unsigned char moreThan8Heads     :1;
   unsigned char reserved3          :3;
   unsigned char reserved4[3];
   unsigned int  landingZone;
   unsigned char sectorsPerTrack;
   unsigned char reserved5;
   } far *FDPT = (struct FDPT_entry far *)0xf000e401;

char msg1[] =
   "Drive |---Number of--| Precomp. Max Ecc Landing  Sectors",
msg2[] = 
   "Type  Cylinders  Heads Cylinder  Burst   Zone   Per Track  Capacity",
msg3[] =
   "===== =========  ===== ========  =====  ======= =========  ========";

#define template \
   " %.2u     %.4Fu       %.2Fu   %.5Fu\
     %.2FX     %.4Fu      %.2Fu       %uMB\n"

void main( int argc, char *argv[] )
{
unsigned int capacity;
float temp;
int entriesToDisplay,
    driveType = 1;

if ( argc > 1 )                          /* If limit specified */
   entriesToDisplay = atoi( argv[1] );   /*    use it */
else                                     /* Otherwise, */
   entriesToDisplay = MAX_DRIVE;         /*    use default value */

/* Display column headings */
printf( "%s\n%s\n%s\n", msg1, msg2, msg3 );

while( entriesToDisplay )
{
/* Drive type 15 is reserved */
   if ( driveType == 15 )
      printf( " %.2u - Reserved\n", driveType );
   else
   {
/* 
   Compute drive capacity using this formula: 
   MBytes = (cylinders*heads*sectorsPerTrack*bytesPerSector)/ONE_MILLION
*/
      temp  = FDPT->cylinders;
      temp *= FDPT->heads;
      temp *= FDPT->sectorsPerTrack;
      temp *= 512;
      temp /= ONE_MILLION;
      capacity = temp;              /* Copy float result to an integer */

      printf( template,             /* Display table entry */
              driveType,
              FDPT->cylinders,
              FDPT->heads,
              FDPT->precompCylinder,
              FDPT->maxEccBurst,
              FDPT->landingZone,
              FDPT->sectorsPerTrack,
              capacity );
      }
/* Repeat loop until all entries displayed */
   --entriesToDisplay;
   ++driveType;
   ++FDPT;
   }
}
