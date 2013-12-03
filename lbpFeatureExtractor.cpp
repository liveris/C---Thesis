#define M_PI 0.318309886183790671538
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "lbpFeatureExtractor.h"
#include "imageUtils.h"
using namespace cv;
using namespace std;

cv::Mat  swapDimMat(cv::Mat src,int drows,int dcols){
	Mat dest = Mat::zeros(drows, dcols, CV_8UC1);
	//cv::Mat dest(src.cols, src.rows, src.type());

	for(int k=0;k<(dcols/drows);k++){
		for(int i=0;i<drows;i++){
			for (int j=0;j<drows;j++)
			{
				/*cout<<i<<endl;
				cout<<j<<endl;*/
				dest.at<unsigned char>(i, j+(drows*k)) =src.at<unsigned char>(i+(drows*k), j);
				//dest.at<unsigned char>(i, j+60) =src.at<unsigned char>(i+60, j);
				//dest.at<unsigned char>(i, j+120) =src.at<unsigned char>(i+120, j);
				//dest.at<unsigned char>(i, j+180) =src.at<unsigned char>(i+180, j);
			}
		}
	}

	return dest;
}

const int lookup[256] = {
	0, 1, 2, 3, 4, 58, 5, 6, 7, 58, 58, 58, 8, 58, 9, 10,
	11, 58, 58, 58, 58, 58, 58, 58, 12, 58, 58, 58, 13, 58, 14, 15,
	16, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	17, 58, 58, 58, 58, 58, 58, 58, 18, 58, 58, 58, 19, 58, 20, 21,
	22, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	23, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	24, 58, 58, 58, 58, 58, 58, 58, 25, 58, 58, 58, 26, 58, 27, 28,
	29, 30, 58, 31, 58, 58, 58, 32, 58, 58, 58, 58, 58, 58, 58, 33,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 34,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 35,
	36, 37, 58, 38, 58, 58, 58, 39, 58, 58, 58, 58, 58, 58, 58, 40,
	58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 41,
	42, 43, 58, 44, 58, 58, 58, 45, 58, 58, 58, 58, 58, 58, 58, 46,
	47, 48, 58, 49, 58, 58, 58, 50, 51, 52, 58, 53, 54, 55, 56, 57 };


	cv::Mat LbpFeatureExtractor::extractOlbpFeaturesHist() {
		Mat hist=Mat::zeros(1,256,CV_32F); // 256 bins, float for later interpolation
		//cv::Mat dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);
		for(int i=1;i<image.rows-1;i++) {
			for(int j=1;j<image.cols-1;j++) {
				int center = image.at<int>(i,j);
				int code = 0;int code2=0;
				code |= (image.at<int>(i-1,j-1) > center) << 7;
				code |= (image.at<int>(i-1,j) > center) << 6;
				code |= (image.at<int>(i-1,j+1) > center) << 5;
				code |= (image.at<int>(i,j+1) > center) << 4;
				code |= (image.at<int>(i+1,j+1) > center) << 3;
				code |= (image.at<int>(i+1,j) > center) << 2;
				code |= (image.at<int>(i+1,j-1) > center) << 1;
				code |= (image.at<int>(i,j-1) > center) << 0;
				// dst.at<unsigned char>(i-1,j-1) = code;
				//cout << "center code value: " << code << endl;
				hist.at<float>(0,code) += 1; 
			}
		}
		return hist;
	}

	cv::Mat patchLbp(cv::Mat img,bool isUniform) {
		int len = isUniform ? 59 : 256;
		Mat hist=Mat::zeros(1,len,CV_32F); // 256 bins, float for later interpolation
		//cv::Mat dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);
		for(int i=1;i<img.rows-1;i++) {
			for(int j=1;j<img.cols-1;j++) {
				int center = img.at<int>(i,j);
				int code = 0;
				code |= (img.at<int>(i-1,j-1) > center) << 7;
				code |= (img.at<int>(i-1,j) > center) << 6;
				code |= (img.at<int>(i-1,j+1) > center) << 5;
				code |= (img.at<int>(i,j+1) > center) << 4;
				code |= (img.at<int>(i+1,j+1) > center) << 3;
				code |= (img.at<int>(i+1,j) > center) << 2;
				code |= (img.at<int>(i+1,j-1) > center) << 1;
				code |= (img.at<int>(i,j-1) > center) << 0;
				// dst.at<unsigned char>(i-1,j-1) = code;
				//cout << "center code value: " << code << endl;
				// 
				if(isUniform){
					hist.at<float>(0,lookup[code]) += 1;
					//cout << "uniform " << hist << endl;
				}
				else{
					hist.at<float>(0,code) += 1;
					//cout << "not uniform " << hist << endl;
				}
			}
		}
		return hist;
	}

	cv::Mat LbpFeatureExtractor::extractUlbpFeaturesHist() {

		Mat hist=Mat::zeros(1,59,CV_32F); // 256 bins, float for later interpolation
		//cv::Mat dst = Mat::zeros(src.rows-2, src.cols-2, CV_8UC1);
		for(int i=1;i<image.rows-1;i++) {
			for(int j=1;j<image.cols-1;j++) {
				int center = image.at<int>(i,j);
				int code = 0;
				code |= (image.at<int>(i-1,j-1) > center) << 7;
				code |= (image.at<int>(i-1,j) > center) << 6;
				code |= (image.at<int>(i-1,j+1) > center) << 5;
				code |= (image.at<int>(i,j+1) > center) << 4;
				code |= (image.at<int>(i+1,j+1) > center) << 3;
				code |= (image.at<int>(i+1,j) > center) << 2;
				code |= (image.at<int>(i+1,j-1) > center) << 1;
				code |= (image.at<int>(i,j-1) > center) << 0;
				// dst.at<unsigned char>(i-1,j-1) = code;
				//cout << "center code value: " << code << endl;
				hist.at<float>(0,lookup[code]) += 1;
				//cout << "hist value: " << hist.at<float>(0,lookup[code]) << endl;
				//cout << "at pos: " <<(lookup[code]) << endl;
				//cout << "Hist " <<hist << endl;

				//
			}
		}
		return hist;
	}
	cv::Mat LbpFeatureExtractor::extractLbpFeaturesImage() {
		//   Mat hist(1,256,CV_32F); // 256 bins, float for later interpolation
		cv::Mat dst = Mat::zeros(image.rows-2, image.cols-2, CV_8UC1);
		for(int i=1;i<image.rows-1;i++) {
			for(int j=1;j<image.cols-1;j++) {
				unsigned char center = image.at< unsigned char>(i,j);
				unsigned char code = 0;
				code |= (image.at< unsigned char>(i-1,j-1) > center) << 7;
				code |= (image.at< unsigned char>(i-1,j) > center) << 6;
				code |= (image.at< unsigned char>(i-1,j+1) > center) << 5;
				code |= (image.at< unsigned char>(i,j+1) > center) << 4;
				code |= (image.at< unsigned char>(i+1,j+1) > center) << 3;
				code |= (image.at< unsigned char>(i+1,j) > center) << 2;
				code |= (image.at< unsigned char>(i+1,j-1) > center) << 1;
				code |= (image.at< unsigned char>(i,j-1) > center) << 0;
				dst.at<unsigned char>(i-1,j-1) = code;
				//	hist.at<float>(0,code) += 1; 
			}
		}
		return dst;
	}


	cv::Mat LbpFeatureExtractor::spatialHist(bool isUniform ) {
		// calculate LBP patch size
		int len = isUniform ? 59 : 256;
		int width = image.cols/gridx;
		int height = image.rows/gridy;
		Mat resultTemp = Mat::zeros(0, 0, CV_32F);
		Mat result = Mat::zeros(0, 0, CV_32F);
		// iterate through grid
		for(int i = 0; i < gridy; i++) {
			for(int j = 0; j < gridx; j++) {
				Mat patch(image, Range(i*height,(i+1)*height), Range(j*width,(j+1)*width));
				Mat hist = Mat::zeros(1,len,CV_32F);
				// !! your code here (minus the img loading) :  lbp(patch,hist,lookup);
				hist=patchLbp(patch,isUniform);
				result.push_back(hist);
				//result=resultTemp.reshape(0,1);
			}
		}
		return result.reshape(0,1);
	}


	cv::Mat patchELbp(Mat src) {
		cv::Mat hist = Mat::zeros(1,256,CV_32F); 
		cv::Mat image32f;
		src.convertTo(image32f,CV_32F);
		int neighbors=8;
		int radius=2;
		neighbors = max(min(neighbors,31),1); // set bounds...
		// Note: alternatively you can switch to the new OpenCV Mat_
		// type system to define an unsigned int matrix... I am probably
		// mistaken here, but I didn't see an unsigned int representation
		// in OpenCV's classic typesystem...
		Mat dst = Mat::zeros(src.rows-2*radius, src.cols-2*radius, CV_8UC1);
		for(int n=0; n<neighbors; n++) {
			// sample points
			float x = static_cast<float>(radius) * cos(2.0*M_PI*n/static_cast<float>(neighbors));
			float y = static_cast<float>(radius) * -sin(2.0*M_PI*n/static_cast<float>(neighbors));
			// relative indices
			int fx = static_cast<int>(floor(x));
			int fy = static_cast<int>(floor(y));
			int cx = static_cast<int>(ceil(x));
			int cy = static_cast<int>(ceil(y));
			// fractional part
			float ty = y - fy;
			float tx = x - fx;
			// set interpolation weights
			float w1 = (1 - tx) * (1 - ty);
			float w2 =      tx  * (1 - ty);
			float w3 = (1 - tx) *      ty;
			float w4 =      tx  *      ty;
			// iterate through your data
			for(int i=radius; i < src.rows-radius;i++) {
				for(int j=radius;j < src.cols-radius;j++) {
					float t = w1*image32f.at<float>(i+fy,j+fx) + w2*image32f.at<float>(i+fy,j+cx) + w3*image32f.at<float>(i+cy,j+fx) + w4*image32f.at<float>(i+cy,j+cx);
					float var;
					// we are dealing with floating point precision, so add some little tolerance
					var=((t > image32f.at<float>(i,j)) && (abs(t-image32f.at<float>(i,j)) > std::numeric_limits<float>::epsilon())) << n;
					cout<<var<<endl;
					//dst.at<float>(i-radius,j-radius) += var;
					//cout<<dst.at<float>(i-radius,j-radius)<<endl;
					hist.at<float>(0,var) += 1;
				}
			}
		}
		return (hist);
	}

	cv::Mat LbpFeatureExtractor::spatialEHist( ) {
		// calculate LBP patch size
		int len = 256;
		int width = image.cols/gridx;
		int height = image.rows/gridy;
		Mat resultTemp = Mat::zeros(0, 0, CV_32F);
		Mat result = Mat::zeros(0, 0, CV_32F);
		// iterate through grid
		for(int i = 0; i < gridy; i++) {
			for(int j = 0; j < gridx; j++) {
				Mat patch(image, Range(i*height,(i+1)*height), Range(j*width,(j+1)*width));
				Mat hist = Mat::zeros(1,len,CV_32F);
				// !! your code here (minus the img loading) :  lbp(patch,hist,lookup);
				hist=patchELbp(patch);
				cout<<hist<<endl;
				result.push_back(hist);
				//result=resultTemp.reshape(0,1);
			}
		}
		return result.reshape(0,1);
	}



	cv::Mat LbpFeatureExtractor::extractELbpFeaturesImage() {
		Mat hist = Mat::zeros(1,256,CV_32F); 
		cout<<hist<<endl;
		cv::Mat image32f;
		image.convertTo(image32f,CV_32F);
		int neighbors=8;
		int radius=2;
		neighbors = max(min(neighbors,31),1); // set bounds...
		// Note: alternatively you can switch to the new OpenCV Mat_
		// type system to define an unsigned int matrix... I am probably
		// mistaken here, but I didn't see an unsigned int representation
		// in OpenCV's classic typesystem...
		Mat dst = Mat::zeros(image.rows-2*radius, image.cols-2*radius, CV_32F);
		for(int n=0; n<neighbors; n++) {
			// sample points
			float x = static_cast<float>(radius) * cos(2.0*M_PI*n/static_cast<float>(neighbors));
			float y = static_cast<float>(radius) * -sin(2.0*M_PI*n/static_cast<float>(neighbors));
			// relative indices
			int fx = static_cast<int>(floor(x));
			int fy = static_cast<int>(floor(y));
			int cx = static_cast<int>(ceil(x));
			int cy = static_cast<int>(ceil(y));
			// fractional part
			float ty = y - fy;
			float tx = x - fx;
			// set interpolation weights
			float w1 = (1 - tx) * (1 - ty);
			float w2 =      tx  * (1 - ty);
			float w3 = (1 - tx) *      ty;
			float w4 =      tx  *      ty;
			// iterate through your data
			for(int i=radius; i < image.rows-radius;i++) {
				for(int j=radius;j < image.cols-radius;j++) {
					float t = w1*image32f.at<float>(i+fy,j+fx) + w2*image32f.at<float>(i+fy,j+cx) + w3*image32f.at<float>(i+cy,j+fx) + w4*image32f.at<float>(i+cy,j+cx);
					float var;
					// we are dealing with floating point precision, so add some little tolerance
					var=((t > image32f.at<float>(i,j)) && (abs(t-image32f.at<float>(i,j)) > std::numeric_limits<float>::epsilon())) << n;
					//cout<<var<<endl;
					dst.at<float>(i-radius,j-radius) += var;
					//cout<<dst.at<float>(i-radius,j-radius)<<endl;
					//hist.at<float>(0,var) += 1;
					//cout<<hist<<endl;//  
				}
			}
		}
		return mat2gray(dst);
	}


