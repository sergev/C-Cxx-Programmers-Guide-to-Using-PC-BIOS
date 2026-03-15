/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-4.  Read Hard Disk Sectors
*/ 


/*
   Read sectors from hard disk.
   Returns 0 or an interrupt 13h error code.
*/

unsigned char readHardDiskSectors( unsigned char drive, 
                                   unsigned char sectorsToRead, 
                                   unsigned char head, 
                                   unsigned int cylinder,
                                   unsigned char sector, 
                                   unsigned char far *bufferPtr )
{
union REGS regs;
struct SREGS sregs;

regs.h.ah = 2;                 /* AH = 2 - read sectors */
regs.h.al = sectorsToRead;     /* AL = number of sectors to read */
regs.x.bx = FP_OFF(bufferPtr); /* BX = buffer offset */
regs.h.dh = head;              /* DH = head */
regs.h.dl = drive;             /* DL = drive (0...3 + 80h) */
regs.h.ch = cylinder;          /* CH = low 8 bits of cylinder number */
regs.h.cl = ( cylinder >> 8 ); /* Get high 2 bits of cylinder number */
regs.h.cl <<= 6;               /* Shift into bits 7 and 6 */
regs.h.cl |= ( sector & 63 );  /* CL low 6 bits = sector number */
sregs.es  = FP_SEG(bufferPtr); /* ES = buffer segment */
int86x( 0x13, &regs, 
        &regs, &sregs );       /* Call BIOS */
return( regs.h.ah );           /* AH = 0 or error code */
}
