/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-10.  Write Character Only
*/ 


/* Writes one character n times starting at current cursor position */
void writeChar( unsigned char videoPage, 
                unsigned char character, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - write character */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.x.cx = times;            /* CX = times to write character */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
