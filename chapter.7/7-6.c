/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-6.  Set Keyboard Delay and Repeat Rate
*/


void setRepeatRate( unsigned char delay, 
                    unsigned char repeatRate )
{
union REGS regs;

regs.h.ah = 3;                /* AH = 3 - set repeat rate and delay */
regs.h.al = 5;                /* AL = 5 */
regs.h.bh = delay;            /* BH = new delay */

/* BH values (in milliseconds) are:
   0 = 250 ms   1 = 500 ms   2 = 750 ms   3 = 1000 ms
*/
regs.h.bl = repeatRate;       /* BL = new repeat rate */

/* BL values (in characters per second) are:
      0=30.0 cps   0x0B=10.9 cps   0x16=4.3 cps
      1=26.7 cps   0x0C=10.0 cps   0x17=4.0 cps
      2=24.0 cps   0x0D= 9.2 cps   0x18=3.7 cps
      3=21.8 cps   0x0E= 8.6 cps   0x19=3.3 cps
      4=20.0 cps   0x0F= 8.0 cps   0x1A=3.0 cps
      5=18.5 cps   0x10= 7.5 cps   0x1B=2.7 cps
      6=17.1 cps   0x11= 6.7 cps   0x1C=2.5 cps
      7=16.0 cps   0x12= 6.0 cps   0x1D=2.3 cps
      8=15.0 cps   0x13= 5.5 cps   0x1E=2.1 cps
      9=13.3 cps   0x14= 5.0 cps   0x1F=2.0 cps
   0x0A=12.0 cps   0x15= 4.6 cps
*/
int86( 0x16, &regs, &regs );  /* Call BIOS */
}                             /* Returns nothing */
