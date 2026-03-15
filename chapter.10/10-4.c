/* 
   C/C++ Programmer's Guide to Using PC BIOS
   Copyright (c) 1992 - Billy P. Taylor

   Program 10-4.  Search EISA Board's Configuration File
*/


/*
   Search open configuration file and copy NAME= 
   and MFR= values to caller's output buffers
*/

void searchConfigFile( FILE *stream, 
                       char *name, 
                       char *vendor )
{
#define BUFFERSIZE 128
char buffer[BUFFERSIZE];

char *pValue;

*name = 0;
*vendor = 0;

while( fgets(buffer, BUFFERSIZE, stream) )
{
   while( isspace(buffer[0]) )           /* Discard leading */
      strcpy( buffer, &buffer[1] );      /*   whitespace */

   pValue = strchr( buffer, '=' );       /* Locate '=' in string */

   if ( pValue )                         /* Continue if found */
   {
      do { ++pValue; }                   /* Locate value */
         while (isspace(*pValue) );

      if ( *pValue == '"' )              /* If value is quoted string */
      {
         ++pValue;                       /* Look past first quote */
         *strchr( pValue, '"' ) = 0;     /* And ignore last quote */
         }

      if ( memicmp( buffer, "NAME=", 5) == 0 )
         strcpy( name, pValue );         /* Copy NAME= value */

      if ( memicmp( buffer, "MFR=", 4) == 0 )
         strcpy( vendor, pValue );       /* Copy MFR= value */

      if ( (*name != 0) && (*vendor != 0) )
         break;                          /* Stop looking if both found */
      }
   }
}
