/* 
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-3.  Locate EISA Configuration File
*/


/*
   Locate EISA configuration file by board ID.  If successful, 
   return 0 and caller's buffer contains file's fully-qualified 
   path (ready to call open()).  If error, return DOS error code.  
*/

unsigned int findConfigFile( char *boardName,
                             char *pathToSearch,
                             char *outputBuffer )
{
union REGS regs;
struct SREGS sregs;

char scratch[128];
char *scratchP;

/* struct DTA defines DOS Disk Transfer Area as used 
   in file searches.  The DTA is a memory area used 
   to pass data between DOS and programs.  Interrupt 21h 
   function 2Fh returns its address.
*/
struct DTA
{  char reserved[21];
   unsigned char attributes;
   unsigned int  creationTime;
   unsigned int  creationDate;
   unsigned long fileSizeInBytes;
   char fileName[13];
   } far *DTA_Ptr;

/* Get current DOS DTA address */
regs.h.ah = 0x2f;                            /* AH = 2Fh - get DTA address */
int86x( 0x21, &regs, &regs, &sregs );        /* Call DOS */
( unsigned long )DTA_Ptr = sregs.es;         /* Get pointer segment */
( unsigned long )DTA_Ptr <<= 16;             /* Shift into position */
( unsigned long )DTA_Ptr |= regs.x.bx;       /* Add address offset */

/* Create wildcard search string */
strcpy( scratch, pathToSearch );             /* Copy config file path */

if ( scratch[strlen(scratch) - 1] != '\\' )
   strcat( scratch,"\\" );                   /* Add trailing slash if needed */

strcpy( outputBuffer, scratch );             /* Save fragment for later */

strcat( scratch,"?" );                       /* First character is ? */
strcat( scratch, boardName );                /* Append board ID */
strcat( scratch, ".cfg" );                   /* Append CFG extension */

regs.h.ah = 0x4e;                            /* AH = 4Eh - Find filename */
scratchP = scratch;
sregs.ds = FP_SEG( scratchP );               /* DS = string address segment */
regs.x.dx = FP_OFF( scratchP );              /* DX = string address offset */
regs.x.cx = 0;                               /* CX = 0 - search normal files */
int86x( 0x21, &regs, &regs, &sregs );        /* Call DOS */

if ( regs.x.cflag )                          /* If file not found */
{
   strcpy( outputBuffer, _strerror(NULL) );  /* Tell user why */
   return( regs.x.ax );                      /* Return error code */
   }
                                             /* File was found */
strcat( outputBuffer, DTA_Ptr->fileName );   /* Add filename to output */
return( 0 );                                 /* Return 0 = success */
}
