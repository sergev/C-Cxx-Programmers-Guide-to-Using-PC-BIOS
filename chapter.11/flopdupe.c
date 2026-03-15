/* ***************************************************
   FLOPDUPE.C
   Copyright (c) 1992 - Billy P. Taylor
   ***************************************************
   Duplicates diskettes using interrupt 13h.
   To run, type FLOPDUPE<Enter>.
   ************************************************* */

/* ******** DEFINES and MACROS ********* */
#include <conio.h>
#include <ctype.h>
#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define CARRIAGE_RETURN 13

#define MAX_BPS      512
#define MAX_SPT       32
#define MAX_DSKT_DRIVE 3

#define DRIVE_DOOR_OPEN 6

struct errorMessage
{
   unsigned char errorCode;
   unsigned char *message;
   };

/* ******  Global Data  ****** */
union REGS regs;
struct SREGS sregs;

unsigned int sourceDrive,
             readSize,
             maxHead,
             maxSector,
             maxTrack,
             head,
             track;

unsigned char diskBuffer[MAX_BPS * MAX_SPT];
unsigned char far *diskBufferP = diskBuffer;

int scratchFile;
char scratchFileName[] = "C:FlopDupe.Tmp";

struct errorMessage errorMessages[] =
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

/* ******  Function Prototypes  ****** */
boolean       createScratchFile     ( void );
unsigned char findMediaSize         ( int,
                                      unsigned int *,
                                      unsigned int *,
                                      unsigned int * );
void          makeCopies            ( void );
void          makeOneCopy           ( void );
boolean       readSourceDiskette    ( void );
unsigned int  readSpot              ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int );
void          resetDrive            ( int );
void          showDiskErrorMessage  ( unsigned int );
unsigned int  writeSpot             ( int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int,
                                      unsigned int );

void main( int argc, char *argv[] )
{
boolean driveOk;

printf("\t**** Diskette Copier ****\n");
printf("Syntax is FLOPDUPE <A...D>  -  The default drive is A\n");

driveOk = NO;

/* Default to drive A if none specified */
if ( argc == 1 )
{
   sourceDrive = 0;
   driveOk = YES;
   }
else
{
   if ( isalpha( *argv[1] ) )
   {
      sourceDrive = toupper( *argv[1] ) - 'A';
      if ( sourceDrive <= MAX_DSKT_DRIVE )
         driveOk = YES;
      }
   }

if ( !driveOk )
{
   printf( "Invalid drive specified!\7" );
   exit( 1 );
   }

if ( createScratchFile() && readSourceDiskette() )
   makeCopies();

close( scratchFile );
remove( scratchFileName );
exit( 0 );
}

/* ******  Functions Called by Main  ****** */
/* Create scratch file to hold diskette contents.
   Return 1=success, or 0=error
*/
boolean createScratchFile( void )
{
scratchFile = open( scratchFileName,
                    O_CREAT|O_RDWR|O_BINARY,
                    S_IREAD|S_IWRITE );

if ( scratchFile == -1 )
   perror( "Error creating scratch file \7" );

return( (scratchFile != -1) );
}

/* Read entire source diskette into hard disk 
   scratch file.  Return 1=success, or 0=error
*/
boolean readSourceDiskette( void )
{
unsigned int errorCode;

printf( "\nInsert source diskette in drive %c and press Enter", 
         sourceDrive+'A' );
getch();

errorCode = findMediaSize( sourceDrive,
                           &maxHead,
                           &maxTrack,
                           &maxSector );

if ( errorCode )
{
   showDiskErrorMessage( errorCode );
   resetDrive( sourceDrive );
   return( 0 );
   }

printf( "\nDrive capacity is %u heads, %u tracks, and %u sectors/track\n",
         maxHead+1,
         maxTrack+1,
         maxSector );

readSize = maxSector * MAX_BPS;

if ( readSize > (MAX_SPT*MAX_BPS) )
{
   printf( "DISK BUFFER IS TOO SMALL !\7" );
   return( 0 );
   }

for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      printf( "%cReading %u sectors at head %u track %u sector 1   ",
               CARRIAGE_RETURN,
               maxSector,
               head,
               track );

      errorCode = readSpot( sourceDrive, head, track, 1, maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( sourceDrive );
         return( 0 );
         }

      if ( write(scratchFile,diskBuffer,readSize) != readSize )
      {
         perror( "ERROR WRITING TO HARD DRIVE !\7" );
         exit( errno );
         }
      }
   }
