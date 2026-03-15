/* *********************************************************
   CIRCLE.C
   Copyright (c) 1992 - Billy P. Taylor
   *********************************************************
   Draws circles using interrupt 10h function 0Ch.
   The algorithm works by calculating an arc around a given
   center point.  It calculates points in quadrant 1, and 
   then reflects them into the other three quadrants.

   To run, type CIRCLE <Enter>
   *********************************************************** */

#include <conio.h>
#include <dos.h>
#include <math.h>

/* These FOUR #defines determine the video parameters */
#define GRAPHICS_MODE    4      /* BIOS video mode */
#define HRES           320      /* Horizontal resolution */
#define VRES           200      /* Vertical resolution */
#define COLORS           4      /* Number of colors */
#define CIRCLE_SIZE (VRES / 2)  /* Value * 2 = circle size (in pixels) */

/* The circle algorithm assumes that screens are square (that is,
   they have the same number of horizontal and vertical pixels).  
   But graphics mode screens may use 320x200, 640x200, 640x350, or 
   other row and column sizes - obviously not square.  Without 
   correction, the algorithm draws ovals, not circles!  The aspect 
   ratio is applied to each pixel's X-axis (column) number to 
   make the circle round (to compensate for the difference between 
   horizontal and vertical sizes).  If you use a different graphics 
   video mode, you should change ASPECT.  Here are common values for 
   popular pixel resolutions:

   320 x 200 : 1.10           640 x 200 : 2.25
   640 x 350 : 1.30           640 x 480 : 1.05
*/
#define ASPECT     1.10       /* Aspect ratio */

#define PAGE_0         0      /* Video page to write */
#define CENTER_H (HRES/2)     /* Screen midpoint - x axis */
#define CENTER_V (VRES/2)     /* Screen midpoint - y axis */

/* STEP determines the circle's granularity, or how close together
   the pixels in an arc are drawn.  As the number increases, fewer
   calculations are required to compute an arc, BUT holes appear 
   where pixels should be.  The smaller the number, the slower
   the program runs, but the circle looks better.
*/
#define STEP        .03       /* Arc granularity */

#define PI               3.14
#define HALF_PI         (PI/2)

/* Function prototypes */
void          calcPoint    ( float, int, unsigned char );
unsigned char getVideoMode ( void );
void          setVideoMode ( unsigned char );
void          writePixel   ( unsigned char, 
                             unsigned char, 
                             unsigned int, 
                             unsigned int );

void main( void )
{
unsigned char color,
              entryVideoMode;
float point;
int width;

/* Save current video mode and switch to graphics mode */
entryVideoMode = getVideoMode();
setVideoMode( GRAPHICS_MODE );

color = 1;    /* Specifies attribute for each pixel */

for( width = 1; width < CIRCLE_SIZE; width++ )
{
/* Stop if a key is pressed */
   if ( kbhit() )
      break;

/* Draw circle */
   for( point = 0; 
        point < HALF_PI; 
        point += STEP )
      calcPoint( point, width, color );

   color++;                  /* Change colors */
   color %= COLORS;          /* Do not exceed maximum color number */
   }

/* Discard key that caused kbhit() to stop us */
getch();

/* Restore entry video mode */
setVideoMode( entryVideoMode );
}

/* Calculate arc point and draw in all 4 quadrants */
void calcPoint( float point, 
                int width, 
                unsigned char color )
{
float px, py;
int q1_x, q1_y,
    q2_x, q2_y,
    q3_x, q3_y,
    q4_x, q4_y;

/* Calculate point on arc */
px  = cos( point ) * width;
py  = sin( point ) * width;

/* Adjust x-axis aspect ratio (makes circle round, not oval) */
px *= ASPECT;

q1_x = CENTER_H - px;           /* Reflect point into Q1 */
q1_y = CENTER_V - py;
q2_x = CENTER_H - px;           /* Reflect point into Q2 */
q2_y = CENTER_V + py;
q3_x = CENTER_H + px;           /* Reflect point into Q3 */
q3_y = CENTER_V - py;
q4_x = CENTER_H + px;           /* Reflect point into Q4 */
q4_y = CENTER_V + py;

/* Draw pixels */
writePixel( color, PAGE_0, q1_y, q1_x );
writePixel( color, PAGE_0, q2_y, q2_x );
writePixel( color, PAGE_0, q3_y, q3_x );
writePixel( color, PAGE_0, q4_y, q4_x );
}

/* Set specified video mode */
void setVideoMode( unsigned char newMode )
{
union REGS regs;

regs.h.ah = 0;                /* AH = 0 - set video mode */
regs.h.al = newMode;          /* AL = video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
}

/* Return current video mode */
unsigned char getVideoMode( void )
{
union REGS regs;

regs.h.ah = 0x0f;             /* AH = 0x0F - read video mode */
int86( 0x10, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AL = current video mode */
}

/* Write pixel in graphics mode */
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
