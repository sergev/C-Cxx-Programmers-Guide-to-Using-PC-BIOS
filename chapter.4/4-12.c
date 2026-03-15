/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-12.  Display Character String
*/ 


/* Displays null-terminated string */
void displayString( unsigned char videoPage, 
                    unsigned char row, 
                    unsigned char column, 
                    unsigned char *string )
{
union REGS regs;

regs.h.ah = 2;                   /* AH = 2 - set cursor position */ 
regs.h.bh = videoPage;           /* BH = video page */ 
regs.h.dh = row;                 /* DH = row */ 
regs.h.dl = column;              /* DL = column */ 
int86( 0x10, &regs, &regs );     /* Call BIOS */ 

while( *string )                 /* Display each character */
{
   regs.h.ah = 0x0e;             /* AH = 0x0E - write typewriter style */
   regs.h.al = *string;          /* AL = character */
   regs.h.bh = videoPage;        /* BH = video page */
   int86( 0x10, &regs, &regs );  /* Call BIOS */

   ++string;                     /* Point to next character */
   }
}
