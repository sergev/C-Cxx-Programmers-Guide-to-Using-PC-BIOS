/* **********************************************************
   EISAINFO.C 
   Copyright (c) 1992 - Billy P. Taylor
   **********************************************************
   Displays contents of each EISA slot using interrupt 15h 
   function D800h, and cross references board IDs with their
   configuration files

   To run, type EISAINFO <configuration file path>
   ******************************************************* */

/* ******** DEFINES, MACROS, and OTHER NECESSARY THINGS ********* */
#include <ctype.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define boolean unsigned char
#define YES 1
#define NO  0

#define EISA_BIOS 0x15
#define MAX_SLOT    64        /* Maximum slots to check */

/* Interrupt 15h function D8xxh error codes */
#define INFO_READ_OK  0x00
#define NO_MORE_SLOTS 0x80
#define NVRAM_CORRUPT 0x82
#define SLOT_EMPTY    0x83
#define SLOT_MISMATCH 0x87

#define BIT0   1  /* 00000001 */
#define BIT1   2  /* 00000010 */
#define BIT2   4  /* 00000100 */
#define BIT3   8  /* 00001000 */
#define BIT4  16  /* 00010000 */
#define BIT5  32  /* 00100000 */
#define BIT6  64  /* 01000000 */
#define BIT7 128  /* 10000000 */

/* Int 15h function D800h duplicate slot ID flag and count */
#define DUP_NR   (BIT0 + BIT1 + BIT2 + BIT3)
#define DUP_FLAG (BIT4 + BIT5)

/* *** GLOBAL DATA *** */
boolean searchConfigFiles = NO,
        moreSlotsToCheck = YES;

char *configFileDirectory;

/* Structure that redefines compressed EISA board ID as bit fields */
union boardIdStruct
{
unsigned int compressed[2];
struct bitFields
{
   unsigned int letter2a : 2;
   unsigned int letter1  : 5;
   unsigned int unused   : 1;
   unsigned int letter3  : 5;
   unsigned int letter2b : 3;
   unsigned int number2  : 4;
   unsigned int number1  : 4;
   unsigned int number4  : 4;
   unsigned int number3  : 4;
   } bits;
} boardId;

union  REGS  regs;
struct SREGS sregs;

char boardName[128],
     vendor[128];

/* *** FUNCTION PROTOTYPES *** */
void          describeSlot            ( int );
void          expandEISAId            ( unsigned int, 
                                        unsigned int, 
                                        char * );
unsigned char hexToAscii              ( char );
void          openAndSearchConfigFile ( char *, 
                                        char *, 
                                        char * );

/* ***  MAIN  *** */
void main( int argc, char *argv[] )
{
static unsigned char eisaString[] = "EISA";
static unsigned char far *EISA_SIG = 0xf000ffd9;

int i, slot;

/* Test whether this is an EISA machine */
for( i = 0; i < strlen( eisaString ); i++ )
{
   if ( EISA_SIG[i] != eisaString[i] )
      break;
   }
if ( i != strlen( eisaString ) )
{
   printf( "Sorry, this program requires an EISA machine.\7" );
   exit( 1 );
   }

/* argv[1] is configuration file directory, if specified */
if ( argc > 1 )               /* If config file directory specified */
{
   searchConfigFiles = YES;    /* remember to scan CFG files */
   configFileDirectory = argv[1];
   }

/* Describe each slot */
for( slot = 0; ((slot <= MAX_SLOT) && moreSlotsToCheck ); slot++ )
   describeSlot( slot );

exit( 0 );
}

