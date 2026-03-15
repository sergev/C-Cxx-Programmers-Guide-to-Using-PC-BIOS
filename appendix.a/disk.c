/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Disk Functions
*/


/* Int 13h / 00h - Reset Drive (Diskette or Hard Disk) */

unsigned char resetDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - reset drive */
regs.h.dl = drive;            /* DL = drive */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 01h - Get Last I/O Status (Diskette or Hard Disk) */

unsigned char readDriveStatus( unsigned char drive )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - get last I/O status */
regs.h.dl = drive;            /* DL = drive */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 02h - Read Sectors (Diskette or Hard Disk) */

unsigned char readSectors( unsigned char drive, 
                           unsigned char todo, 
                           unsigned char head, 
                           unsigned int cylinder, 
                           unsigned char sector, 
                           unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 2;                         /* AH = 2 - read sectors */
regs.h.al = todo;                      /* AL = number of sectors */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = buffer offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 2 bits */
regs.h.cl <<= 6;                       /*      shift into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(bufferPtr);         /* ES = buffer segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 03h - Write Sectors (Diskette or Hard Disk) */

unsigned char writeSectors( unsigned char drive, 
                            unsigned char todo, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector, 
                            unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 3;                         /* AH = 3 - write sectors */
regs.h.al = todo;                      /* AL = number of sectors */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = buffer offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 2 bits */
regs.h.cl <<= 6;                       /*      shift into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(bufferPtr);         /* ES = buffer segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 04h - Verify Sectors (Diskette or Hard Disk) */

unsigned char verifySectors( unsigned char drive, 
                             unsigned char numberOfSectors, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector,
                             unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 4;                         /* AH = 4 - verify sectors */
regs.h.al = numberOfSectors;           /* AL = number of sectors */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = buffer offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 2 bits */
regs.h.cl <<= 6;                       /*      shift into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(bufferPtr);         /* ES = buffer segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 05h - Format Track (Diskette) */

unsigned char formatTrack( unsigned char drive, 
                           unsigned char head, 
                           unsigned char track, 
                           unsigned char numberOfSectors, 
                           unsigned char far *trackMap )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 5;                         /* AH = 5 - format track */
regs.h.al = numberOfSectors;           /* AL = number of sectors */
regs.h.ch = track;                     /* CH = track number */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.x.bx = FP_OFF(trackMap);          /* BX = track map address offset */
sregs.es  = FP_SEG(trackMap);          /* ES = track map address segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 05h - Format Track (Hard Disk) */

unsigned char formatHdTrack( unsigned char drive, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector, 
                             unsigned char far *flagTable )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 5;                         /* AH = 5 - format track */
regs.x.bx = FP_OFF(flagTable);         /* BX = flagtbl address offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 2 bits */
regs.h.cl <<= 6;                       /*      shifted into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(flagTable);         /* ES = flagtbl address segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 08h - Get Drive Parameters (Diskette) */

unsigned char getDisketteGeometry( unsigned char drive,
                                   unsigned int *maxTrack,
                                   unsigned char *maxSector,
                                   unsigned char *maxHead,
                                   unsigned char *nrDrives,
                                   unsigned char far **fdpt )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *pCh;

regs.h.ah = 8;                         /* AH = 8 - get drive parms */
regs.h.dl = drive;                     /* DL = drive number */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */

if ( regs.x.cflag )                    /* If error... */
   return( regs.h.ah );                /* Return AH = status */

*maxSector = regs.h.cl & 63;           /* CL ..xxxxxx = max sector number */
*maxTrack = regs.h.ch;                 /* CH = max track number */
*maxHead = regs.h.dh;                  /* DH = maximum head number */
*nrDrives = regs.h.dl;                 /* DL = number of drives */

(unsigned long)pCh = sregs.es;         /* ES = table address segment */
(unsigned long)pCh <<= 16;             /* Shift it into position */
(unsigned long)pCh |= regs.x.di;       /* DI = table address offset */

*fdpt = pCh;                           /* Write to caller's variable */
return( 0 );                           /* Return 0 = success */
}



/* Int 13h / 08h - Get Drive Parameters (Hard Drives) */

unsigned char getHdGeometry( unsigned char drive,
                             unsigned int *maxCylinder,
                             unsigned char *maxSector,
                             unsigned char *maxHead,
                             unsigned char *nrDrives,
                             unsigned char far **fdpt )
{
union REGS regs;
struct SREGS sregs;

unsigned int tmp;
unsigned char far *pCh;

regs.h.ah = 8;                         /* AH = 8 - get drive parms */
regs.h.dl = drive;                     /* DL = drive number */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */

if ( regs.x.cflag )                    /* If error... */
   return( regs.h.ah );                /* Return AH = status */

*maxSector = regs.h.cl & 63;           /* CL ..xxxxxx = max sector number */

tmp = regs.h.cl & (128+64);            /* CL xx...... = cylinder high bits */
tmp <<= 2;                             /* Combine with CH */
tmp |= regs.h.ch;                      /* CH = cylinder low 8 bits */

*maxCylinder = tmp;                    /* Write it to caller's variable */
*maxHead = regs.h.dh;                  /* DH = maximum head number */
*nrDrives = regs.h.dl;                 /* DL = number of drives */

(unsigned long)pCh = sregs.es;         /* ES = parm. table address segment */
(unsigned long)pCh <<= 16;             /* Isolate offset part of address */
(unsigned long)pCh |= regs.x.di;       /* DI = parm. table address offset */

*fdpt = pCh;                           /* Write to caller's variable */
return( 0 );                           /* Return 0 = success */
}



/* Int 13h / 09h - Initialize Drive Parameters (Hard Drives) */

unsigned char initHdController( unsigned char drive )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - init drive parameters */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 0Ah - Read Long (Hard Drives) */

unsigned char readLong( unsigned char drive, 
                        unsigned char numberOfSectors, 
                        unsigned char head, 
                        unsigned int cylinder, 
                        unsigned char sector, 
                        unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0x0a;                      /* AH = 0x0A - read long */
regs.h.al = numberOfSectors;           /* AL = number of sectors */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = buffer address offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 8 bits */
regs.h.cl <<= 6;                       /*      shifted into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(bufferPtr);         /* ES = buffer address segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 0Bh - Write Long (Hard Drives) */

unsigned char writeLong( unsigned char drive, 
                         unsigned char numberOfSectors, 
                         unsigned char head, 
                         unsigned int cylinder, 
                         unsigned char sector, 
                         unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0x0b;                      /* AH = 0x0B - write long */
regs.h.al = numberOfSectors;           /* AL = number of sectors */
regs.x.bx = FP_OFF(bufferPtr);         /* BX = buffer address offset */
regs.h.dh = head;                      /* DH = head number */
regs.h.dl = drive;                     /* DL = drive number */
regs.h.ch = cylinder;                  /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;             /* CL = cylinder high 8 bits */
regs.h.cl <<= 6;                       /*      shifted into xx...... */
regs.h.cl |= ( sector & 63 );          /* CL ..xxxxxx = sector number */
sregs.es  = FP_SEG(bufferPtr);         /* ES = buffer address segment */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 0Ch - Seek to Cylinder (Hard Drives) */

unsigned char seekCylinder( unsigned char drive, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - seek to cylinder */
regs.h.dh = head;             /* DH = head number */
regs.h.dl = drive;            /* DL = drive number */
regs.h.ch = cylinder;         /* CH = cylinder low 8 bits */
regs.h.cl = cylinder >> 8;    /* CL = cylinder high 2 bits */
regs.h.cl <<= 6;              /*      shifted into xx...... */
regs.h.cl |= ( sector & 63 ); /* CL ..xxxxxx = sector number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 0Dh - Alternate Disk Reset (Hard Drives) */

unsigned char altDiskReset( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - alt. disk reset */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 10h - Test Drive Ready (Hard Drives) */

unsigned char testDriveReady( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x10;             /* AH = 0x10 - test drive ready */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 11h - Recalibrate Drive (Hard Drives) */

unsigned char recalibrateDrive( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x11;             /* AH = 0x11 - recalibrate drive */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 12h - Execute Controller RAM Diagnostic (Hard Drives) */

unsigned char doControllerRAMDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x12;             /* AH = 0x12 - execute diagnostic */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 13h - Execute Controller Diagnostic (Hard Drives) */

unsigned char doControllerDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x13;             /* AH = 0x13 - execute diagnostic */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 14h - Execute Controller Diagnostic (Hard Drives) */

unsigned char executeControllerDiagnostic( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x14;             /* AH = 0x14 - execute diagnostic */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 15h - Get Drive Type (Diskette) */

unsigned char getDriveType( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x15;             /* AH = 0x15 - get drive type */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return ( regs.h.ah );         /* AH = status */
}



/* Int 13h / 15h - Get Drive Type (Hard Drive) */

unsigned char driveIsValid( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x15;             /* AH = 0x15 - get drive type */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



unsigned char getSectorsOnDrive( unsigned char drive,
                                 unsigned long *out )
{
union REGS regs;

*out = 0L;
regs.h.ah = 0x15;             /* AH = 0x15 - get drive type */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
if ( !regs.x.cflag )          /* If no error... */
{
   *out = regs.x.cx;          /* CX = sector count - high 16 bits */
   *out <<= 16;
   *out += regs.x.dx;         /* DX = sector count - low 16 bits */
   }
return( regs.h.ah );          /* AH = status */
}



/* Int 13h / 16h - Get Diskette Change Status (Diskette) */

unsigned char getChangeSignal( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x16;             /* AH = 0x16 - get change signal */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return ( regs.h.ah );         /* AH = status (0, 1, or 6) */
}



/* Int 13h / 17h - Set Diskette Type for Format (Diskette) */

unsigned char setDriveType( unsigned char drive, 
                            unsigned char type )
{
union REGS regs;

regs.h.ah = 0x17;             /* AH = 0x17 - set drive/media type */
regs.h.al = type;             /* AL = drive/media type */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return ( regs.h.ah );         /* AH = status */
}



/* Int 13h / 18h - Set Media Type for Format (Diskette) */

unsigned char setMediaType( unsigned char drive, 
                            unsigned char maxTracks,
                            unsigned char sectorsPerTrack,
                            unsigned char far **tablePtr )
{
union REGS regs;
struct SREGS sregs;

unsigned char far *pCh;

regs.h.ah = 0x18;                      /* AH = 0x18 - set media type */
regs.h.ch = maxTracks;                 /* CH = maximum track number */
regs.h.cl = sectorsPerTrack;           /* CL = sectors per track */
regs.h.dl = drive;                     /* DL = drive number */
int86x( 0x13, &regs, &regs, &sregs );  /* Call BIOS */

if ( !regs.x.cflag )                   /* If no errors... */
{
   (unsigned long)pCh = sregs.es;      /* ES = parm. table address segment */
   (unsigned long)pCh <<= 16;          /* Shift it into position */
   (unsigned long)pCh |= regs.x.di;    /* DI = parm. table address offset */
   *tablePtr = pCh;                    /* Write it to caller's variable */
   }

return( regs.h.ah );                   /* AH = status */
}



/* Int 13h / 19h - Park Heads (Hard Drives) */

unsigned char parkHeads( unsigned char drive )
{
union REGS regs;

regs.h.ah = 0x19;             /* AH = 0x19 - park heads */
regs.h.dl = drive;            /* DL = drive number */
int86( 0x13, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}
