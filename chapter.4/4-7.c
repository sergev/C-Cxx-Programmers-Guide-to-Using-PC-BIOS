/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-7.  Write Character and Attribute
*/


/* Writes character and attribute to specified video page */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - write char and attribute */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.bl = attribute;        /* BL = attribute */
regs.x.cx = times;            /* CX = number of times to write character */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
