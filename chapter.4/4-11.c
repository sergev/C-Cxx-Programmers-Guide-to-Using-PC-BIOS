/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-11.  Write Character Typewriter-Style
*/ 


/* Displays character typewriter-style */
void writeTTYChar( unsigned char videoPage,
                   unsigned char character, 
                   unsigned char attribute )
{
union REGS regs;

regs.h.ah = 0x0e;             /* AH = 0x0E - write typewriter-style */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
