
#ifndef IMAGEUTILS
#define IMAGEUTILS
// OpenCV
#include <opencv2/core/core.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;
static Mat norm_0_255(InputArray _src); 

// take number image type number (from cv::Mat.type()), get OpenCV's enum string.
string getImgType(int imgTypeInt);
Mat mat2gray(const Mat& src);
Mat convert2gray(Mat img);
Mat  cropImage( Mat img,  CvRect region);
Mat resizeImage( Mat origImg, int newWidth,	int newHeight);
Mat getHist(cv::Mat img);
void showHistogram(Mat& img);
#endif