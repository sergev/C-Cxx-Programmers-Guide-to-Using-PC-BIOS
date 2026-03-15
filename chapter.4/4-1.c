/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-1.  Read or Set a Video Mode
*/


/* Sets a video mode */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                   /* AH = 0 - set video mode */
regs.h.al = newMode;             /* AL = desired video mode */
int86( 0x10, &regs, &regs );     /* Call BIOS */
}                                /* Returns nothing */


/* Returns current video mode */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;               /* AH = get screen parameters */
int86( 0x10, &regs, &regs );    /* Call BIOS */
return( regs.h.al );            /* AL = current video mode */
}
