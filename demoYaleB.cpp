//
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <ctime>
//
//#include <list>
//
//#if MSVC
//#include <windows.h>
//#endif
//
//#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include "crowdsight.h"
//#include "imageUtils.h"
//#include "lbpUniform.h"
//#include "facePreprocessor.h"
//#include "facialRegionExtractor.h"
//#include "hogFeatureExtractor.h"
//#include "lbpFeatureExtractor.h"
//#include "GaborFeatureExtractor.h"
//#include <string>
//#include "ltpFeatureExtractor.h"
//#if defined( _MSC_VER )
//#define snprintf _snprintf
//#endif
//
//#ifndef HUMAN_NAME
//// Extract date and time and append to window name
//std::time_t result = std::time( NULL );
//char buf[50];
//int a = snprintf( buf,
//	sizeof buf,
//	"%s%s",
//	"CrowdSight Demo\t",
//	std::asctime( std::localtime(&result) ) );
//#define HUMAN_NAME buf
//#endif
//
//// Turn on/off recording of output video and csv
//#define RECORDING 0
////#define DESIRED_RIGHT_EYE_X 0.25
//
//
//using namespace cv;
//using namespace std;
//
//cv::Mat tan_triggs_preprocessing(InputArray src,
//	float alpha, float tau , float gamma, int sigma0,
//	int sigma1) {
//
//		// Convert to floating point:
//		Mat X = src.getMat();
//		X.convertTo(X, CV_32FC1);
//		// Start preprocessing:
//		Mat I;
//		pow(X, gamma, I);
//		// Calculate the DOG Image:
//		{
//			Mat gaussian0, gaussian1;
//			// Kernel Size:
//			int kernel_sz0 = (3*sigma0);
//			int kernel_sz1 = (3*sigma1);
//			// Make them odd for OpenCV:
//			kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
//			kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
//			GaussianBlur(I, gaussian0, Size(kernel_sz0,kernel_sz0), sigma0, sigma0);
//			GaussianBlur(I, gaussian1, Size(kernel_sz1,kernel_sz1), sigma1, sigma1);
//			subtract(gaussian0, gaussian1, I);
//		}
//
//		{
//			double meanI = 0.0;
//			{
//				Mat tmp;
//				pow(abs(I), alpha, tmp);
//				meanI = mean(tmp).val[0];
//
//			}
//			I = I / pow(meanI, 1.0/alpha);
//		}
//
//		{
//			double meanI = 0.0;
//			{
//				Mat tmp;
//				pow(min(abs(I), tau), alpha, tmp);
//				meanI = mean(tmp).val[0];
//			}
//			I = I / pow(meanI, 1.0/alpha);
//		}
//
//		// Squash into the tanh:
//		{
//			for(int r = 0; r < I.rows; r++) {
//				for(int c = 0; c < I.cols; c++) {
//					I.at<float>(r,c) = tanh(I.at<float>(r,c) / tau);
//				}
//			}
//			I = tau * I;
//		}
//		return mat2gray(I);
//}
//
//int main ( int argc, char *argv[] )
//{
//	cv::VideoCapture cap;
//	std::string auth_key;
//	std::string file_name;
//	std::string input;
//	std::string data_dir = "../data/";
//	std::string resources_dir = "../resources/";
//	bool is_webcam_input = false;
//	auth_key = "6e6e3227fc1d4c89afed3635c20b5d40";
//
//	vector<Mat> normLeftEyeRegion,normRightEyeRegion,normNoseRegion,normLowResFaceRegion;
//	vector<Mat> vHogLres,vHogReye,vHogLeye,vHogNose,vLbpLres,vLbpReye,vLbpLeye,vLbpNose,vHogFace,vEqualizedNorm,vGabor;
//	vector<float> headPose,headYaw;
//	vector <int> labels;
//	// Create crowdsight instance
//	CrowdSight crowdsight( data_dir );
//
//	// Authenticate crowdsight instance
//	if ( !crowdsight.authenticate( auth_key ) )
//	{
//		std::cerr << crowdsight.getErrorDescription() << std::endl;
//		std::cout << "hello" << std::endl;
//		return -1;
//	}
//
//	static CvScalar colors[] =
//	{
//		{{0,   0,   255}},
//		{{0,   128, 255}},
//		{{0,   255, 255}},
//		{{0,   255, 0  }},
//		{{255, 128, 0  }},
//		{{255, 255, 0  }},
//		{{255, 0,   0  }},
//		{{255, 0,   255}}
//	};
//
//	// Get the path to your CSV.
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/orl_faces/data_faces.csv";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/colorferet/dvd1/data/images/faces_colorferet_train_all_withoutprofile_withouthalf_withoutrerd.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/faces/faces_0_30.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/pubfig83lfw_raw_in_dirs/pubfig83lfw_raw_in_dirs/train/faces_pubfig83lfw_test.txt";
//	string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/faces_yaleBCropped.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/lfw/faces_lfw_5.txt";
//	// These vectors hold the images and corresponding labels.
//
//	// Read in the data. This can fail if no valid
//	// input filename is given.
//	std::ifstream file(fn_csv.c_str(), ifstream::in);
//	if (!file) {
//		string error_message = "No valid input file was given, please check the given filename.";
//		CV_Error(CV_StsBadArg, error_message);
//	}
//	int counter=0,icounter=0;
//	char separator = ';';
//	string line, path, classlabel;std::string filename;
//	int i=1;
//	while (getline(file, line)) {
//		stringstream liness(line);
//		getline(liness, path, separator);
//		getline(liness, classlabel);
//		//path="C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/pubfig83lfw_raw_in_dirs/pubfig83lfw_raw_in_dirs/test/Harrison Ford/test__000034-000011.jpg";
//		//iterate through all images
//		//for ( unsigned int i = 0; i < images.size();++i )
//		//{
//		cv::Mat frame;
//		int label;
//		i++ ;
//		// Grab one frame
//		if(!path.empty() && !classlabel.empty()) {
//			frame = ((imread(path,1)));
//			label=(atoi(classlabel.c_str()));
//			filename= path;
//		}
//		 //frame = imread("D:/CroppedYale/CroppedYale/yaleB11/yaleB11_P00A+000E+00.pgm",1);
//		// frame = imread("original.jpg",1);
//		cout<<"inner counter "<<icounter++<<endl;// Get person
//
//		//Mat noseEyeRegion=fextractor.getNoseEyeRegion();
//		//imshow("Nose Eye region",faceImage);
//		//Mat faceImage=(mat2gray(frame));
//		 Mat greyFrame=convert2gray(frame);
//		Mat faceImage=resizeImage(greyFrame,72,80);
//		faceImage=tan_triggs_preprocessing(faceImage,0.1, 10.0,0.2,1,2);
//		imshow("Tan",faceImage);
//		namedWindow( "Tan", CV_WINDOW_KEEPRATIO );
//			Mat gaborCropped= resizeImage(faceImage,30,30);
//		imshow("gabor cropped",gaborCropped);
//		namedWindow( "gabor cropped", CV_WINDOW_KEEPRATIO );
//		waitKey(1);
//		//-----Resize them to extract Hog and Lbp features----------------
//		// add the labels
//		labels.push_back(label);
//
//		//headPose.push_back(person.getHeadPitch());
//		//headYaw.push_back(person.getHeadYaw());
//
//		//puth them to vectors for opencv lbp uniform
//		vEqualizedNorm.push_back(faceImage);
//
//		HogFeatureExtractor descFace(faceImage,16,16,8,8,8,8);
//		vector<float> HogFace=descFace.extractFeatures();
//		Mat visHogFace=descFace.convertDescriptor2Mat(HogFace);
//		vHogFace.push_back(visHogFace);
//		//cout<<visHogFace<<endl;
//		imshow("visHog",descFace.getHogVis(visHogFace));
//		waitKey(3);
//		//cout<<visHogFace.rows<<endl;
//
//		//Extract Gabor Features
//		GaborFeatureExtractor gFace(gaborCropped,4);
//		Mat gMat= gFace.extractFeatures();
//			//cout<<gMat<<endl;
//		vGabor.push_back(gMat);
//
//	}
//
//
//	//Create lbp uniform decriptor
//	LBPH lbph(2,8,9,10,DBL_MAX,true);
//	lbph.train(vEqualizedNorm, labels);
//	//FileStorage fs1("lbpUniformFaceTanTrain.yml", FileStorage::WRITE);
//	lbph.save("lbpUniformFaceTanYaleBCropped.yml");
//	//fs1.release();
//
//	//Save the hog face
//	FileStorage fs2("hogFaceTanYaleBCropped.yml", FileStorage::WRITE);
//	fs2 << "TestImages" << vHogFace;
//	fs2 << "LabelImages"<< labels;
//	fs2.release();
//
//	//Save the gabor face
//	FileStorage fs3("gaborFaceTanYaleBCropped.yml", FileStorage::WRITE);
//	fs3 << "TestImages" << vGabor;
//	fs3 << "LabelImages"<< labels;
//	fs3.release();
//
//	return 0; 
//}
//
//
//
