/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 5-2.  Read Hard Drive Parameters.
*/


/* Returns 0 = success, or an interrupt 13h error code */
unsigned char readHardDriveParameters( unsigned char drive,
                                       unsigned int  *maxCylinder,
                                       unsigned char *maxSector,
                                       unsigned char *maxHead )
{
union REGS regs;

unsigned int tmp;
regs.h.ah = 8;                         /* AH = 8 - get drive parameters */
regs.h.dl = drive;                     /* DL = drive number (80h...83h) */
int86( 0x13, &regs, &regs );           /* Call BIOS */

if ( regs.x.cflag )                    /* If Carry true */
   return( regs.h.ah );                /* AH = error code */

/* CL bits 5-0 = sectors-per-track */
*maxSector = (regs.h.cl & 63);         /* Isolate sectors/track and save */

/* CL bits 7-6 = high 2 bits of 10-bit maximum cylinder number */
tmp = (regs.h.cl & (128+64));          /* Isolate top 2 bits */
tmp <<= 2;                             /* Shift them into position */

/* CH = low 8 bits of 10-bit maximum cylinder number */
tmp |= regs.h.ch;                      /* Store cylinder number low 8 bits */
*maxCylinder = tmp;                    /* Copy to caller's variable */

*maxHead = regs.h.dh;                  /* DH = number of heads */
return( 0 );                           /* Signal success */
}