return( 1 );
}

/* Make diskette copies */
void makeCopies( void )
{
for(;;)
{
   printf( "\nInsert formatted diskette in drive %c and press ENTER", 
            sourceDrive+'A' );
   getch();

   printf( "\n" );

   makeOneCopy();

   printf( "\nDo you want to make another copy (Y/N) ? " );

   if ( toupper(getch()) != 'Y' )
      break;

   printf( "\n" );
   }
}

/* Copy source diskette */
void makeOneCopy( void )
{
unsigned int errorCode;

lseek( scratchFile, 0L, SEEK_SET );

for( track = 0; track <= maxTrack; track++ )
{
   for( head = 0; head <= maxHead; head++ )
   {
      if ( read(scratchFile,
                diskBuffer,
                readSize) != readSize )
      {
         perror( "ERROR READING HARD DRIVE !\7" );
         return;
         }

      printf( "%cWriting %u sectors at head %u track %u sector 1   ",
               CARRIAGE_RETURN,
               maxSector,
               head,
               track );

      errorCode = writeSpot( sourceDrive,
                             head,
                             track,
                             1,
                             maxSector );
      if ( errorCode )
      {
         showDiskErrorMessage( errorCode );
         resetDrive( sourceDrive );
         return;
         }
      }
   }
}

/* ******  Physical Disk I/O Functions  ****** */
/* Reset drive and controller */
void resetDrive( int drive )
{
regs.h.ah = 0;                     /* AH = 0 - reset drive/controller */
regs.h.dl = drive;                 /* DL = drive number */
int86( 0x13, &regs, &regs );       /* Call BIOS */
}

/* Read sectors.  Return 0 = success, else BIOS error code */
unsigned int readSpot( int drive,
                       unsigned int head,
                       unsigned int cylinder,
                       unsigned int sector,
                       unsigned int size )
{                      
unsigned int sectorMask;
boolean secondTry;

secondTry = NO;
while (1)
{
/* sectorMask = high 2 bits of cylinder number + sector number */
   sectorMask  = ( cylinder >> 2) & ( 128 + 64 );
   sectorMask += sector;
   regs.h.ah   = 2;                      /* AH = 2 - read sectors */
   regs.h.al   = size;                   /* AL = number to read */
   regs.h.ch   = cylinder;               /* CH = cylinder low 8 bits */
   regs.h.cl   = sectorMask;             /* CL = cylinder high 2 bits */
                                         /*      sector in low 6 bits */
   regs.h.dh   = head;                   /* DH = head */
   regs.h.dl   = drive;                  /* DL = drive */
   regs.x.bx   = FP_OFF( diskBufferP );  /* BX = data buffer offset */
   sregs.es    = FP_SEG( diskBufferP );  /* ES = data buffer segment */
   int86x( 0x13, &regs, 
            &regs, &sregs );             /* Call BIOS */
   if ( regs.x.cflag == 0 )              /* Carry true == error */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );

   secondTry = YES;
   }

return( 0 );
}

