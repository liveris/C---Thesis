#include "imageUtils.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch(src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

// take number image type number (from cv::Mat.type()), get OpenCV's enum string.
string getImgType(int imgTypeInt)
{
	int numImgTypes = 35; // 7 base types, with five channel options each (none or C1, ..., C4)

	int enum_ints[] =       {CV_8U,  CV_8UC1,  CV_8UC2,  CV_8UC3,  CV_8UC4,
		CV_8S,  CV_8SC1,  CV_8SC2,  CV_8SC3,  CV_8SC4,
		CV_16U, CV_16UC1, CV_16UC2, CV_16UC3, CV_16UC4,
		CV_16S, CV_16SC1, CV_16SC2, CV_16SC3, CV_16SC4,
		CV_32S, CV_32SC1, CV_32SC2, CV_32SC3, CV_32SC4,
		CV_32F, CV_32FC1, CV_32FC2, CV_32FC3, CV_32FC4,
		CV_64F, CV_64FC1, CV_64FC2, CV_64FC3, CV_64FC4};

	string enum_strings[] = {"CV_8U",  "CV_8UC1",  "CV_8UC2",  "CV_8UC3",  "CV_8UC4",
		"CV_8S",  "CV_8SC1",  "CV_8SC2",  "CV_8SC3",  "CV_8SC4",
		"CV_16U", "CV_16UC1", "CV_16UC2", "CV_16UC3", "CV_16UC4",
		"CV_16S", "CV_16SC1", "CV_16SC2", "CV_16SC3", "CV_16SC4",
		"CV_32S", "CV_32SC1", "CV_32SC2", "CV_32SC3", "CV_32SC4",
		"CV_32F", "CV_32FC1", "CV_32FC2", "CV_32FC3", "CV_32FC4",
		"CV_64F", "CV_64FC1", "CV_64FC2", "CV_64FC3", "CV_64FC4"};

	for(int i=0; i<numImgTypes; i++)
	{
		if(imgTypeInt == enum_ints[i]) return enum_strings[i];
	}
	return "unknown image type";
}


Mat mat2gray(const Mat& src)
{
	Mat dst;
	switch(src.channels()) {
	case 1:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}

Mat convert2gray(Mat img){
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_BGR2GRAY);
	}
	else if (img.channels() == 4) {
		cvtColor(img, gray, CV_BGRA2GRAY);
	}
	else {
		// Access the grayscale input image directly.
		gray = img;
	}
	return gray;
}

// Returns a new image that is a cropped version (rectangular cut-out)
// of the original image.
Mat  cropImage( Mat img,  CvRect region)
{
	if (img.cols <= 0 || img.rows <= 0
		|| region.width <= 0 || region.height <= 0) {
			cerr << "ERROR in cropImage(): invalid dimensions." << endl;
			//exit(1);
	}
	// Set the desired region of interest.
	Mat image_roi = img(region);

	return image_roi;
}

// Creates a new image copy that is of a desired size. The aspect ratio will
// be kept constant if 'keepAspectRatio' is true, by cropping undesired parts
// so that only pixels of the original image are shown, instead of adding
// extra blank space.
// Remember to free the new image later.
Mat resizeImage( Mat origImg, int newWidth,	int newHeight)
{
	Mat outImg;
	int origWidth;
	int origHeight;
	origWidth = origImg.cols;
	origHeight = origImg.rows;
	if (newWidth <= 0 || newHeight <= 0 || origWidth <= 0 || origHeight <= 0) {
		cerr << "ERROR: Bad desired image size of " << newWidth;
		//	<< "x" << newHeight << " in resizeImage().\n";
		//exit(1);
	}
	// Resize the image without changing its aspect ratio,
	// by cropping off the edges and enlarging the middle section.
	CvRect r;
	// input aspect ratio
	float origAspect = (origWidth / (float)origHeight);
	// output aspect ratio
	float newAspect = (newWidth / (float)newHeight);
	// crop width to be origHeight * newAspect
	if (origAspect > newAspect) {
		int tw = (origHeight * newWidth) / newHeight;
		r = cvRect((origWidth - tw)/2, 0, tw, origHeight);
	}
	else {	// crop height to be origWidth / newAspect
		int th = (origWidth * newHeight) / newWidth;
		r = cvRect(0, (origHeight - th)/2, origWidth, th);
	}
	Mat croppedImg = cropImage(origImg, r);

	// Call this function again, with the new aspect ratio image.
	// Will do a scaled image resize with the correct aspect ratio.
	CvSize size;
	size.width=newWidth;
	size.height= newHeight;
	//outImg = resizeImage(croppedImg, newWidth, newHeight, false);
	resize( croppedImg, outImg, size ); 
	//cvReleaseImage( &croppedImg );
	return outImg;
}

cv::Mat getHist(cv::Mat img)
{
	int histSize = img.cols;
	float range[] = { 0, img.cols} ;
	const float* histRange = { range }; 

	bool uniform = true; bool accumulate = false;

	Mat hist;

	calcHist( &img, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

	int hist_w = 600; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	// Draw 
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
			Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
			Scalar( 255, 0, 0), 2, 8, 0  );
	}


	//imshow("LBP image Histogram", histImage );
	//namedWindow( "LBP image Histogram", CV_WINDOW_KEEPRATIO );
	//waitKey(2);
	return histImage;

}


void showHistogram(Mat& img)
{
	int bins = img.cols;             // number of bins
	int nc = img.channels();    // number of channels
	vector<Mat> hist(nc);       // array for storing the histograms
	vector<Mat> canvas(nc);     // images for displaying the histogram
	int hmax[3] = {0,0,0};      // peak value for each histogram

	// The rest of the code will be placed here
	for (int i = 0; i < hist.size(); i++)
		hist[i] = Mat::zeros(1, bins, CV_32SC1);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			for (int k = 0; k < nc; k++)
			{
				uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
				hist[k].at<int>(val) += 1;
			}
		}
	}
	for (int i = 0; i < nc; i++)
	{
		for (int j = 0; j < bins-1; j++)
			hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
	}
	const char* wname[3] = { "blue", "green", "red" };
	Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

	for (int i = 0; i < nc; i++)
	{
		canvas[i] = Mat::ones(125, bins, CV_8UC3);

		for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
		{
			line(
				canvas[i], 
				Point(j, rows), 
				Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])), 
				nc == 1 ? Scalar(200,200,200) : colors[i], 
				1, 8, 0
				);
		}

		imshow(nc == 1 ? "value" : wname[i], canvas[i]);
		//imwrite("histp.jpg",canvas[i]);
		
	}
}