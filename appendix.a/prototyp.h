/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Appendix A.  Function Prototypes
*/


/* Int 05h - Print Screen Key Software Interface */
unsigned char printScreen( void );


/* Int 10h / 00h - Set Video Mode */
void setVideoMode( unsigned char newMode );


/* Int 10h / 01h - Set Cursor Type */
void setCursorType( unsigned char topScanLine, 
                    unsigned char bottomScanLine );


/* Int 10h / 02h - Set Cursor Position */
void setCursorPosition( unsigned char videoPage, 
                        unsigned char row, 
                        unsigned char column );


/* Int 10h / 03h - Read Cursor Position */
void getCursorPosition( unsigned char videoPage,
                        unsigned char *row,
                        unsigned char *column,
                        unsigned char *topScanLine,
                        unsigned char *bottomScanLine );


/* Int 10h / 04h - Read Light Pen Position */
unsigned char readLightPen( unsigned char *row,
                            unsigned char *column,
                            unsigned int *rasterLine,
                            unsigned int *pixelColumn );


/* Int 10h / 05h - Set Active Video Page */
void setVideoPage( unsigned char newPage );


/* Int 10h / 06h - Scroll Page Up */
void scrollUp( unsigned char lines, 
               unsigned char attribute, 
               unsigned char topRow, 
               unsigned char leftColumn, 
               unsigned char bottomRow, 
               unsigned char rightColumn );


/* Int 10h / 07h - Scroll Page Down */
void scrollDown( unsigned char lines, 
                 unsigned char attribute, 
                 unsigned char topRow, 
                 unsigned char leftColumn, 
                 unsigned char bottomRow, 
                 unsigned char rightColumn );


/* Int 10h / 08h - Read Character and Attribute */
void readCell( unsigned char videoPage, 
               unsigned char *pChar, 
               unsigned char *pAttribute );


/* Int 10h / 09h - Write Character and Attribute */
void writeCell( unsigned char videoPage, 
                unsigned char character, 
                unsigned char attribute, 
                unsigned int times );


/* Int 10h / 0Ah - Write Character */
void writeChar( unsigned char videoPage, 
                unsigned char ch, 
                unsigned int times );


/* Int 10h / 0Bh - Set Color Palette */
void setPalette( unsigned char paletteEntry,
                 unsigned char color );


/* Int 10h / 0Ch - Write Pixel */
void writePixel( unsigned char color, 
                 unsigned char videoPage, 
                 unsigned int row, 
                 unsigned int column );


/* Int 10h / 0Dh - Read Pixel */
unsigned char readPixel( unsigned char videoPage, 
                         unsigned int row, 
                         unsigned int column );


/* Int 10h / 0Eh - Write Character Typewriter-Style */
void writeTTYChar( unsigned char videoPage, 
                   unsigned char ch, 
                   unsigned char attribute );


/* Int 10h / 0Fh - Read Current Video State */
void readVideoState( unsigned char *columns, 
                     unsigned char *currentMode, 
                     unsigned char *currentPage );


/* Int 10h / 1A00h - Read Video Type Code */
void getVideoType( unsigned char *primary, 
                   unsigned char *secondary );


/* Int 11h - Read Installed Equipment Word */
unsigned int getInstalledEquipment( void );


/* Int 12h - Get Base Memory Size */
unsigned int getBaseMemorySize( void );


/* Int 13h / 00h - Reset Drive (Diskette or Hard Disk) */
unsigned char resetDrive( unsigned char drive );


/* Int 13h / 01h - Get Last I/O Status (Diskette or Hard Disk) */
unsigned char readDriveStatus( unsigned char drive );


/* Int 13h / 02h - Read Sectors (Diskette or Hard Disk) */
unsigned char readSectors( unsigned char drive, 
                           unsigned char todo, 
                           unsigned char head, 
                           unsigned int cylinder, 
                           unsigned char sector, 
                           unsigned char far *bufferPtr );


/* Int 13h / 03h - Write Sectors (Diskette or Hard Disk) */
unsigned char writeSectors( unsigned char drive, 
                            unsigned char todo, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector, 
                            unsigned char far *bufferPtr );


/* Int 13h / 04h - Verify Sectors (Diskette or Hard Disk) */
unsigned char verifySectors( unsigned char drive, 
                             unsigned char numberOfSectors, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector,
                             unsigned char far *bufferPtr );


/* Int 13h / 05h - Format Track (Diskette) */
unsigned char formatTrack( unsigned char drive, 
                           unsigned char head, 
                           unsigned char track, 
                           unsigned char numberOfSectors, 
                           unsigned char far *trackMap );


/* Int 13h / 05h - Format Track (Hard Disk) */
unsigned char formatHdTrack( unsigned char drive, 
                             unsigned char head, 
                             unsigned int cylinder, 
                             unsigned char sector, 
                             unsigned char far *flagTable );


