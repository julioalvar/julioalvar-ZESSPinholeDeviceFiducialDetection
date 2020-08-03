//----------------------------------------------------------------------------------
//! Project global and OS specific declarations.
/*!
// \file    
// \author  Alvarez
// \date    2020-07-23
*/
//----------------------------------------------------------------------------------


#pragma once


// DLL export macro definition.
#ifdef ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORTS
  // Use the ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORT macro to export classes and functions.
  #define ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORT ML_LIBRARY_EXPORT_ATTRIBUTE
#else
  // If included by external modules, exported symbols are declared as import symbols.
  #define ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORT ML_LIBRARY_IMPORT_ATTRIBUTE
#endif
