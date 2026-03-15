/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-1.  Read Diskette Drive Parameters.
*/


/* Returns 0 = success, or an interrupt 13h error code */
unsigned char readDisketteParameters( unsigned char drive,
                                      unsigned char *maxTrack,
                                      unsigned char *maxSector,
                                      unsigned char *maxHead )
{
union REGS regs;

regs.h.ah = 8;                         /* AH = 8 - get drive parameters */
regs.h.dl = drive;                     /* DL = drive number (0...3) */
int86( 0x13, &regs, &regs );           /* Call BIOS */

if ( regs.x.cflag )                    /* If Carry true */
   return( regs.h.ah );                /* AH = error code */

*maxSector = (regs.h.cl & 63);         /* CL bits 5-0 = sectors-per-track */
*maxTrack = regs.h.ch;                 /* CH = maximum track number */
*maxHead = regs.h.dh;                  /* DH = number of heads */

return( 0 );                           /* Signal success */
}
