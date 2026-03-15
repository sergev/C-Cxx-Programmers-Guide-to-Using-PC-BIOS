/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-1.  Using FP_SEG and FP_OFF with Pointer Variables
*/

char string[] = "Printer Error!";
char far *pCh = &string[0];
unsigned int segment, offset;

segment = FP_SEG( pCh );
offset  = FP_OFF( pCh );
