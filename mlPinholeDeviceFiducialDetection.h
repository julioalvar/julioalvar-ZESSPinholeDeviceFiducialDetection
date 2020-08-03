//----------------------------------------------------------------------------------
//! The ML module class PinholeDeviceFiducialDetection.
/*!
// \file    
// \author  Alvarez
// \date    2020-07-23
//
// 
*/
//----------------------------------------------------------------------------------


#pragma once


#include "ZESSPinholeDeviceFiducialDetectionSystem.h"

#include <Eigen\Dense>

#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

#include <mlModuleIncludes.h>

ML_START_NAMESPACE


//! 
class ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORT PinholeDeviceFiducialDetection : public Module
{
public:

  //! Constructor.
  PinholeDeviceFiducialDetection();
  bool input2DImageToEigenMAtrix(unsigned inputNumber, Eigen::MatrixXf& inputImage2D);

  std::vector<std::pair<double, double>> FindFiducialsMassCenter(Eigen::MatrixXf& inputImage2D, Eigen::MatrixXf& Mask);
  static bool sortbysec(std::pair<double, double> &a, std::pair<double, double> &b);
  std::string  VectorToXmarkerList(std::vector<std::pair<double, double>> coordinates);


  //! Handles field changes of the field \p field.
  virtual void handleNotification (Field* field);

private:

	BoolField*		isAutoApplyFld;
	TriggerField*	ApplyFld;

	bool isAutoApply;

  // ----------------------------------------------------------
  //! \name Module field declarations
  //@{
  // ----------------------------------------------------------

  //! 
  BaseField* _outMarkersFld;
  StringField* _XmarkerListOutFld;

  std::vector<std::pair<double, double>> fiducialsCenters;

  
  Eigen::MatrixXf StaticImageIn;
  Eigen::MatrixXf MaskIn;
  bool ImageLoaded;
  bool MaskLoaded;

  //@}

  // Implements interface for the runtime type system of the ML.
  ML_MODULE_CLASS_HEADER(PinholeDeviceFiducialDetection)
};


ML_END_NAMESPACE