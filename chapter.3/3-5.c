/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-5.  Determine Base Memory Size
*/ 


/* Returns KB of installed base memory */
unsigned int getBaseMemorySize( void )
{
union REGS regs;

   int86( 0x12, &regs, &regs );  /* Just call BIOS - no register setup */
   return( regs.x.ax );          /* AX = KB of base memory */
   }
