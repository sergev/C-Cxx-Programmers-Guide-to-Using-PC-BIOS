/* *************************************************************
   BADSPOT.C
   Copyright (c) 1992 - Billy P. Taylor
   *************************************************************
   Demonstrates interrupt 13h functions that work on either 
   hard disks or diskettes.  It locates unreadable sectors by 
   reading the disk by track.  When it finds a problem, it
   rereads the track by sector to identify the damaged areas.
   It also shows how to differentiate between diskette and 
   hard disk to determine media capacity.

   To run, type BADSPOT <drive letter or number>.
   Diskette drive letters are A...D.
   Hard drive numbers are 80...83.
   ********************************************************** */

/* ******** DEFINES and MACROS ********* */
#include <ctype.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define DRIVE_DOOR_OPEN 6   /* BIOS change line error */
#define MAX_BPS       512   /* Maximum bytes/sector */
#define MAX_DSKT_NUMBER 3   /* Largest diskette drive number */

#define CARRIAGE_RETURN 13

/* Exit (ERRORLEVEL) values */
#define ALL_SECTORS_READABLE 0
#define DISK_HAS_BAD_SECTORS 1
#define BAD_COMMAND_LINE     2
#define BAD_DRIVE_SPECIFIED  3
#define INSUFFICIENT_MEMORY  8

/* ********  Global Data  ******** */
unsigned int readSize,
             maxHead,
             maxSector,
             maxTrack,
             head,
             track,
             sector,
             returnCode = ALL_SECTORS_READABLE;

unsigned char oneSector[MAX_BPS];
unsigned char far *diskBuffer = oneSector;

struct errorMessage
{
   unsigned char errorCode;
   unsigned char *message;
   } errorMessages[] =
{
   { 0x01, "Illegal command" },
   { 0x02, "Address mark not found" },
   { 0x04, "Sector not found" },
   { 0x05, "Drive reset failed" },
   { 0x07, "Drive parameter command failed" },
   { 0x09, "Transfer across 64K boundry attempted" },
   { 0x0a, "Bad block flag read" },
   { 0x10, "Uncorrectable ECC error encountered" },
   { 0x11, "Correctable ECC error encountered" },
   { 0x20, "Controller failure" },
   { 0x40, "Seek failed" },
   { 0x80, "Drive does not respond" },
   { 0xaa, "Drive not ready" },
   { 0xbb, "Unknown error" },
   { 0xcc, "Write fault" },
   { 0xff, "Sense command failed" },
   {    0, "Unrecognized error code!" }
   };

/* ********  Function Prototypes  ******** */
unsigned char findMediaSize         ( int,
                                      unsigned int *,
                                      unsigned int *,
                                      unsigned int *);
boolean       readSourceDiskette    ( void );
unsigned int  readSpot              ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int);
void          resetDrive            ( int );
void          showDiskErrorMessage  ( unsigned int );

/* ********  MAIN  ******** */
void main( int argc, char *argv[] )
{
int drive;
boolean driveOk;
unsigned int errorCode;

printf( "BADSPOT : Find bad sectors on diskette or hard disk\n" );

/* Get drive letter or number from command line.  Note 
   that diskette drives are specified by letter (for
   convenience), so we must convert them to binary.
*/
driveOk = NO;
if ( argc >= 2 )
{
   if ( isalpha( *argv[1] ) )
   {
/* Convert diskette drive letter to drive number and validate it */
      drive = toupper( *argv[1] ) - 'A';
      if ( drive <= MAX_DSKT_NUMBER )
         driveOk = YES;
      }
   else
   {
/* Convert ASCII decimal number to binary */
      drive = atoi( argv[1] );
/* Only allow values 80...83 */
      if ( (drive >= 80) && (drive <= 83) )
      {
         drive -= 80;       /* Convert to base 0 drive number */
         drive += 0x80;     /* Set high order bit for hard drives */
         driveOk = YES;
         }
      }
   }

if ( !driveOk )
{
   printf( "\nSyntax: %s <drive letter or number>", argv[0] );
   printf( "\n        Diskette drives are A...D." );
   printf( "\n        Hard drives are 80...83.\7" );
   exit( BAD_COMMAND_LINE );
   }

/* Read drive parameters */
errorCode = findMediaSize( drive, &maxHead, &maxTrack, &maxSector );
if ( errorCode )
{
   showDiskErrorMessage( errorCode );
   resetDrive( drive );
   exit( BAD_DRIVE_SPECIFIED );
   }

printf( "\n\tDrive %Xh capacity is %u heads, %u tracks, %u sectors/track\n",
        drive,
        maxHead+1,
        maxTrack+1,
        maxSector);

/* Allocate track-size disk buffer */
readSize = maxSector * MAX_BPS;
diskBuffer = malloc( readSize );
if ( diskBuffer == NULL )
{
   perror( "ERROR ALLOCATING MEMORY FOR DISK BUFFER : \7" );
   exit( INSUFFICIENT_MEMORY );
   }

/* Read drive by track */
for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      printf( "%c\tReading %u sectors at head %u track %u sector 1   ",
              CARRIAGE_RETURN,
              maxSector,
              head,
              track);

      errorCode = readSpot( drive, head, track, 1, maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( drive );
         printf( "\n" );

/* If track read fails, reread by sector */
         for( sector = 1; sector <= maxSector; sector++ )
         {
            printf( "%cRereading 1 sector at head %u track %u sector %u",
                    CARRIAGE_RETURN,
                    head,
                    track,
                    sector);
            errorCode = readSpot( drive, head, track, sector, 1 );

/* Report unreadable sectors */
            if ( errorCode )
            {
               showDiskErrorMessage( errorCode );
               returnCode = DISK_HAS_BAD_SECTORS;
               printf( "\n" );
               resetDrive( drive );
               }
            }
         printf( "\n" );
         }
      }
   }
