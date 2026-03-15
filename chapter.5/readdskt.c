/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-10.  Read Diskette and Report Media Errors
*/


/*
   READDSKT.C
   Read entire diskette (of known size) by track and 
   report errors that persist after a few retries
*/

#include <dos.h>
#include <stdio.h>

/* Function prototypes */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );
unsigned char resetDrive( unsigned char );

#define LASTHEAD         1      /*   2 heads (0 and 1) */
#define LASTTRACK       79      /*  80 tracks (0 - 79) */
#define LASTSECTOR      15      /*  15 sectors/track (1 - 15) */
#define BYTESPERSECTOR 512      /* 512 bytes per sector */
#define driveToRead      0      /* Drive A = 0 */
#define MAXRETRIES       3      /* Times to retry a failed read */
#define NO_ERROR         0      /* Returned from interrupt 13h */

char diskBuffer[ BYTESPERSECTOR * LASTSECTOR ];

void main( void )
{
unsigned int currentHead, 
             currentTrack;
unsigned char errorCode;
int retryCount;

for( currentTrack = 0; 
        currentTrack <= LASTTRACK; 
           currentTrack++ )
{
   for( currentHead = 0; 
           currentHead <= LASTHEAD; 
              currentHead++ )
   {
      for( retryCount = 0; 
              retryCount < MAXRETRIES; 
                 retryCount++ )
      {
         errorCode = readDisketteSectors( driveToRead,
/* Read 15 sectors */                     LASTSECTOR,
                                          currentHead,
                                          currentTrack, 
/* Starting at sector 1 */                1,
                                          diskBuffer );
         if ( errorCode == NO_ERROR )
            break;

         resetDrive( driveToRead );
         }

      if ( errorCode != NO_ERROR )
         printf( "Error reading head %u cylinder %u\n\7",
                 currentHead, currentTrack );
      }
   }
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
regs.h.ch = track;             /* CH = track */
regs.h.cl = sector;            /* CL = sector number */
sregs.es  = FP_SEG(bufferPtr); /* ES = buffer segment */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Call BIOS */
return( regs.h.ah );           /* AH = 0 or error code */
}

/* Reset drive and controller */
unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset drive */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = 0 and Carry false if success, or */
}                             /* AH = error code and Carry true if error */
