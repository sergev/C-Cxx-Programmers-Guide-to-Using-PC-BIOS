/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-3.  Generate Far Pointer Using MK_FP Macro
*/


/*
   Use interrupt 13h function 8 to illustrate converting 
   segment:offset returned by BIOS into a pointer variable
   using Borland's MK_FP macro.
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

/* 
   If successful, sregs.es contains the segment number, and
   regs.x.di the offset.  MK_FP combines them into a far 
   pointer.  Its first argument is the segment number, 
   its second the offset.
*/

if ( regs.x.cflag == 0 )                /* If successful, */
   tablePtr = MK_FP( sregs.es, regs.x.di );

/* Return pointer to caller (returns NULL if int 13h fails) */
return( tablePtr );
}
