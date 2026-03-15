/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-3.  Read Diskette Sectors
*/ 


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
regs.h.dl = drive;             /* DL = drive number (0...3) */
regs.h.ch = track;             /* CH = track number */
regs.h.cl = sector;            /* CL = sector number */
sregs.es  = FP_SEG(bufferPtr); /* ES = buffer segment */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Call BIOS */
return( regs.h.ah );           /* AH = 0 or error code */
}
