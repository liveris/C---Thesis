#if !defined LTPFEATUREEXTRACTOR
#define LTPFEATUREEXTRACTOR

#include <iostream>
#include <opencv2/core/core.hpp>
#include "imageUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
class LtpFeatureExtractor {

private:

	cv::Mat imageRegion;
	int radius;
	float threshold;


public:
	bool viz;
	unsigned short lut[8][3];
	// constructor
	LtpFeatureExtractor(cv::Mat image, int r, float t) { 
		 viz=false;
		 radius=r;
		 threshold=t;
		 LtpFeatureExtractor::init();
	}

	// empty destructor
	~LtpFeatureExtractor() { 

		// default parameter initialization here
	}

	// Extract the facial regions
	void LtpFeatureExtractor::extractFeatures(cv::Mat src, cv::Mat dst);
	void LtpFeatureExtractor::init();
};


#endif