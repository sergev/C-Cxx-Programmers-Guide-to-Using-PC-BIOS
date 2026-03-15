/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-4.  Generate Far Pointer Using Manual Conversion
*/


/*
   Use interrupt 13h function 8 to illustrate converting 
   segment:offset returned by BIOS into a pointer variable
   via manual method
*/

unsigned char far *getDriveParameterTable( unsigned char whichDrive )
{
union REGS regs;
struct SREGS sregs;
unsigned char far *tablePtr;

/* Begin with NULL pointer in case interrupt fails */
tablePtr = NULL;

regs.h.ah = 8;                          /* AH = function 8 */
regs.h.dl = whichDrive;                 /* DL = drive number */
int86x( 0x13, &regs, &regs, &sregs );   /* Call BIOS */

if ( regs.x.cflag == 0 )                /* If successful, */
{                                       /* ES:DI = table address */
/* Store segment number in lower 16 bits */
   (unsigned long)tablePtr = sregs.es;

/* Shift segment number to upper 16 bits */
   (unsigned long)tablePtr <<= 16;

/* Store offset in lower 16 bits */
   (unsigned long)tablePtr |= regs.x.di;

/* tablePtr now contains far pointer returned by BIOS */
   }

/* Return pointer to caller (NULL if int 13h fails) */
return( tablePtr );
}
