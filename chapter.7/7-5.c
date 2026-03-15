/*
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 7-5.  Read Unbuffered Key Status
*/


/*
   Returns current state of Shift, Ctrl, Alt, 
   Num Lock, Ins, and Caps Lock keys
*/

void readKeyStatus( unsigned char *keyStatus1,
                    unsigned char *keyStatus2 )
{
union REGS regs;

regs.h.ah = 2;                /* AH = 2 or 12h - read key status */
int86( 0x16, &regs, &regs );  /* Call BIOS */

*keyStatus1 = regs.h.al;      /* AL = key status for all keyboards */
*keyStatus2 = regs.h.ah;      /* AH = more status from function 12h */

/* Bits in AL are set as follows:
      bit 7 : 1 = Insert mode is on
      bit 6 : 1 = CapsLock mode is on
      bit 5 : 1 = Num Lock mode is on
      bit 4 : 1 = Scroll Lock mode is on
      bit 3 : 1 = an Alt key is pressed
      bit 2 : 1 = a Ctrl key is pressed
      bit 1 : 1 = left Shift key is pressed
      bit 0 : 1 = right Shift key is pressed
   For function 12h only, bits in AH are set as follows:
      bit 7 : 1 = SysReq key is pressed
      bit 6 : 1 = CapsLock key is pressed
      bit 5 : 1 = Num Lock key is pressed
      bit 4 : 1 = Scroll Lock key is pressed
      bit 3 : 1 = right Alt key is pressed
      bit 2 : 1 = right Ctrl key is pressed
      bit 1 : 1 = left Alt key is pressed
      bit 0 : 1 = left Ctrl key is pressed
*/
}                             /* Returns nothing */
