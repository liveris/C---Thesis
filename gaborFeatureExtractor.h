#if !defined GABORFEATUREEXTRACTOR
#define GABORFEATUREEXTRACTOR

#include <iostream>
#include <opencv2/core/core.hpp>
#include "imageUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
class GaborFeatureExtractor {

private:

	cv::Mat imageRegion;
	int lambda;
public:
	bool viz;
	// constructor
	GaborFeatureExtractor(cv::Mat image,int l){
     imageRegion=image;
	 lambda=l;
	}

	// empty destructor
	~GaborFeatureExtractor() { 

		// default parameter initialization here
	}

	// Extract the facial regions
	Mat GaborFeatureExtractor::extractFeatures();
	Mat GaborFeatureExtractor::extractFeaturesImage();

};


#endif