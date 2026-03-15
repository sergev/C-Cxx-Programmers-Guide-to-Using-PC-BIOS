/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-13.  Read Character and Attribute
*/ 


/* Returns character and attribute at current cursor position */
void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute )
{
union REGS regs;

regs.h.ah = 8;                /* AH = 8 - read character and attribute */
regs.h.bh = videoPage;        /* BH = video page */
int86( 0x10, &regs, &regs );  /* Call BIOS */

*pChar = regs.h.al;           /* AL = character */
*pAttribute = regs.h.ah;      /* AH = attribute */
}
