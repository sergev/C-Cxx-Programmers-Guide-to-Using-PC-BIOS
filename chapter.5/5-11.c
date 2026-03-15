/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-11.  Determine Installed Media Size
*/


/* 
   Determine installed diskette size.
   Returns:
      0 = drive not installed or media unknown
      1 = 360KB media
      2 = 1.2MB media
      3 = 720KB media
      4 = 1.44MB media
*/

#include <dos.h>

#define M_UNKNOWN 0
#define M_360KB   1
#define M_12MB    2
#define M_720KB   3
#define M_144MB   4

/* Function prototypes */
unsigned char readDisketteSectors( unsigned char, unsigned char, 
                                   unsigned char, unsigned char, 
                                   unsigned char, unsigned char far * );
unsigned char resetDrive( unsigned char );

/* Determine media size */
unsigned char getMediaSize( unsigned char drive )
{
#define ONE_SECTOR      1
#define HEAD_0          0
#define TRACK_0         0
#define SECTOR_1        1
#define SECTOR_9        9
#define SECTOR_18      18

#define driveA (unsigned char far *)0x00400090
#define driveB (unsigned char far *)0x00400091

unsigned char mediaStatus;
char diskBuffer[512];

/* Dummy read to reset BDA */
readDisketteSectors( drive, ONE_SECTOR, HEAD_0, 
                      TRACK_0, SECTOR_1, diskBuffer );

/* Read again - first one may fail due to change line signal */
resetDrive( drive );
if ( readDisketteSectors(drive, ONE_SECTOR, HEAD_0, 
                           TRACK_0, SECTOR_1, diskBuffer) != 0 )
   return( M_UNKNOWN );  /* Any error this time is real */

if ( drive == 0 )            /* If drive A, */
   mediaStatus = *driveA;    /* read type from 0x00400090 */
else                         /* Else */
   mediaStatus = *driveB;    /* read drive B type from 00400091 */

mediaStatus &= 7;            /* Mask extraneous bits */

/* Return information for 5¬" media */
   switch ( mediaStatus )
   {
      case 0:   /* 360KB media in 360KB drive - not verified */
      case 1:   /* 360KB media in 1.2MB drive - not verified */
      case 2:   /* 1.2MB media in 1.2MB drive - not verified */
         return( M_UNKNOWN );              /* Unknown media! */

      case 3:   /* 360KB media in 360KB drive - verified     */
      case 4:   /* 360KB media in 1.2MB drive - verified     */
         return( M_360KB );

      case 5:   /* 1.2MB media in 1.2MB drive - verified     */
         return( M_12MB );
      }

/* 
   It is 3« inch media - now see if it is 720KB or 1.44MB
   by reading sectors 9 and 18 at head 0 track 0.
*/
if ( readDisketteSectors(drive, ONE_SECTOR, HEAD_0, 
                           TRACK_0, SECTOR_9, diskBuffer) != 0 )
   return( M_UNKNOWN );      /* Read failed - no idea what it is */

if ( readDisketteSectors(drive, ONE_SECTOR, HEAD_0, 
                           TRACK_0, SECTOR_18, diskBuffer) == 0 )
   return( M_144MB );        /* Read 18th sector, so it is 1.44MB */
return( M_720KB );           /* Only read 9th sector, so it is 720KB */
}

/* Reset drive and controller */
unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset drive */
regs.h.dl = drive;            /* DL = drive to reset (0 or 1) */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = 0 and Carry false if success, or */
}                             /* AH = code and Carry true if error */

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
