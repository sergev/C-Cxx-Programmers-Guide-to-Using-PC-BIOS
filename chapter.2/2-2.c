/*
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 2-2.  Using FP_SEG and FP_OFF with Fixed Addresses
                 (Borland C only)
*/

char string[] = "Printer Error!";
unsigned int segment, offset;

segment = FP_SEG( string );
offset  = FP_OFF( string );
