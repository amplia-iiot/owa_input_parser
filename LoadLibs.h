#ifndef __LOAD_LIBS_H
   #define __LOAD_LIBS_H

#include <string.h>
#include <stdarg.h>

#include "owa4x/owcomdefs.h"
#include "owa4x/pm_messages.h"
#include "owa4x/IOs_ModuleDefs.h"

#ifdef __LOAD_LIBS_CPP
      void LoadExternalLibrary( char *wLibraryName, void **wLibHandle );
      int UnloadExternalLibrary( void *wLibHandle );
      void LoadIOSFunctions( void *wLibHandle );
      void LoadRTUControlFunctions( void *wLibHandle);
      int (*FncRTUControl_Initialize)( void *);
      int (*FncRTUControl_Start)( void);
      int (*FncRTUControl_Finalize)( void);
      int (*Fncusecsleep)(int,int);

      int (*FncIO_Initialize)( void );
      int (*FncIO_Start)( void);
      int (*FncIO_Finalize)( void);
      int (*FncIO_IsActive)( int *);
      int (*FncDIGIO_Set_ADC_RANGE)( unsigned char, unsigned char);
      int (*FncANAGIO_GetAnalogIn)( int, int *);
      
      int (*FncDIGIO_Get_DIN)( unsigned char, unsigned char * );
      int (*FncDIGIO_Set_DOUT)( unsigned char, unsigned char );

#else
      extern void LoadExternalLibrary( char *wLibraryName, void **wLibHandle );
      extern int UnloadExternalLibrary( void *wLibHandle );
      extern void LoadIOSFunctions( void *wLibHandle );
      extern void LoadRTUControlFunctions( void *wLibHandle); 
      extern int (*FncRTUControl_Initialize)( void *);
      extern int (*FncRTUControl_Start)( void);
      extern int (*FncRTUControl_Finalize)( void);
      extern int (*Fncusecsleep)(int,int);

      extern int (*FncIO_Initialize)( void );
      extern int (*FncIO_Start)( void);
      extern int (*FncIO_Finalize)( void);
      extern int (*FncIO_IsActive)( int *);
      extern int (*FncDIGIO_Set_ADC_RANGE)( unsigned char, unsigned char);
      extern int (*FncANAGIO_GetAnalogIn)( int, int *);

      extern int (*FncDIGIO_Get_DIN)( unsigned char, unsigned char * );
      extern int (*FncDIGIO_Set_DOUT)( unsigned char, unsigned char );

   #endif

#endif