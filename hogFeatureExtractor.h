#if !defined HOGFEATUREEXTRACTOR
#define HOGFEATUREEXTRACTOR

#include <iostream>
#include <opencv2/core/core.hpp>
#include "imageUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
class HogFeatureExtractor {

private:

	cv::Mat imageRegion;
	int nbins,derivAper;
	double sigma,L2HysThresh,histogramNormType,gammalCorrection;
		int blockWidth, blockHeight,blockStrideWidth,blockStrideHeight, cellWidth, cellHeight, winHeight,winWidth;
public:
	bool viz;
	// constructor
	HogFeatureExtractor(cv::Mat image, //winSize
		int bWidth,int bHeight, //blocksize
		int bStrideWidth,int bStrideHeight, //blockStride,
		int cWidth,int cHeight //cellSize,
		) { 
		 viz=false;
		 winWidth=image.cols;
		 winHeight=image.rows;
		 imageRegion=image;
		 nbins=8; //nbins,
		derivAper=0; //derivAper,
		sigma=-1; //winSigma,
		histogramNormType=0;
		L2HysThresh=0.2;
		gammalCorrection=0;
		imageRegion=image;
		blockWidth=bWidth;
		blockHeight=bHeight;
		blockStrideWidth=bStrideWidth;
		blockStrideHeight=bStrideHeight;
		cellWidth=cWidth;
		cellHeight=cHeight;

		//nlevels=64
		// default parameter initialization here
	}

	// empty destructor
	~HogFeatureExtractor() { 

		// default parameter initialization here
	}

	// Extract the facial regions
	vector<float> HogFeatureExtractor::extractFeatures();
	cv::Mat HogFeatureExtractor::getHogVis(vector<float> descriptors);
	cv::Mat HogFeatureExtractor::convertDescriptor2Mat(vector<float> descriptors);
};


#endif