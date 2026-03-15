/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-6.  Determine Available Extended Memory
*/ 


/* Returns KB of available extended memory */
unsigned int getExtendedMemorySize( void )
{
union REGS regs;

   regs.h.ah = 0x88;             /* AH = 88h - get extended memory size */
   int86( 0x15, &regs, &regs );  /* Call BIOS */
   return ( regs.x.ax );         /* AX = KB available extended mem */
   }
