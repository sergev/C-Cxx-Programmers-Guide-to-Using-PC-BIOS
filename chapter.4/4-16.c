/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-16.  Read a Pixel
*/ 


/* Returns pixel color at specified row and column */
unsigned char readPixel( unsigned char videoPage,
                         unsigned int row, 
                         unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - read pixel */
regs.h.bh = videoPage;        /* BH = video page */
regs.x.cx = column;           /* CX = column */
regs.x.dx = row;              /* DX = row */
int86( 0x10, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = pixel color */
}
