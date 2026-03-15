/* 
   C/C++ Programmer's Guide to PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 4-20.  Return Video Adapter Type
*/


/*
   Return installed video adapter type.
   Returns:
      0x00 = no display                    0x07 = VGA / monochrome monitor
      0x01 = MDA / monochrome monitor      0x08 = VGA / color monitor
      0x02 = CGA / color monitor           0x0A = MCGA / color monitor
      0x04 = EGA / color monitor           0x0B = MCGA / monochrome monitor
      0x05 = EGA / monochrome monitor      0x0C = MCGA / color monitor
      0x06 = PGA / color monitor           0xFF = unknown adapter / monitor
*/

unsigned char getVideoAdapterType( void )
{
union REGS regs;

regs.x.ax = 0x1a00;             /* AX = function 1Ah subfunction 0 */
int86( 0x10, &regs, &regs );    /* Call BIOS */

if ( regs.h.al == 0x1a )        /* AL = 1Ah if function is supported */
   return( regs.h.bl );         /* BL = video adapter type */
                                /* If function not supported, */
return( (unsigned char)0xff );  /*    return 0FFh (unknown type) */
}
