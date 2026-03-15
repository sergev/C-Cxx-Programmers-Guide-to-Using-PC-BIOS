/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Environment Functions
*/


/* Int 11h - Read Installed Equipment Word */

unsigned int getInstalledEquipment( void )
{
union REGS regs;

int86( 0x11, &regs, &regs);  /* Call BIOS */
return( regs.x.ax );         /* AX = installed equipment word */
}



/* Int 12h - Get Base Memory Size */

unsigned int getBaseMemorySize( void )
{
union REGS regs;

int86( 0x12, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AX = KB of base memory */
}



/* Int 15h / 88h - Get Extended Memory Size */

unsigned int getExtendedMemorySize( void )
{
union REGS regs;

regs.h.ah = 0x88;             /* AH = 0x88 - get extended memory size */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return ( regs.x.ax );         /* AX = available KB of extended memory */
}



/* Int 15h / C0h - Return System Environment */

struct cfgParamTbl
{
   unsigned int  tableLength;
   unsigned char systemModelCode;
   unsigned char systemTypeCode;
   unsigned char BIOSRevisionNumber;
   unsigned char cfgInfoBits;
   };

struct cfgParamTbl far *getCfgParmTblAddr( void )
{
union REGS regs;
struct SREGS sregs;

struct cfgParamTbl far *pCh;

regs.h.ah = 0xc0;                      /* AH = 0xC0 - return environment */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */

(unsigned long)pCh = sregs.es;         /* ES = table address segment */
(unsigned long)pCh <<= 16;             /* Shift it into position */
(unsigned long)pCh |= regs.x.bx;       /* BX = table address offset */

return( pCh );                         /* Return address to caller */
}



/* Int 15h / C1h - Return Extended BIOS Data Area Segment */

unsigned char getEBDASegment( unsigned char *segment )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 0xC1;                      /* AH = 0xC1 - read EBDA segment */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */

if ( regs.x.cflag )                    /* If error */
   return( regs.h.ah );                /* AH = status */

*segment = sregs.es;                   /* ES = EBDA address segment */
return( 0 );
}



/* Int 15h / D800h - Read EISA Slot Overview */

struct EISASlotInfo
{
   unsigned int boardId1;
   unsigned int boardId2;
   unsigned char boardInfo;
   unsigned char cfgUtilMajorVer;
   unsigned char cfgUtilMinorVer;
   unsigned int  checkSum;
   unsigned char functionCount;
   unsigned char functionSummary;
   };

unsigned char readEISASlotInfo( unsigned char slot,
                                struct EISASlotInfo far *out )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xd800;                    /* AX = 0xD800 - read slot info */
regs.h.cl = slot;                      /* CL = slot number */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */

out->boardId1 = regs.x.di;             /* DI = 1st compressed board id */
out->boardId2 = regs.x.si;             /* SI = 2nd compressed board id */
out->boardInfo = regs.h.al;            /* AL = board information byte */
out->cfgUtilMajorVer = regs.h.bh;      /* BH = cfg. utility major version */
out->cfgUtilMinorVer = regs.h.bl;      /* BL = cfg. utility version */
out->checkSum = regs.x.cx;             /* CX = checksum */
out->functionCount = regs.h.dh;        /* DH = number of functions */
out->functionSummary = regs.h.dl;      /* DL = function summary byte */

return( regs.h.ah );                   /* AH = status */
}