exit( returnCode );
}

/* ********  Physical Disk I/O Functions  ******** */
/* Reset drive and controller */
void resetDrive( int driveNumber )
{
union REGS regs;

regs.h.ah = 0;                   /* AH = 0 - reset drive and controller */
regs.h.dl = driveNumber;         /* DL = drive number */
int86( 0x13, &regs, &regs );     /* Call BIOS */
}

/* Read sectors.
   Return 0 = success, else int 13h error code 
*/
unsigned int readSpot( int drive,
                       unsigned int head,
                       unsigned int cylinder,
                       unsigned int sector,
                       unsigned int sectorsToRead )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *bufferPtr;
unsigned int sectorMask;
boolean secondTry;

bufferPtr = diskBuffer;
secondTry = NO;

while(1)
{
   sectorMask  = ( cylinder >> 2);     /* Get cylinder number high 2 bits */
   sectorMask &= ( 128 + 64 );         /* Zero other bits */
   sectorMask += sector;               /* Add sector number to low 6 bits */

   regs.h.ah   = 2;                    /* AH = 2 - read sectors */
   regs.h.al   = sectorsToRead;        /* AL = number to read */
   regs.h.ch   = cylinder;             /* CH = cylinder low 8 bits */
   regs.h.cl   = sectorMask;           /* CL = sector/cylinder high 2 bits */
   regs.h.dh   = head;                 /* DH = head */
   regs.h.dl   = drive;                /* DL = drive */
   regs.x.bx   = FP_OFF( bufferPtr );  /* BX = data buffer offset */
   sregs.es    = FP_SEG( bufferPtr );  /* ES = data buffer segment */

   int86x( 0x13, &regs, 
            &regs, &sregs );           /* Call BIOS */

   if ( regs.x.cflag == 0 )            /* Carry false = success */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );             /* Return if non-recoverable error */

   secondTry = YES;
   }

return( 0 );
}

/* Display message for int 13h error code */
void showDiskErrorMessage( unsigned int errorCode )
{
int i;

printf( "\7\nERROR %u : ",errorCode );

i = 0;
while( (errorMessages[i].errorCode != errorCode) &&
       (errorMessages[i].errorCode != 0) )
   i++;

printf( "%s", errorMessages[i].message );
}