/* Int 13h / 08h - Get Drive Parameters (Diskette) */
unsigned char getDisketteGeometry( unsigned char drive,
                                   unsigned int *maxTrack,
                                   unsigned char *maxSector,
                                   unsigned char *maxHead,
                                   unsigned char *nrDrives,
                                   unsigned char far **fdpt );


/* Int 13h / 08h - Get Drive Parameters (Hard Drives) */
unsigned char getHdGeometry( unsigned char drive,
                             unsigned int *maxCylinder,
                             unsigned char *maxSector,
                             unsigned char *maxHead,
                             unsigned char *nrDrives,
                             unsigned char far **fdpt );


/* Int 13h / 09h - Initialize Drive Parameters (Hard Drives) */
unsigned char initHdController( unsigned char drive );


/* Int 13h / 0Ah - Read Long (Hard Drives) */
unsigned char readLong( unsigned char drive, 
                        unsigned char numberOfSectors, 
                        unsigned char head, 
                        unsigned int cylinder, 
                        unsigned char sector, 
                        unsigned char far *bufferPtr );


/* Int 13h / 0Bh - Write Long (Hard Drives) */
unsigned char writeLong( unsigned char drive, 
                         unsigned char numberOfSectors, 
                         unsigned char head, 
                         unsigned int cylinder, 
                         unsigned char sector, 
                         unsigned char far *bufferPtr );


/* Int 13h / 0Ch - Seek to Cylinder (Hard Drives) */
unsigned char seekCylinder( unsigned char drive, 
                            unsigned char head, 
                            unsigned int cylinder, 
                            unsigned char sector );


/* Int 13h / 0Dh - Alternate Disk Reset (Hard Drives) */
unsigned char altDiskReset( unsigned char drive );


/* Int 13h / 10h - Test Drive Ready (Hard Drives) */
unsigned char testDriveReady( unsigned char drive );


/* Int 13h / 11h - Recalibrate Drive (Hard Drives) */
unsigned char recalibrateDrive( unsigned char drive );


/* Int 13h / 12h - Execute Controller RAM Diagnostic (Hard Drives) */
unsigned char doControllerRAMDiagnostic( unsigned char drive );


/* Int 13h / 13h - Execute Controller Diagnostic (Hard Drives) */
unsigned char doControllerDiagnostic( unsigned char drive );


/* Int 13h / 14h - Execute Controller Diagnostic (Hard Drives) */
unsigned char executeControllerDiagnostic( unsigned char drive );


/* Int 13h / 15h - Get Drive Type (Diskette) */
unsigned char getDriveType( unsigned char drive );


/* Int 13h / 15h - Get Drive Type (Hard Drive) */
unsigned char driveIsValid( unsigned char drive );
unsigned char getSectorsOnDrive( unsigned char drive, unsigned long *out );


/* Int 13h / 16h - Get Diskette Change Status (Diskette) */
unsigned char getChangeSignal( unsigned char drive );


/* Int 13h / 17h - Set Diskette Type for Format (Diskette) */
unsigned char setDriveType( unsigned char drive, 
                            unsigned char type );


/* Int 13h / 18h - Set Media Type for Format (Diskette) */
unsigned char setMediaType( unsigned char drive, 
                            unsigned char maxTracks,
                            unsigned char sectorsPerTrack,
                            unsigned char far **tablePtr );


/* Int 13h / 19h - Park Heads (Hard Drives) */
unsigned char parkHeads( unsigned char drive );


/* Int 14h / 00h - Initialize Serial Port */
unsigned int initSerialPort( unsigned int port, 
                             unsigned char setupVal );


/* Int 14h / 01h - Transmit Character via Serial Port */
unsigned char comTx( unsigned int port, 
                     unsigned char charToXmit );


/* Int 14h / 02h - Receive Character from Serial Port */
unsigned int comRx( unsigned int port );


/* Int 14h / 03h - Read Serial Port Status */
unsigned int comReadStatus( unsigned int port );


/* Int 14h / 04h - Extended Initialize Serial Port */
unsigned int comExtInit( unsigned int port, 
                         unsigned char breakCode, 
                         unsigned char parity, 
                         unsigned char stopBits, 
                         unsigned char wordLgt, 
                         unsigned char baudRate );


/* Int 14h / 05h - Extended Modem Control */
unsigned int comModemCtl( unsigned int port, 
                          unsigned char rwCode, 
                          unsigned char *modemCtlReg );


/* Int 15h / 83h - Set / Clear Timer */
extern unsigned char doneFlag;
unsigned char startTimer( unsigned long timeValue );

/* Macros to test whether timer is still running */
#define TIMER_HAS_EXPIRED( flag ) ( (flag & 128) == 1 )
#define TIMER_IS_RUNNING( flag )  ( (flag & 128) == 0 )


/* Int 15h / 84h - Read Joystick */
unsigned char readJsSwitches( void );
void readJsPots( unsigned *potAx,
                 unsigned *potAy,
                 unsigned *potBx,
                 unsigned *potBy );


