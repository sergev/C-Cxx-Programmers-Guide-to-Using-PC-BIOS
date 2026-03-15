/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Joystick / Mouse Functions
*/


/* Int 15h / 84h - Read Joystick */

unsigned char readJsSwitches( void )
{
union REGS regs;

regs.h.ah = 0x84;             /* AH = 0x84 - read joystick */
regs.x.dx = 0;                /* DX = 0 */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.al );          /* AH = switch settings */
}

void readJsPots( unsigned *potAx,
                 unsigned *potAy,
                 unsigned *potBx,
                 unsigned *potBy )
{
union REGS regs;

regs.h.ah = 0x84;             /* AH = 0x84 - read joystick */
regs.x.dx = 0;                /* DX = 0 */
int86( 0x15, &regs, &regs );  /* Call BIOS */
*potAx = regs.x.ax;           /* AX = pot A X value */
*potAy = regs.x.bx;           /* BX = pot A Y value */
*potBx = regs.x.cx;           /* CX = pot B X value */
*potBy = regs.x.dx;           /* DX = pot B Y value */
}



/* Int 15h / C200h - Enable / Disable PDI Mouse */

#define mouseOn 1
#define mouseOff 0

unsigned char turnMouseOnOff( int flag )
{
union REGS regs;

regs.x.ax = 0xC200;           /* AX = 0xC200 - enable/disable PDI mouse */
regs.h.bh = flag;             /* BH = 0 (disable) or 1 (enable) */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C201h - Reset PDI Mouse */

unsigned char resetMouse( void )
{
union REGS regs;

regs.x.ax = 0xC201;           /* AX = 0xC201 - reset mouse */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C202h - Set PDI Mouse Sample Rate */

unsigned char setMouseSampleRate( unsigned char newRate )
{
union REGS regs;

regs.x.ax = 0xC202;           /* AX = 0xC202 - set PDI mouse sample rate */
regs.h.bh = newRate;          /* BH = new sample rate */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C203h - Set PDI Mouse Resolution */

unsigned char setMouseResolution( unsigned char newResolution )
{
union REGS regs;

regs.x.ax = 0xC203;           /* AX = 0xC203 - set PDI mouse resolution */
regs.h.bh = newResolution;    /* BH = new resolution value */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C204h - Read PDI Mouse Type */

unsigned char readMouseIdByte( unsigned char *idByte )
{
union REGS regs;

regs.x.ax = 0xC204;           /* AX = 0xC204 - read PDI mouse id */
int86( 0x15, &regs, &regs );  /* Call BIOS */
*idByte = regs.h.bh;          /* BH = ID byte */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C205h - Initialize PDI Mouse Interface */

unsigned char initPDI( unsigned char packetSize )
{
union REGS regs;

regs.x.ax = 0xC205;           /* AX = 0xC205 - init PDI mouse interface */
regs.h.bh = packetSize;       /* BH = new packet size */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C206h - Get/Set PDI Mouse Scaling Factor */

unsigned char getMouseStatus( unsigned char command,   /* 0, 1, or 2 */
                              unsigned char *buttons,
                              unsigned char *resolution,
                              unsigned char *sampleRate )
{
union REGS regs;

regs.x.ax = 0xC206;           /* AX = 0xC206 - get/set scaling factor */
regs.h.bh = command;          /* BH = command byte */
int86( 0x15, &regs, &regs );  /* Call BIOS */

*buttons = regs.h.bl;         /* BL = current state value */
*resolution = regs.h.cl;      /* CL = resolution value */
*sampleRate = regs.h.dl;      /* CL = sample rate value */

return( regs.h.ah );          /* AH = status */
}



/* Int 15h / C207h - Set PDI Mouse Driver Address */

unsigned char installMouseDriver( void far *func )
{
union REGS regs;
struct SREGS sregs;

regs.x.ax = 0xC207;                    /* AX = 0xC207 - install driver */
regs.x.bx = FP_OFF(func);              /* BX = driver address offset */
sregs.es = FP_SEG(func);               /* ES = driver address segment */
int86x( 0x15, &regs, &regs, &sregs );  /* Call BIOS */
return( regs.h.ah );                   /* AH = status */
}
