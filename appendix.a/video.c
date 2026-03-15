/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Video Functions
*/


/* Int 10h / 00h - Set Video Mode */

void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - set video mode */
regs.h.al = newMode;          /* AL = video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 01h - Set Cursor Type */

void setCursorType( unsigned char topScanLine, 
                    unsigned char bottomScanLine )
{
union REGS regs;

regs.h.ah = 1;                /* AH = 1 - set cursor type */
regs.h.ch = topScanLine;      /* CH = top scan line */
regs.h.ch &= 15;              /* Mask unused bits */
regs.h.cl = bottomScanLine;   /* CL = bottom scan line */
regs.h.cl &= 15;              /* Mask unused bits */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 02h - Set Cursor Position */

void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 - set cursor position */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.dh = row;              /* DH = row number */
regs.h.dl = column;           /* DL = column number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 03h - Read Cursor Position */

void getCursorPosition( unsigned char videoPage,
                        unsigned char *row,
                        unsigned char *column,
                        unsigned char *topScanLine,
                        unsigned char *bottomScanLine )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - get cursor position */
regs.h.bh = videoPage;        /* BH = video page */
int86( 0x10, &regs, &regs );  /* Call BIOS */
*topScanLine = regs.h.ch;     /* CH = cursor top scan line */
*bottomScanLine = regs.h.cl;  /* CL = cursor bottom scan line */
*row = regs.h.dh;             /* DH = cursor row number */
*column = regs.h.dl;          /* DL = cursor column number */
}



/* Int 10h / 04h - Read Light Pen Position */

unsigned char readLightPen( unsigned char *row,
                            unsigned char *column,
                            unsigned int *rasterLine,
                            unsigned int *pixelColumn )
{
union REGS regs;

regs.h.ah = 4;                /* AH = 4 - read light pen position */
int86( 0x10, &regs, &regs );  /* Call BIOS */
*pixelColumn = regs.x.bx;     /* BX = pixel column */
*rasterLine = regs.x.cx;      /* CH or CX = raster line */
*row = regs.h.dh;             /* DH = row */
*column = regs.h.dl;          /* DL = column */
return( regs.h.ah );          /* AH = 1 (success), or 0 (no light pen) */
}



/* Int 10h / 05h - Set Active Video Page */

void setVideoPage( unsigned char newPage )
{
union REGS regs;

regs.h.ah = 5;                /* AH = 5 - set active video page */
regs.h.al = newPage;          /* AL = video page to activate */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 06h - Scroll Page Up */

void scrollUp( unsigned char lines, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 6;                /* AH = 6 - scroll screen up */
regs.h.al = lines;            /* AL = number of lines to scroll */
regs.h.bh = attribute;        /* BH = attribute for new lines */
regs.h.ch = topRow;           /* CH = top row number */
regs.h.cl = leftColumn;       /* CL = leftmost column number */
regs.h.dh = bottomRow;        /* DH = bottom row number */
regs.h.dl = rightColumn;      /* DL = rightmost column number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 07h - Scroll Page Down */

void scrollDown( unsigned char lines, 
                 unsigned char attribute, 
                 unsigned char topRow, 
                 unsigned char leftColumn, 
                 unsigned char bottomRow, 
                 unsigned char rightColumn )
{
union REGS regs;

regs.h.ah = 7;                /* AH = 7 - scroll down */
regs.h.al = lines;            /* AL = number of lines to scroll */
regs.h.bh = attribute;        /* BH = attribute for new lines */
regs.h.ch = topRow;           /* CH = top row number */
regs.h.cl = leftColumn;       /* CL = leftmost column number */
regs.h.dh = bottomRow;        /* DH = bottom row number */
regs.h.dl = rightColumn;      /* DL = rightmost column number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 08h - Read Character and Attribute */

void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute )
{
union REGS regs;

regs.h.ah = 8;                /* AH = 8 - read cell */
regs.h.bh = videoPage;        /* BH = video page to read */
int86( 0x10, &regs, &regs );  /* Call BIOS */
*pAttribute = regs.h.ah;      /* AH = attribute */
*pChar = regs.h.al;           /* AL = character */
}



/* Int 10h / 09h - Write Character and Attribute */

void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 9;                /* AH = 9 - write cell */
regs.h.al = character;        /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.bl = attribute;        /* BL = attribute */
regs.x.cx = times;            /* CX = times to write cell */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 0Ah - Write Character Only */

void writeChar( unsigned char videoPage, 
                unsigned char ch, 
                unsigned int times )
{
union REGS regs;

regs.h.ah = 0x0a;             /* AH = 0x0A - write character */
regs.h.al = ch;               /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.x.cx = times;            /* CX = times to write char */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 0Bh - Set Color Palette */

void setPalette( unsigned char function,
                 unsigned char color )
{
union REGS regs;

regs.h.ah = 0x0b;             /* AH = 0x0B - set color palette */
regs.h.bh = function;         /* BH = function number (0 or 1) */
regs.h.bl = color;            /* BL = new color or palette */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 0Ch - Write Pixel */

void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0c;             /* AH = 0x0C - write pixel */
regs.h.al = color;            /* AL = color */
regs.h.bh = videoPage;        /* BH = video page to write */
regs.x.cx = column;           /* CX = column number */
regs.x.dx = row;              /* DX = row number */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 0Dh - Read Pixel */

unsigned char readPixel( unsigned char videoPage, 
                         unsigned int row, 
                         unsigned int column )
{
union REGS regs;

regs.h.ah = 0x0d;             /* AH = 0x0D - read pixel */
regs.h.bh = videoPage;        /* BH = video page to read */
regs.x.cx = column;           /* CX = column */
regs.x.dx = row;              /* DX = row */
int86( 0x10, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = pixel color */
}



/* Int 10h / 0Eh - Write Character Typewriter-Style */

void writeTTYChar( unsigned char videoPage, 
                   unsigned char ch, 
                   unsigned char attribute )
{
union REGS regs;

regs.h.ah = 0x0e;             /* AH = 0x0E - write character */
regs.h.al = ch;               /* AL = character */
regs.h.bh = videoPage;        /* BH = video page */
regs.h.bl = attribute;        /* BL = video attribute (graphics modes) */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}



/* Int 10h / 0Fh - Read Current Video State */

void readVideoState( unsigned char *columns, 
                     unsigned char *currentMode, 
                     unsigned char *currentPage )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0x0F - read video state */
int86( 0x10, &regs, &regs );  /* Call BIOS */
*columns = regs.h.ah;         /* AH = number of columns displayed */
*currentMode = regs.h.al;     /* AL = current video mode */
*currentPage = regs.h.bh;     /* BH = current video page displayed */
}



/* Int 10h / 1A00h - Read Video Type Code */
void getVideoType( unsigned char *primary,
                   unsigned char *secondary )
{
union REGS regs;

regs.h.ah = 0x1a;             /* AH = 0x1A - read video type code */
regs.h.al = 0;                /* AL = 0 */
int86( 0x10, &regs, &regs );  /* Call BIOS */
if ( regs.h.al == 0x1a )      /* AL = 0x1A if function is supported */
{
   *primary = regs.h.bl;      /* BL = primary video type code */
   *secondary = regs.h.bh;    /* BH = secondary video type code */
   }
else                          /* Else function not supported */
{
   *primary = 0xff;           /* Return both primary and secondary */
   *secondary = 0xff;         /* unknown since we cannot be sure */
   }
}