/* Write sectors.  Return 0 = success, else BIOS error code */
unsigned int writeSpot( int drive,
                        unsigned int head,
                        unsigned int cylinder,
                        unsigned int sector,
                        unsigned int size )
{
unsigned int sectorMask;
boolean secondTry;

secondTry = NO;
while (1)
{/* sectorMask = high 2 bits of cylinder number + sector number */
   sectorMask = ( cylinder >> 2 ) & ( 128+64 );
   sectorMask += sector;
   regs.h.ah = 3;                     /* AH = 3 - write sectors */
   regs.h.al = size;                  /* AL = number to write */
   regs.h.ch = cylinder;              /* CH = cylinder low 8 bits */
   regs.h.cl = sectorMask;            /* CL = cylinder high 2 bits */
                                      /*      sector in low 6 bits */
   regs.h.dh = head;                  /* DH = head */
   regs.h.dl = drive;                 /* DL = drive number */
   regs.x.bx = FP_OFF( diskBufferP ); /* BX = data buffer offset */
   sregs.es = FP_SEG( diskBufferP );  /* ES = data buffer segment */
   int86x( 0x13, &regs, 
            &regs, &sregs );          /* Call BIOS */
   if ( regs.x.cflag == 0 )           /* Carry true == error */
      break;

   if ( (regs.h.ah != DRIVE_DOOR_OPEN) || secondTry )
      return( regs.h.ah );

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

/* Determine media size.  
   Return 0, else BIOS error code 
*/
unsigned char findMediaSize ( int drive,
                              unsigned int *maxHead,
                              unsigned int *maxTrack,
                              unsigned int *maxSector )
{
#define driveA (unsigned char far *)0x00400090
#define driveB (unsigned char far *)0x00400091

#define CMOS_DRIVE_AB  16    /* CMOS RAM index for drive A/B type */
#define HEAD_0          0
#define ONE_SECTOR      1
#define SECTOR_1        1
#define SECTOR_9        9
#define SECTOR_18      18
#define TRACK_0         0
#define TYPE_144MB      4    /* CMOS RAM bit mask for 1.44MB media */
#define TYPE_720KB      3    /* CMOS RAM bit mask for 720KB media */

unsigned char mediaStatus, 
              driveType;
unsigned int errorCode;

/* Dummy read to force BIOS to ID media */
readSpot( drive,0,0,1,1 );
resetDrive( drive );

/* Reread in case of failure due to change line signal 
*/
errorCode = readSpot( drive,0,0,1,1 );

/* If still unreadable, return error */
if ( errorCode )
   return( errorCode );

/* Read media status from BDA */
if ( drive == 0 )
   mediaStatus = *driveA;
else
   mediaStatus = *driveB;

mediaStatus &= 7;      /* Mask extraneous bits */

/* Return information for 5¬" media */
switch ( mediaStatus )
{
   case 0:                        /* 360KB media in 360KB drive - not verified */
   case 1:                        /* 360KB media in 1.2MB drive - not verified */
   case 2:                        /* 1.2MB media in 1.2MB drive - not verified */
      return( 4 );                /* Unknown media size !                      */

   case 3:                        /* 360KB media in 360KB drive - verified     */
   case 4:                        /* 360KB media in 1.2MB drive - verified     */
      *maxHead  = 1;
      *maxSector  = 9;
      *maxTrack = 39;
      return( 0 );

   case 5:   /* 1.2MB media in 1.2MB drive - verified     */
      *maxHead  = 1;
      *maxSector  = 15;
      *maxTrack = 79;
      return( 0 );
   }

/* Anything else must be 3«" media */
outp( 0x70, CMOS_DRIVE_AB );      /* Set CMOS RAM index */
driveType = inp(0x71);            /* Read byte from CMOS */

if ( drive == 0 )                 /* If drive is A */
   driveType >>= 4;               /* look at upper 4 bits */
else                              /* else */
   driveType &= 15;               /* look at lower 4 bits */

if ( (driveType != TYPE_720KB) &&
     (driveType != TYPE_144MB) )
   return( 4 );                   /* Cannot tell media size ! */

/* 
   It is 3«" media - now see if it is 720KB or 1.44MB
   by reading sectors 9 and 18 at head 0 track 0
*/
if ( readSpot( drive, HEAD_0, TRACK_0, SECTOR_9, ONE_SECTOR ) != 0 )
   return( 4 );                   /* Cannot tell media size ! */

if ( readSpot( drive,
               HEAD_0,
               TRACK_0,
               SECTOR_18,
               ONE_SECTOR) == 0 )
{
   *maxHead  = 1;                 /* Read 18, so it is 1.44MB */
   *maxSector  = 18;
   *maxTrack = 79;
   }
else
{
   *maxHead  = 1;                 /* Only read 9, so it is 720KB */
   *maxSector  = 9;
   *maxTrack = 79;
   }
return( 0 );
}
