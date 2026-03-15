/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-6.  Turn Cursor On or Off
*/


/* Toggles the text mode cursor */
void cursorOnOff( void )
{
unsigned char topLine,
              bottomLine;

getCursorSize( &topLine, &bottomLine );
setCursorSize( bottomLine, topLine );
}
