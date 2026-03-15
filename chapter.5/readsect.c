/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-5.  Read Entire Diskette by Sector
*/


/*
   READSECT.C
   Read entire diskette (of known size) by sector
*/

#include <dos.h>

/* Function prototype */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );

#define LASTHEAD         1          /*   2 heads (0 and 1) */
#define LASTTRACK       79          /*  80 tracks (0 - 79) */
#define LASTSECTOR      15          /*  15 sectors/track (1 - 15) */
#define BYTESPERSECTOR 512          /* 512 bytes per sector */
#define driveToRead      0          /* Drive A = 0 */
#define ONE_SECTOR       1          /* Read only 1 sector at a time */
char diskBuffer[ BYTESPERSECTOR ];  /* Allocate disk buffer */

void main( void )
{
unsigned char currentHead, 
              currentTrack, 
              currentSector;

for( currentTrack = 0; 
        currentTrack <= LASTTRACK; 
           currentTrack++ )
   for( currentHead = 0; 
           currentHead <= LASTHEAD; 
              currentHead++ )
      for( currentSector = 1; 
              currentSector <= LASTSECTOR; 
                 currentSector++ )
         readDisketteSectors( driveToRead,
                              ONE_SECTOR,
                              currentHead,
                              currentTrack, 
                              currentSector, 
                              diskBuffer );
}

/*
   Read sectors from diskette.
   Returns 0 or an interrupt 13h error code.
*/
unsigned char readDisketteSectors( unsigned char drive, 
                                   unsigned char sectorsToRead, 
                                   unsigned char head, 
                                   unsigned char track,
                                   unsigned char sector, 
                                   unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 2;                 /* AH = 2 - read sectors */
regs.h.al = sectorsToRead;     /* AL = number of sectors to read */
regs.x.bx = FP_OFF(bufferPtr); /* BX = buffer offset */
regs.h.dh = head;              /* DH = head */
regs.h.dl = drive;             /* DL = drive number */
regs.h.ch = track;             /* CH = track number */
regs.h.cl = sector;            /* CL = sector number */
sregs.es  = FP_SEG(bufferPtr); /* ES = buffer segment */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Call BIOS */
return( regs.h.ah );           /* AH = 0 or error code */
}
