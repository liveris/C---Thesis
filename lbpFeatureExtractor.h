#if !defined LBPFEATUREEXTRACTOR
#define LBPFEATUREEXTRACTOR

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
class LbpFeatureExtractor {

private:

	cv::Mat image;
	int gridx,gridy;

public:
	bool viz;
	// constructor
	LbpFeatureExtractor(cv::Mat img,int gx,int gy){
		image=img;
		gridx=gx;
		gridy=gy;
		// default parameter initialization here
	}

	// empty destructor
	~LbpFeatureExtractor() { 

		// default parameter initialization here
	}

	// Extract the facial regions
	cv::Mat LbpFeatureExtractor::extractOlbpFeaturesHist();
	cv::Mat LbpFeatureExtractor::extractOlbp2FeaturesHist();
	cv::Mat LbpFeatureExtractor::extractUlbpFeaturesHist();
	cv::Mat LbpFeatureExtractor::extractUlbp2FeaturesHist();
	cv::Mat LbpFeatureExtractor::extractLbpFeaturesImage();
	cv::Mat LbpFeatureExtractor::spatialHist( bool r);
	//void LbpFeatureExtractor::
	//cv::Mat patchLbp(cv::Mat img);
	cv::Mat LbpFeatureExtractor::spatialEHist( );
	cv::Mat LbpFeatureExtractor::extractELbpFeaturesImage();
	// cv::Mat LbpFeatureExtractor::swapDimMat(cv::Mat src,int drows,int dcols);
};


#endif