/* Int 15h / 86h - Unconditional Wait */
unsigned char suspendProgram( unsigned long timeValue );


/* Int 15h / 88h - Get Extended Memory Size */
unsigned int getExtendedMemorySize( void );


/* Int 15h / C0h - Return System Environment */
struct cfgParamTbl
{
   unsigned int  tableLength;
   unsigned char systemModelCode;
   unsigned char systemTypeCode;
   unsigned char BIOSRevisionNumber;
   unsigned char cfgInfoBits;
   };

struct cfgParamTbl far *getCfgParmTblAddr( void );


/* Int 15h / C1h - Return Extended BIOS Data Area Segment */
unsigned char getEBDASegment( unsigned char *segment );


/* Int 15h / C200h - Enable/Disable PDI Mouse */
#define mouseOn 1
#define mouseOff 0
unsigned char turnMouseOnOff( int flag );


/* Int 15h / C201h - Reset PDI Mouse */
unsigned char resetMouse( void );


/* Int 15h / C202h - Set PDI Mouse Sample Rate */
unsigned char setMouseSampleRate( unsigned char newRate );


/* Int 15h / C203h - Set PDI Mouse Resolution */
unsigned char setMouseResolution( unsigned char newResolution );


/* Int 15h / C204h - Read PDI Mouse Type */
unsigned char readMouseIdByte( unsigned char *idByte );


/* Int 15h / C205h - Initialize PDI Mouse Interface */
unsigned char initPDI( unsigned char packetSize );


/* Int 15h / C206h - Get/Set PDI Mouse Scaling Factor */
unsigned char getMouseStatus( unsigned char command,
                              unsigned char *buttons,
                              unsigned char *resolution,
                              unsigned char *sampleRate );


/* Int 15h / C207h - Set PDI Mouse Driver Address */
unsigned char installMouseDriver( void far *func );


/* Int 15h / D800h - Read EISA Slot Overview */

struct EISASlotInfo
{
   unsigned int boardId1;
   unsigned int boardId2;
   unsigned char boardInfo;
   unsigned char cfgUtilMajorVer;
   unsigned char cfgUtilMinorVer;
   unsigned int  checkSum;
   unsigned char functionCount;
   unsigned char functionSummary;
   };

unsigned char readEISASlotInfo( unsigned char slot,
                                struct EISASlotInfo far *out );


/* Int 16h / 00h - Get Keyboard Character */
unsigned int getKbdChar( void );


/* Int 16h / 01h - Check for Keyboard Data Ready */
unsigned int testKbdCharReady( void );


/* Int 16h / 02h - Read Keyboard Shift Status */
unsigned char readShiftStatus( void );


/* Int 16h / 03h - Set Keyboard Repeat Rate */
void setRepeatRate( unsigned char delay, 
                    unsigned char repeatRate );


/* Int 16h / 05h - Write to Typeahead Buffer */
unsigned char writeKbdData( unsigned char scanCode, 
                            unsigned char charCode );


/* Int 16h / 10h - Get Enhanced Keyboard Character */
unsigned int getEnhancedKbdChar( void );


/* Int 16h / 11h - Check for Enhanced Keyboard Data Ready */
unsigned int testEnhancedKbdCharReady( void );


/* Int 16h / 12h - Read Extended Keyboard Shift Status */
unsigned int readExtendedShiftStatus( void );


/* Int 17h / 00h - Print Character */
unsigned char printChar( unsigned int printer, 
                         unsigned char chToPrint );


/* Int 17h / 01h - Initialize Printer Port */
unsigned char initPrinterPort( unsigned int port );


/* Int 17h / 02h - Get Printer Status */
unsigned char getPrinterStatus( unsigned int printer );


/* Int 1Ah / 00h - Read System Tick Counter */
unsigned char readTickCounter( unsigned long *out );


/* Int 1Ah / 01h - Set System Tick Counter */
void setTickCounter( unsigned long tickValue );


/* Int 1Ah / 02h - Read Real-Time Clock Time */
void readRTCTime( unsigned char *hour, 
                  unsigned char *min, 
                  unsigned char *sec );


/* Int 1Ah / 03h - Set Real-Time Clock Time */
void setRTCTime( unsigned char hour, 
                 unsigned char min, 
                 unsigned char sec );


/* Int 1Ah / 04h - Read Real-Time Clock Date */
void readRTCDate( unsigned int *year, 
                  unsigned char *month, 
                  unsigned char *day );


/* Int 1Ah / 05h - Set Real-Time Clock Date */
void setRTCDate( unsigned int year, 
                 unsigned char month, 
                 unsigned char day );


/* Int 1Ah / 06h - Set Real-Time Clock Alarm */
void setRTCAlarm( unsigned char hour, 
                  unsigned char min, 
                  unsigned char sec );


/* Int 1Ah / 07h - Reset Real-Time Clock Alarm */
void resetRTCAlarm( void );
