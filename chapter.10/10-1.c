/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-1.  Read EISA Slot Overview
*/


/*
   Read configuration overview for specified slot.
   Return 0 if successful, or error code if not.
*/

struct EISASlotInfo
{ unsigned int  boardIdHighPart;
  unsigned int  boardIdLowPart;
  unsigned char boardInfo;
  unsigned char cfgUtilMajorVer;
  unsigned char cfgUtilMinorVer;
  unsigned int  checkSum;
  unsigned char functionCount;
  unsigned char functionSummary;
  };

unsigned int readEISASlotInfo( unsigned char slot,
                               struct EISASlotInfo far *slotInfoP )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xd800;                         /* AX = D800h - read slot info */
regs.h.cl = slot;                           /* CL = slot number */
int86x( 0x15, &regs, &regs, &sregs );       /* Call BIOS */

if ( regs.x.cflag == 0 )                    /* If no error, */
{
   slotInfoP->boardIdHighPart = regs.x.si;  /* SI = board ID high 16 bits */
   slotInfoP->boardIdLowPart = regs.x.di;   /* DI = board ID low 16 bits */
   slotInfoP->boardInfo = regs.h.al;        /* AL = board information */
   slotInfoP->cfgUtilMajorVer = regs.h.bh;  /* BH = major version */
   slotInfoP->cfgUtilMinorVer = regs.h.bl;  /* BL = minor version */
   slotInfoP->checkSum = regs.x.cx;         /* CX = checksum */
   slotInfoP->functionCount = regs.h.dh;    /* DH = function count */
   slotInfoP->functionSummary = regs.h.dl;  /* DL = function summary */
   }

return( regs.h.ah );                       /* AH = 0 for success, else */
}                                          /*    = error code          */
