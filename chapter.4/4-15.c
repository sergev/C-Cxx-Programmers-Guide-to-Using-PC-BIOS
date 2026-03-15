/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-15.  Write a Pixel
*/ 


/* Writes pixel in specified row and column */
void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - write pixel */
regs.h.al = color;            /* AL = pixel color */
regs.h.bh = videoPage;        /* BH = video page */
regs.x.cx = column;           /* CX = column number */
regs.x.dx = row;              /* DX = row number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
