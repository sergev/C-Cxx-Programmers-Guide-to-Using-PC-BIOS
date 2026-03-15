/* 
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-2.  Expand EISA Board ID
*/


/*
   Convert compressed EISA board 
   ID into null-terminated string
*/

/* Function prototype */
unsigned char hexToAscii( char );

/* Redefine 32-bit ID using this union */
union boardIdStruct
{
unsigned int compressed[2];     /* 32-bit compressed ID */
struct bitFields                /* Bit field components */
{
   unsigned int letter2a : 2;   /* ID[1] packed letter 1st half */
   unsigned int letter1  : 5;   /* ID[0] packed letter */
   unsigned int unused   : 1;
   unsigned int letter3  : 5;   /* ID[2] packed letter */
   unsigned int letter2b : 3;   /* ID[1] packed letter 2nd half */
   unsigned int number2  : 4;   /* ID[4] binary digit */
   unsigned int number1  : 4;   /* ID[3] binary digit */
   unsigned int number4  : 4;   /* ID[6] binary digit */
   unsigned int number3  : 4;   /* ID[5] binary digit */
   } bits;
} boardId;

/* boardId.compressed holds 32-bit ID returned from BIOS.
   We construct output string using bit fields read from boardId.bits.
Output string  Expansion algorithm
  A......      string[0] = boardId.bits.letter1 + '@'
  AA.....      string[1] = (boardId.bits.letter2a * 8) +
                            boardId.bits.letter2b + '@'
  AAA....      string[2] = boardId.bits.letter3 + '@'
  AAAN...      string[3] = boardId.bits.number1 converted to ASCII hex
  AAANN..      string[4] = boardId.bits.number2 converted to ASCII hex
  AAANNN.      string[5] = boardId.bits.number3 converted to ASCII hex
  AAANNNN      string[6] = boardId.bits.number4 converted to ASCII hex
  AAANNNN0     string[7] = 0 (null terminator)
*/

void expandEISAId( unsigned int high16Bits,     /* DI from D800h */
                   unsigned int low16Bits,      /* SI from D800h */
                   char *out )                  /* Output buffer */
{
/* Insert 32-bit ID */
boardId.compressed[0] = high16Bits;
boardId.compressed[1] = low16Bits;

/* Create string */
*(out+0) = boardId.bits.letter1 + '@';          /* Byte 1 of 7 */
*(out+1) = (boardId.bits.letter2a * 8) +        /* Byte 2 of 7 */
            boardId.bits.letter2b + '@';
*(out+2) = boardId.bits.letter3 + '@';          /* Byte 3 of 7 */
*(out+3) = hexToAscii( boardId.bits.number1 );  /* Byte 4 of 7 */
*(out+4) = hexToAscii( boardId.bits.number2 );  /* Byte 5 of 7 */
*(out+5) = hexToAscii( boardId.bits.number3 );  /* Byte 6 of 7 */
*(out+6) = hexToAscii( boardId.bits.number4 );  /* Byte 7 of 7 */
*(out+7) = 0;                                   /* Zero terminate */
}

/* Convert binary hex digit into ASCII hex character */
unsigned char hexToAscii( char character )
{
if ( (character >= 0) && (character <= 9) )
   return (character + '0');

if ( (character >= 0x0a) && (character <= 0x0f) )
   return (character - 0x0a + 'A');

return( 0 );
}
