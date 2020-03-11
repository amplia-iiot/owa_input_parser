#define __LOAD_LIBS_CPP

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

#include "LoadLibs.h"
#include "owa4x_io_adapter.h"

/* Functions definition */

//-----------------------------------------------------------------//
// Function: LoadExternalLibrary()
// Input Params:
//                *wLibraryName: library name string
//                **wLibHandle: pointer to the handler of the
//                              dynamic library
// Output Params:
// Description:
//    Loads a dynamic library from the file named wLibraryName.	
//-----------------------------------------------------------------//
void LoadExternalLibrary( char *wLibraryName, void **wLibHandle )
{
   *wLibHandle = dlopen( wLibraryName, RTLD_LAZY );
   if( !*wLibHandle ) {
      printf("%s\n", dlerror());
      exit(1);
   }
}

//-----------------------------------------------------------------//
// Function: UnloadExternalLibrary()
// Input Params:
//                *wLibHandle: pointer to the handler of the
//                             dynamic library
// Output Params:
// Description:
//    Unloads a dynamic library.  							
//-----------------------------------------------------------------//
int UnloadExternalLibrary( void *wLibHandle )
{
   if( (dlclose( wLibHandle) ) != 0 ) {
		exit(1);
	}
	return 0;
}

//-----------------------------------------------------------------//
// Function: LoadIOSFunctions()
// Input Params:
//                *wLibHandle: pointer to the handler of the
//                             dynamic library
// Output Params:
// Description:
//    Calls to dlsym to get the addresses where the functions of
//    the library are loaded to be used in the program.  		
//-----------------------------------------------------------------//
void LoadIOSFunctions( void *wLibHandle )
{
   FncIO_Initialize = ( int ( *) ( void )) dlsym( wLibHandle, "IO_Initialize");
   FncIO_Start = ( int ( *) ( void)) dlsym( wLibHandle, "IO_Start");
   FncIO_Finalize = ( int ( *) ( void)) dlsym( wLibHandle, "IO_Finalize");
   FncIO_IsActive = ( int ( *) ( int *)) dlsym( wLibHandle, "IO_IsActive");
   FncDIGIO_Set_ADC_RANGE = ( int ( *) ( unsigned char, unsigned char)) dlsym( wLibHandle, "DIGIO_Set_ADC_RANGE");
   FncANAGIO_GetAnalogIn = ( int ( *) ( int, int *)) dlsym( wLibHandle, "ANAGIO_GetAnalogIn");
//Interrupt
   FncDIGIO_Get_DIN = ( int ( *) ( unsigned char, unsigned char *)) dlsym( wLibHandle, "DIGIO_Get_DIN");
   FncDIGIO_Set_DOUT = ( int ( *) ( unsigned char, unsigned char )) dlsym( wLibHandle, "DIGIO_Set_DOUT");
}

//-----------------------------------------------------------------//
// Function: LoadRTUControlFunctions()
// Input Params:
// Output Params:
// Description:
//-----------------------------------------------------------------//
void LoadRTUControlFunctions( void *wLibHandle)
{
	FncRTUControl_Initialize = ( int ( *) ( void *)) dlsym( wLibHandle, "RTUControl_Initialize");
  	FncRTUControl_Start = ( int ( *) ( void)) dlsym( wLibHandle, "RTUControl_Start");
   FncRTUControl_Finalize = ( int ( *) ( void)) dlsym( wLibHandle, "RTUControl_Finalize");
   Fncusecsleep = ( int ( *) (int, int)) dlsym( wLibHandle, "usecsleep");
}