/* Describe specified slot.  Set boolean 
   moreSlotsToCheck FALSE when last slot read
*/
void describeSlot( int slotNumber )
{
static char nullString[] = {0};
unsigned char boardID[10];

regs.x.ax = 0xd800;                /* AX = D800h - get slot info */
regs.h.cl = slotNumber;            /* CL = slot number */
int86( EISA_BIOS, &regs, &regs );  /* Call BIOS */

printf( "\n" );

if ( regs.h.ah == NO_MORE_SLOTS )
{
   printf( "* No more slots installed *" );
   moreSlotsToCheck = NO;
   return;
   }

/* If slot data is corrupt or contents does not 
   match installed equipment, alert user.
*/
if ( (regs.h.ah == NVRAM_CORRUPT) || 
     (regs.h.ah == SLOT_MISMATCH) )
{
   printf( "*** EISA CONFIGURATION MEMORY IS INVALID! ***\7" );
   moreSlotsToCheck = NO;
   return;
   }

/* Continue if slot data was read 
   successfully or if slot is empty 
*/
if ( (regs.h.ah == INFO_READ_OK) || 
     (regs.h.ah == SLOT_EMPTY) )
{
   printf( "Slot Number..................[%u]", slotNumber);

   if ( regs.h.ah == SLOT_EMPTY )
      printf( " is Empty\n", slotNumber);
   else
   {
      expandEISAId( regs.x.di, regs.x.si, boardID );

      printf( "\n" );
      printf( "Board ID.....................[%s]\n", boardID);

      if ( searchConfigFiles )
      {
         openAndSearchConfigFile( boardID, boardName, vendor );

/* Display left-justified - they may be long and cause line wrap */
         printf( "Board Name...[%s]\n", boardName);
         printf( "Manufacturer.[%s]\n", vendor);
         }

      printf( "Configuration utility ver....[%u.%u]\n", 
                regs.h.bh, regs.h.bl );

      printf( "Functions in this slot.......[%u]\n", regs.h.dh );

      printf( "Board is....................." );

      if ( (regs.h.al & BIT7) != 0 )
         printf( "[Unique]\n" );
      else
         printf( "[Instance %u]\n", (1 + (regs.h.al & DUP_NR)) );

      printf( "Slot ID......................[%sReadable]\n",
               ( (regs.h.al & BIT6) != 0 ) ? "Not" : nullString );

      printf( "Slot Type....................[" );
      switch ( (regs.h.al & DUP_FLAG) )
      {
         case 0x00   :  printf( "Expansion]\n" );
                        break;
         case BIT4   :  printf( "Embedded]\n" );
                        break;
         case BIT5   :  printf( "Virtual]\n" );
                        break;
         default     :  printf( "Unknown]\n" );
                        break;
         }
      }
   }
}

/* Expand compressed EISA board ID into 
   null-terminated displayable string
*/
void expandEISAId( unsigned int high16Bits,    /* DI from D800h */
                   unsigned int low16Bits,     /* SI from D800h */
                   char *stringP )             /* output buffer */
{
boardId.compressed[0] = high16Bits;
boardId.compressed[1] = low16Bits;

*(stringP+0) = boardId.bits.letter1 + '@';          /* Byte 1 of 7 */
*(stringP+1) = (boardId.bits.letter2a * 8) +        /* Byte 2 of 7 */
                boardId.bits.letter2b + '@';
*(stringP+2) = boardId.bits.letter3 + '@';          /* Byte 3 of 7 */
*(stringP+3) = hexToAscii( boardId.bits.number1 );  /* Byte 4 of 7 */
*(stringP+4) = hexToAscii( boardId.bits.number2 );  /* Byte 5 of 7 */
*(stringP+5) = hexToAscii( boardId.bits.number3 );  /* Byte 6 of 7 */
*(stringP+6) = hexToAscii( boardId.bits.number4 );  /* Byte 7 of 7 */
*(stringP+7) = 0;                                   /* Zero terminate */
}

/* Convert binary hex digit into ASCII hex character */
unsigned char hexToAscii( char character )
{
if ( (character >= 0) && (character <= 9) )
   return (character + '0');

if ( (character >= 0x0a) && (character <= 0x0f) )
   return (character - 0x0a + 'A');

return( 0 );
}

