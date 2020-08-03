#include "mlPinholeDeviceFiducialDetection.h"


using namespace std;

ML_START_NAMESPACE


bool PinholeDeviceFiducialDetection::input2DImageToEigenMAtrix(unsigned inputNumber, Eigen::MatrixXf& inputImage2D)
{
	bool reply = false;

	if ((getUpdatedInputImage(inputNumber) != NULL) && getUpdatedInputImage(inputNumber)->isValid())
	{
		const ml::PagedImage* pi = getUpdatedInputImage(inputNumber);

		// always requires a valid input image
		ML_CHECK(pi);

		const ImageVector iSize = pi->getImageExtent();

		if (!(iSize == Vector(0, 0, 0, 0, 0, 0)))
		{
			// Init the slice data pointer here since we want to reuse the allocated slice memory later.
			void *sliceData = NULL;

			// get sub image box for current slice
			ml::SubImageBox sliceSib = pi->getBoxFromImageExtent();

			MLDataType imageDT = pi->getDataType();
			// Get the current slice from the buffer.  
			MLErrorCode err = getTile(getInputImage(inputNumber), sliceSib, imageDT, &sliceData);

			TSubImage<MLuint8> slice;
			if (pi->getDataType() == 1)
				slice = TSubImage<MLuint8>(sliceSib.getExtent(), pi->getDataType(), sliceData);
			else //if (pi->getDataType() == 3 )
				slice = TSubImage<MLuint16>(sliceSib.getExtent(), pi->getDataType(), sliceData);

			err;


			if (err == ML_RESULT_OK)
			{
				inputImage2D.resize(iSize.x, iSize.y);

				for (unsigned int y = 0; y < iSize.y; ++y)
				{
					for (unsigned int x = 0; x < iSize.x; ++x)
					{
						inputImage2D(x, y) = slice.getImageValue(x, y, 0);
					}
				}
			}


			reply = true;


			// Release the allocated memory.
			if (sliceData)
			{
				freeTile(sliceData);
				sliceData = NULL;
			}
		}
	}

	return reply;

}


vector<pair<double, double>> PinholeDeviceFiducialDetection::FindFiducialsMassCenter(Eigen::MatrixXf& inputImage2D, Eigen::MatrixXf& Mask)
{
	// segment the values in individual arrays

	// first vector is the level of fiducials, from 1 to 12
	// second vector contains each individual pixel (as a pair)
	vector<vector <pair<unsigned, unsigned>>> indicesOfFiducialsPositionsOnImage;
	vector <pair<double, double>> indicesOfFiducialsCenters;
	unsigned numberOfFiducials = static_cast<unsigned>(Mask.maxCoeff());
	indicesOfFiducialsPositionsOnImage.resize(numberOfFiducials);

	for (unsigned j = 0; j < Mask.cols(); ++j)
	{
		for (unsigned i = 0; i < Mask.rows(); ++i)
		{
			if (Mask(i, j) != 0)
			{
				unsigned currentFiducial = Mask(i, j) - 1;
				pair<unsigned, unsigned> coordinate = pair<unsigned, unsigned>(i, j);
				indicesOfFiducialsPositionsOnImage.at(currentFiducial).push_back(coordinate);
			}
		}
	}

	// Find the mass center of each array

	for (auto currentFiducialCoordinates : indicesOfFiducialsPositionsOnImage)
	{
		double summatoryOfIntensities = 0;
		double weightedSummatoryRowCoordinates = 0;
		double weightedSummatoryColCoordinates = 0;
		double summatoryRowCoordinates = 0;
		double summatoryColCoordinates = 0;
		double numberOfCoordinates = 0;

		//cout << "Start of Fiducial:" << endl;
		for (auto currentCoordinate : currentFiducialCoordinates)
		{
			double currentRowCoordinate = currentCoordinate.first;
			double currentColCoordinate = currentCoordinate.second;
			double currentIntensity = 255 - inputImage2D(currentRowCoordinate, currentColCoordinate);
			//cout << "(" << currentRowCoordinate << " " << currentColCoordinate << "): " << currentIntensity << endl;
			summatoryOfIntensities += currentIntensity;
			summatoryRowCoordinates += currentRowCoordinate;
			summatoryColCoordinates += currentColCoordinate;
			weightedSummatoryRowCoordinates += currentIntensity*currentRowCoordinate;
			weightedSummatoryColCoordinates += currentIntensity*currentColCoordinate;
			numberOfCoordinates++;
		}

		if (!currentFiducialCoordinates.empty())
		{
			double massCenterRow = (weightedSummatoryRowCoordinates / summatoryOfIntensities) + 0.5;
			double massCenterCol = (weightedSummatoryColCoordinates / summatoryOfIntensities) + 0.5;
			//cout << "Mass Center: (" << massCenterRow << " " << massCenterCol << ")" << endl;
			double centerRow = (summatoryRowCoordinates / numberOfCoordinates) + 0.5;
			double centerCol = (summatoryColCoordinates / numberOfCoordinates) + 0.5;
			//cout << "Geometric Center: (" << centerRow << " " << centerCol << ")" << endl;

			//pair<double, double> currentFiducialCenter = pair<double, double>(massCenterRow, massCenterCol);
			pair<double, double> currentFiducialCenter = pair<double, double>((massCenterRow+ centerRow)/2, (massCenterCol+ centerCol)/2);
			indicesOfFiducialsCenters.push_back(currentFiducialCenter);
		}
	}

	sort(indicesOfFiducialsCenters.begin(), indicesOfFiducialsCenters.end(), sortbysec);

	return indicesOfFiducialsCenters;
}

// Driver function to sort the vector elements 
// by second element of pairs 
bool PinholeDeviceFiducialDetection::sortbysec(pair<double, double> &a, pair<double, double> &b)
{
	return (a.second < b.second);
}


ML_END_NAMESPACE