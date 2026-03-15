/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-7.  Read Installed Equipment Word
*/ 


/* Return BDA installed equipment word */
unsigned int getInstalledEquipment( void )
{
union REGS regs;

   int86( 0x11, &regs, &regs);  /* No registers needed */
   return( regs.x.ax );         /* AX = installed equipment word */
   }