/* Determine drive parameters.
   Return 0 = success, else int 13h error code
*/
unsigned char findMediaSize( int drive,
                             unsigned int *maxHead,
                             unsigned int *maxTrack,
                             unsigned int *maxSector )
{
#define CMOS_DRIVE_AB      16    /* CMOS RAM index for drive A/B type */
#define FIRST_HARD_DRIVE 0x80
#define HEAD_0              0
#define ONE_SECTOR          1
#define SECTOR_1            1
#define SECTOR_9            9
#define SECTOR_18          18
#define TRACK_0             0
#define TYPE_720KB          3    /* CMOS RAM bit mask for 720KB media */
#define TYPE_144MB          4    /* CMOS RAM bit mask for 1.44MB media */

#define driveA (unsigned char far *)0x00400090
#define driveB (unsigned char far *)0x00400091

#define CMOS_CMD  0x70
#define CMOS_DATA 0x71

union REGS regs;

unsigned char mediaStatus, 
              driveType;

unsigned int errorCode;

if ( drive >= FIRST_HARD_DRIVE )  /* for hard drives */
{
/* Test whether drive exists */
   regs.h.ah = 0x10;              /* AH = 10h - test drive ready */
   regs.h.dl = drive;             /* DL = drive number */
   int86( 0x13, &regs, &regs );   /* Call BIOS */
   if ( regs.x.cflag )            /* Carry true == error */
      return( regs.h.ah );        /* AH = error code */

/* Read hard drive parameters from BIOS */
   regs.h.ah = 8;                 /* AH = 8 - read drive parameters */
   regs.h.dl = drive;             /* DL = drive number */
   int86( 0x13, &regs, &regs );   /* Call BIOS */
   if ( regs.x.cflag )            /* Carry true == error */
      return( regs.h.ah );        /* AH = error code */

   *maxHead  = regs.h.dh;         /* DH = number of heads */
                                  /* CL = maximum cylinder high 2 bits */
                                  /*      sectors/track in low 6 bits */

   *maxSector  = regs.h.cl & 63;  /* Sectors in low 6 bits */
   *maxTrack = regs.h.cl;         /* Save cylinder number high 2 bits */
   *maxTrack &= ( 128 + 64 );     /* Zero other bits */
   *maxTrack <<= 2;               /* Shift bits into position */
   *maxTrack |= regs.h.ch;        /* Add low 8 bits */
   }
else                              /* for diskette */
{
/* Dummy read to make BIOS ID diskette */
   readSpot( drive, 0, 0, 1, 1 );
   resetDrive( drive );

/* Reread - first read may fail due to change line signal */
   errorCode = readSpot( drive, 0, 0, 1, 1 );

/* If unreadable, return error */
   if ( errorCode )
      return( errorCode );

/* Read media status from BDA */
   if ( drive == 0 )
      mediaStatus = *driveA;
   else
      mediaStatus = *driveB;

   mediaStatus &= 7;              /* Mask extraneous bits */

/* Return information for 5¬" media */
   switch ( mediaStatus )
   {
      case 0:                     /* 360KB media in 360KB drive - not verified */
      case 1:                     /* 360KB media in 1.2MB drive - not verified */
      case 2:                     /* 1.2MB media in 1.2MB drive - not verified */
         return( 4 );             /* Unknown media size !                      */

      case 3:                     /* 360KB media in 360KB drive - verified     */
      case 4:                     /* 360KB media in 1.2MB drive - verified     */
         *maxHead  = 1;
         *maxSector  = 9;
         *maxTrack = 39;
         return( 0 );

      case 5:                     /* 1.2MB media in 1.2MB drive - verified     */
         *maxHead  = 1;
         *maxSector  = 15;
         *maxTrack = 79;
         return( 0 );
      }

/* Anything else must be 3«" media.  Read drive type from CMOS RAM */
   outp( CMOS_CMD, CMOS_DRIVE_AB );
   driveType = inp( CMOS_DATA );

   if ( drive == 0 )              /* If drive is A */
      driveType >>= 4;            /*    look at upper 4 bits */
   else                           /* else */
      driveType &= 15;            /*    look at lower 4 bits */

   if ( (driveType != TYPE_720KB) &&
        (driveType != TYPE_144MB) )
      return( 4 );                /* Cannot determine media size ! */

/* Found 3«" media - determine 720KB or 1.44MB
   by reading sectors 9 and 18 at head 0 track 0.
*/
   if ( readSpot(drive, HEAD_0, TRACK_0, SECTOR_9, ONE_SECTOR) != 0 )
      return( 4 );                /* Cannot determine media size ! */

   if ( readSpot(drive, HEAD_0, TRACK_0, SECTOR_18, ONE_SECTOR) == 0 )
   {
/* Read 18th sector, so it is 1.44MB */
      *maxHead  = 1;
      *maxSector  = 18;
      *maxTrack = 79;
      }
   else
   {
/* Only read 9th sector, so it is 720KB */
      *maxHead  = 1;
      *maxSector  = 9;
      *maxTrack = 79;
      }
   }
return( 0 );
}