/* Locate EISA configuration file by board ID.  
   If successful, return 0 and caller's buffer 
   contains file's fully qualified path (ready to 
   call open()).  If error, return DOS error code
   and explanation in caller's buffer.  
*/
unsigned int findConfigFile( char *boardName,
                             char *pathToSearch,
                             char *outputBuffer )
{
char scratch[128];
char far *scratchP;

struct DTA
{  char reserved[21];
   unsigned char attributes;
   unsigned int  creationTime;
   unsigned int  creationDate;
   unsigned long fileSizeInBytes;
   char fileName[13];
   } far *DTA_Ptr;

/* Get current DOS DTA address */
regs.h.ah = 0x2f;                       /* AH = 2Fh - get DTA address */
int86x( 0x21, &regs, &regs, &sregs );   /* Call DOS */

( unsigned long )DTA_Ptr = sregs.es;    /* Get pointer segment */
( unsigned long )DTA_Ptr <<= 16;        /* Shift into position */
( unsigned long )DTA_Ptr |= regs.x.bx;  /* Add address offset */

/* Create wildcard search string */
strcpy( scratch, pathToSearch );        /* Copy config file path */

if ( scratch[strlen(scratch) - 1] != '\\' )
   strcat( scratch,"\\" );              /* Add trailing slash if needed */

strcpy( outputBuffer, scratch );        /* Save fragment for later */

strcat( scratch,"?" );                  /* First character is a ? */
strcat( scratch, boardName );           /* Append board ID */
strcat( scratch, ".cfg" );              /* Append extension (CFG) */

regs.h.ah = 0x4e;                       /* AH = 4Eh - Find filename */
scratchP = scratch;
sregs.ds = FP_SEG( scratchP );          /* DS = string address segment */
regs.x.dx = FP_OFF( scratchP );         /* DX = string address offset */
regs.x.cx = 0;                          /* CX = 0 - search normal files */
int86x( 0x21, &regs, &regs, &sregs );   /* Call DOS */

/* If file not found, tell user why */
if ( regs.x.cflag )
{
   strcpy( outputBuffer, _strerror(NULL) );
   return( regs.x.ax );
   }

/* File was found */
strcat( outputBuffer, DTA_Ptr->fileName );  /* Add filename to output */
return( 0 );                                /* Return 0 = success */
}

/* Search open configuration file and copy 
   NAME= and MFR= strings to caller's buffers
*/
void searchConfigFile( FILE *stream, 
                       char *name, 
                       char *vendor )
{
#define BUFFERSIZE 128

char buffer[BUFFERSIZE];
char *pValue;

*name = 0;
*vendor = 0;

while( fgets(buffer, BUFFERSIZE, stream) )
{
   while( isspace(buffer[0]) )           /* Discard leading */
      strcpy( &buffer[0], &buffer[1] );  /*   whitespace */

   pValue = strchr( buffer, '=' );       /* Locate '=' in string */

   if ( pValue )                         /* Continue if found */
   {
      do                                 /* Locate token */
      {
         ++pValue; 
         }  while ( isspace(*pValue) );

      if ( *pValue == '"' )              /* If token is quoted string */
      {
         ++pValue;                       /* Look past first quote */
         *strchr( pValue, '"' ) = 0;     /* And ignore last quote */
         }

      if ( memicmp( buffer, "NAME", 4) == 0 )
         strcpy( name, pValue );         /* Copy NAME= token */

      if ( memicmp( buffer, "MFR", 3) == 0 )
         strcpy( vendor, pValue );       /* Copy MFR= token */

      if ( (*name != 0) && (*vendor != 0) )
         break;                          /* Stop looking if both found */
      }
   }
}

/* Locate configuration file for specified board ID, 
   open it, extract board name and vendor strings
*/
void openAndSearchConfigFile( char *boardID, 
                              char *boardName, 
                              char *vendor )
{
char scratch[128];
FILE *configFile;
boolean okSoFar;
static char notAvailable[] = "Not available";

okSoFar = ( findConfigFile( boardID, 
                            configFileDirectory, 
                            scratch ) == 0 );

if ( okSoFar )
{
   configFile = fopen( scratch, "rt" );
   okSoFar = ( configFile != NULL );
   }

if ( !okSoFar )
{
   strcpy( boardName, notAvailable );
   strcpy( vendor, notAvailable );
   }
else
{
   searchConfigFile( configFile, boardName, vendor );
   fclose( configFile );
   }
}
