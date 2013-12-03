#if !defined FACIALREGIONEXTRACTOR
#define FACIALREGIONEXTRACTOR

#include <iostream>
#include <opencv2/core/core.hpp>
#include "imageUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
class FacialRegionExtractor {

private:

	cv::Point2f rightEye,leftEye;
	cv::Mat face;

public:
	bool viz;
	// constructor
	FacialRegionExtractor(cv::Point2f right_eye,cv::Point2f left_eye,cv::Mat image,bool bviz) { 
		 //viz=false;
		 leftEye=left_eye;
		 rightEye=right_eye;
		 face=image;
		 viz=bviz;
		// default parameter initialization here
	}

	// empty destructor
	~FacialRegionExtractor() { 

		// default parameter initialization here
	}

	// Extract the facial regions
	cv::Mat FacialRegionExtractor::getLeftEyeRegion();

	cv::Mat FacialRegionExtractor::getFace();

	cv::Mat FacialRegionExtractor::getRightEyeRegion();

	cv::Mat FacialRegionExtractor::getMouthRegion();

	cv::Mat FacialRegionExtractor::getNoseRegion();

	cv::Mat FacialRegionExtractor::getLowResFace();

	cv::Mat FacialRegionExtractor::getNoseEyeRegion();

	cv::Mat FacialRegionExtractor::getLowResFace( int width,int height);
};


#endif