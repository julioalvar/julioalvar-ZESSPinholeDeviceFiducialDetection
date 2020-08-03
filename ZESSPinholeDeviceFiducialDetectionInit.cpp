//----------------------------------------------------------------------------------
//! Dynamic library and runtime type system initialization.
/*!
// \file    
// \author  Alvarez
// \date    2020-07-23
*/
//----------------------------------------------------------------------------------

#include "ZESSPinholeDeviceFiducialDetectionSystem.h"

// Include definition of ML_INIT_LIBRARY.
#include <mlLibraryInitMacros.h>

// Include all module headers ...
#include "mlPinholeDeviceFiducialDetection.h"


ML_START_NAMESPACE

//----------------------------------------------------------------------------------
//! Calls init functions of all modules to add their types to the runtime type
//! system of the ML.
//----------------------------------------------------------------------------------
int ZESSPinholeDeviceFiducialDetectionInit()
{
  // Add initClass calls from modules here.
  PinholeDeviceFiducialDetection::initClass();

  return 1;
}

ML_END_NAMESPACE


//! Calls the init method implemented above during load of shared library.
ML_INIT_LIBRARY(ZESSPinholeDeviceFiducialDetectionInit)