/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 3-8.  Display Installed Devices
*/


/* *********************************************************
   DEVICES.C
   Use interrupts 11h, 12h, and 15h function 88h to 
   identify installed devices.  To run, type DEVICES<Enter>.
************************************************************ */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/*
   Macro returns pointer to "YES" string if flag
   bit is set(1), or to "NO" string if clear(0)
*/
#define YesOrNo( value, bit ) (value & bit) ? yes : no

/* Defines for reading CMOS RAM */
#define CMOS_INDEX 0x70
#define CMOS_DATA  0x71
#define EXTENDED_MEM_LSB 24
#define EXTENDED_MEM_MSB 23

/* Installed Equipment Word Bits     15..............0 */
#define IEW_PRINTERS         3<<14 /* xx.............. */
#define IEW_MODEM            1<<13 /* ..x............. */
#define IEW_r1               1<<12 /* ...x............ */
#define IEW_SERIAL_PORTS     7<< 9 /* ....xxx......... */
#define IEW_r2               1<< 8 /* .......x........ */
#define IEW_DISKETTE_DRIVES  3<< 6 /* ........xx...... */
#define IEW_VIDEO_MODE       3<< 4 /* ..........xx.... */
#define IEW_r3               1<< 3 /* ............x... */
#define IEW_MOUSE            1<< 2 /* .............x.. */
#define IEW_COPROCESSOR      1<< 1 /* ..............x. */
#define IEW_DSKT_DRIVES_PRESENT  1 /* ...............x */

#define IEW_40x25COLOR           1 /* ..........01.... */
#define IEW_80x25COLOR           2 /* ..........10.... */
#define IEW_80x25MONO            3 /* ..........11.... */

/* Shift values to convert bit fields into counters */
#define IEW_PRINTER_SHIFT                 14
#define IEW_SERIAL_PORT_SHIFT              9
#define IEW_DISKETTE_SHIFT                 6
#define IEW_VIDEO_SHIFT                    4

/* Function prototypes */
unsigned int getBaseMemorySize          ( void );
unsigned int getFreeExtendedMemory      ( void );
unsigned int getInstalledExtendedMemory ( void );
unsigned int getInstalledEquipment      ( void );

void main( void )
{
static char yes[] = "YES",
            no[]  = "NO";

unsigned int baseMemory,
             freeExtendedMemory,
             installedExtendedMemory,
             iew,
             numberOfPrinters,
             numberOfDisketteDrives,
             numberOfSerialPorts,
             initialVideoMode;

/* Get installed base memory size from BIOS */
baseMemory = getBaseMemorySize();

/* Get free extended memory size from BIOS */
freeExtendedMemory = getFreeExtendedMemory();

/* Get installed extended memory size from CMOS RAM */
installedExtendedMemory = getInstalledExtendedMemory();

/* Get Installed Equipment Word */
iew = getInstalledEquipment();

/* Convert applicable IEW bit patterns into counters */
numberOfPrinters = 
   ((iew & IEW_PRINTERS) >> IEW_PRINTER_SHIFT);

numberOfSerialPorts = 
   ((iew & IEW_SERIAL_PORTS) >> IEW_SERIAL_PORT_SHIFT);

if ( (iew & IEW_DSKT_DRIVES_PRESENT) )
   numberOfDisketteDrives = 1 + 
      ((iew & IEW_DISKETTE_DRIVES) >> IEW_DISKETTE_SHIFT);
else
   numberOfDisketteDrives = 0;

initialVideoMode = 
   ((iew & IEW_VIDEO_MODE) >> IEW_VIDEO_SHIFT);

/* Display information */
printf( "\n   %uKB of installed base memory", baseMemory );
printf( "\n   %uKB of extended memory",       installedExtendedMemory );
printf( "\n   %uKB of free extended memory",  freeExtendedMemory );
printf( "\n------------------------------------");
printf( "\n   %u printers",                   numberOfPrinters );
printf( "\n   %u serial ports",               numberOfSerialPorts );
printf( "\n   %u diskette drives",            numberOfDisketteDrives );
printf( "\n------------------------------------");
printf( "\n   Internal Modem  ?   %s",        YesOrNo(iew,IEW_MODEM) );
printf( "\n   Mouse           ?   %s",        YesOrNo(iew,IEW_MOUSE) );
printf( "\n   Coprocessor     ?   %s",        YesOrNo(iew,IEW_COPROCESSOR) );
printf( "\n------------------------------------");
printf( "\n   Initial video Mode: " );

switch ( initialVideoMode )
{
   case IEW_40x25COLOR:    
      printf( "40 x 25 Color" );
      break;
   case IEW_80x25COLOR:    
      printf( "80 x 25 Color" );
      break;
   case IEW_80x25MONO:     
      printf( "80 x 25 Monochrome" );
      break;
   default:                
      printf( "Unknown" );
      break;
   }
}

/* Return base memory size in KB */
unsigned int getBaseMemorySize( void )
{
union REGS regs;

int86( 0x12, &regs, &regs );  /* Call BIOS */
return( regs.x.ax );          /* AX = KB */
}

/* Return available extended memory size in KB */
unsigned int getFreeExtendedMemory( void )
{
union REGS regs;

regs.h.ah = 0x88;             /* AH = 88h - get extended memory size */
int86( 0x15, &regs, &regs );  /* Call BIOS */
return ( regs.x.ax );         /* AX = KB */
}

/* Return KB of installed extended memory */
unsigned int getInstalledExtendedMemory( void )
{
unsigned int size;

outp( CMOS_INDEX, EXTENDED_MEM_LSB );
size = inp( CMOS_DATA );
size *= 256;

outp( CMOS_INDEX, EXTENDED_MEM_MSB );
size += inp( CMOS_DATA );

return( size );
}

/* Return BIOS Installed Equipment word */
unsigned int getInstalledEquipment( void )
{
union REGS regs;

int86( 0x11, &regs, &regs);  /* Call BIOS */
return( regs.x.ax );         /* AX = installed equipment word */
}
