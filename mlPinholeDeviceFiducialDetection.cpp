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

#include "mlPinholeDeviceFiducialDetection.h"
#include <algorithm>
using namespace  std;

ML_START_NAMESPACE

//! Implements code for the runtime type system of the ML
ML_MODULE_CLASS_SOURCE(PinholeDeviceFiducialDetection, Module);

//----------------------------------------------------------------------------------

PinholeDeviceFiducialDetection::PinholeDeviceFiducialDetection() : Module(2, 0)
{
  // Suppress calls of handleNotification on field changes to
  // avoid side effects during initialization phase.
  handleNotificationOff();

  // Add fields to the module and set their values.
  _outMarkersFld = addBase("outMarkers", NULL);

  isAutoApplyFld = addBool("isAutoApply",true);
  ApplyFld = addTrigger("ApplyFld");

  _XmarkerListOutFld = addString("XMarkerList_output", "");

  ImageLoaded = false;
  MaskLoaded = false;
  isAutoApply = true;

  // Reactivate calls of handleNotification on field changes.
  handleNotificationOn();


}

//----------------------------------------------------------------------------------

void PinholeDeviceFiducialDetection::handleNotification(Field* field)
{
  // Handle changes of module parameters and input image fields here.

	// Image
	if (field == getInputImageField(0))
	{
		if (input2DImageToEigenMAtrix(0, StaticImageIn))
		{
			ImageLoaded = true;
		}
		else
			ImageLoaded = false;
	}

	// Mask
	if (field == getInputImageField(1))
	{
		if (input2DImageToEigenMAtrix(1, MaskIn))
		{
			MaskLoaded = true;
		}
		else
			MaskLoaded = false;
	}

	if (field == isAutoApplyFld)
	{
		isAutoApply = isAutoApplyFld->getBoolValue();
	}

	if (field == ApplyFld || isAutoApply )
	{
		if (ImageLoaded && MaskLoaded)
		{
			fiducialsCenters = FindFiducialsMassCenter(StaticImageIn, MaskIn);
			// update the output
			_XmarkerListOutFld->updateStringValue(VectorToXmarkerList(fiducialsCenters));
		}
	}

}

string  PinholeDeviceFiducialDetection::VectorToXmarkerList(vector<pair<double, double>> coordinates)
{

	stringstream reply;
	unsigned counter = 1;
	reply << "[";

	for (auto &element : coordinates)
	{
		if (&element != &coordinates.back())
			reply << "(" << element.first << " " << element.second << " 0) #" << to_string(counter++) << ", ";
		else
			reply << "(" << element.first << " " << element.second << " 0) #" << to_string(counter++);

	}

	reply << "]";

	//for (auto element : coordinates)
	//	reply << element.first << " " << element.second << endl;


	return reply.str();
}


ML_END_NAMESPACE