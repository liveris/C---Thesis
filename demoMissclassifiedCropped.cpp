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
// #include "fssFeatureExtractor.h"
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
//
//Mat imHist(Mat hist, float scaleX=1, float scaleY=1){
//	double maxVal=0;
//	minMaxLoc(hist, 0, &maxVal, 0, 0);
//	int rows = 64; //default height size
//	int cols = hist.rows; //get the width size from the histogram
//	Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
//	//for each bin
//	for(int i=0;i<cols-1;i++) {
//		float histValue = hist.at<float>(i,0);
//		float nextValue = hist.at<float>(i+1,0);
//		Point pt1 = Point(i*scaleX, rows*scaleY);
//		Point pt2 = Point(i*scaleX+scaleX, rows*scaleY);
//		Point pt3 = Point(i*scaleX+scaleX, (rows-nextValue*rows/maxVal)*scaleY);
//		Point pt4 = Point(i*scaleX, (rows-nextValue*rows/maxVal)*scaleY);
//
//		int numPts = 5;
//		Point pts[] = {pt1, pt2, pt3, pt4, pt1};
//
//		fillConvexPoly(histImg, pts, numPts, Scalar(255,255,255));
//	}
//	return histImg;
//}
//
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
//	string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/missClassification256SameRandomFaceTanKnnHogLbpGaborPca3661Seper09NArtTrainPaper.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/corrClassification512SameRandomKnnHogLbpGaborYawPitchPaper.txt";
//
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/missClassification256SameRandomKnnHogLbpGaborYawPitchPaper.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/corrClassification256SameRandomKnnHogLbpGaborYawPitchPaper.txt";
//
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/missClassification256SameRandomNoseFaceCroppedKnnHogLbpGaborYawPitchPaper.txt";
//	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/faceClassifierNCMClean/corrClassification256SameRandomNoseFaceCroppedKnnHogLbpGaborYawPitchPaper.txt";
//	std::ifstream file(fn_csv.c_str(), ifstream::in);
//	if (!file) {
//		string error_message = "No valid input file was given, please check the given filename.";
//		CV_Error(CV_StsBadArg, error_message);
//	}
//
//	int counter=0,icounter=0;
//	char separator = ':';
//	char key=' ';
//	string str;
//	string line,line1s,line1,line2,correctLabel,path, path1,path2, wrongLabel;
//	std::string filename;
//	int i=1;
//	while (getline(file, line)) {
//		unsigned found = line.rfind(separator);
//		line1=line.substr(0,found-2);
//		stringstream liness(line1);
//		getline(liness, path);
//		//getline(liness2, wrongLabel,separator);
//
//		unsigned found1 = line1.find_last_of(key);
//		line1s=line1.substr(0,found1);
//		stringstream liness3(line1s);
//		getline(liness3, path1);
//		//Get the similar image
//		line2=line.substr(found-1);
//		stringstream liness2(line2);
//		getline(liness2, path2);
//		cv::Mat frame;
//		int label;
//		i++ ;
//		cv::Mat frameQ, frameS;
//		// Grab one frame
//		if(!path.empty()) {
//			frameQ = ((imread(path1)));
//			frameS = ((imread(path1)));
//			filename= path;
//		}
//
//		//frame =images[i]; //imread("C:/Users/Liveris/Downloads/lena.jpg");//images[i]; // //Give the full path to the image file#	namedWindow( "Display window 1", CV_WINDOW_KEEPRATIO );// Create a window for display. 
//		// Define region of interest here if required
//		int marginTop     = 0;
//		int marginBottom  = 0;
//		int marginLeft    = 0;
//		int marginRight   = 0;
//		cv::Rect roi;
//
//		int  maxNumPeople = 10;
//
//		//Start main processing loop
//
//		crowdsight.setMaxNumPeople( maxNumPeople );
//
//
//		// If frame is empty break
//		if (frameQ.empty() || frameS.empty() )
//		{
//			break;
//		}
//
//		//cv::pyrDown(frame,frame);
//
//		// Specify the roi that will be extracted from the frame
//		roi.x      = marginLeft;                    // pixels to skip from the left
//		roi.y      = marginTop;                     // pixels to skip from the top
//		roi.width  = frameQ.cols-roi.x-marginRight;  // width of roi
//		roi.height = frameQ.rows-roi.y-marginBottom; // height of roi
//
//		// Change settings if required
//
//		if ( !crowdsight.process(frameQ,roi) )
//		{
//			std::cout << crowdsight.getErrorDescription() << std::endl;
//
//		}
//
//		// Get the list of people in the last processed frame
//		std::vector<Person> people;
//		if ( !crowdsight.getCurrentPeople(people) )
//		{
//			std::cerr << crowdsight.getErrorDescription() << std::endl;
//		}
//
//		cout<<"outer counter "<<counter++<<endl;
//		// For the one person in the frame, do:
//		if (people.size()>0 )
//		{
//			cout<<"inner counter "<<icounter++<<endl;// Get person
//			Person person = people.at(0);
//
//			/*********************************** RETRIEVE PERSN INFO ***********************************/
//
//			cv::Rect    face              = person.getFaceRect();      // Retrieve the person's face
//			cv::Point   right_eye         = person.getRightEye();      // Retrieve left and right eye locations of the person. Eye location is relative to the face rectangle.
//			cv::Point   left_eye          = person.getLeftEye();
//
//			/************************************* DRAW PERSON INFO *************************************/
//
//			// Offset eye position with face position, to get frame coordinates.
//			right_eye.x += face.x;
//			right_eye.y += face.y;
//			left_eye.x  += face.x;
//			left_eye.y  += face.y;
//
//			// Draw circles in the center of left and right eyes
//			//cv::circle( frame, right_eye, 3, cv::Scalar(0,255,0) );
//			//cv::circle( frame, left_eye,  3, cv::Scalar(0,255,0) );
//			Mat faceRect=frameQ(face);
//			// Draw a rectangle around person's face on the current frame
//			//cv::rectangle(frame, face, cv::Scalar(255,255,255), 1);
//
//			//Show image
//			imshow("Qframe Q",frameQ);
//			namedWindow( "Qframe Q", CV_WINDOW_KEEPRATIO );
//			//Framo preprocessor
//			Mat greyFrame=convert2gray(frameQ);
//			//initialize constructor with original eye points
//			FacePreprocessor fprocessor(right_eye,left_eye,faceRect.cols,faceRect.rows);
//			//register image
//			Mat warped=fprocessor.registerImage(greyFrame);
//			//Mat tan=fprocessor.tan_triggs_preprocessing(warped);
//			//imshow("warped",tan);
//			//mean normalize image
//			Mat norm=fprocessor.meanNormalizeImage(warped);
//			Mat equalizedNorm;
//			equalizeHist( norm, equalizedNorm );//Equalize the face image
//			//imshow("norm",norm);
//			//cout<<getImgType(normEllipse.type())<<endl;
//			//imshow("Equalized norm",equalizedNorm);
//			//namedWindow( "Equalized norm", CV_WINDOW_KEEPRATIO );
//			waitKey(1);
//			//Extract facial regions from face
//			bool viz=false;
//			FacialRegionExtractor fextractor(fprocessor.getRegisteredRightEye(fprocessor.getTransformMatrix()),fprocessor.getRegisteredLeftEye(fprocessor.getTransformMatrix()),equalizedNorm,viz);
//
//			//Mat noseEyeRegion=fextractor.getNoseEyeRegion();
//			Mat noseEyeRegion=fextractor.getLowResFace(72,80);
//			//Mat cr=cropImage(noseEyeRegion,cvRect(4,4,72,80));
//			//imshow("Nose Eye region",noseEyeRegion);
//			Mat gaborCropped= resizeImage(noseEyeRegion,30,30);
//			//imshow("gabor cropped",gaborCropped);
//			//namedWindow( "gabor cropped", CV_WINDOW_KEEPRATIO );
//			waitKey(1);
//
//			//Extract hog features from resized facial regions
//
//			HogFeatureExtractor descFace(noseEyeRegion,16,16,8,8,8,8);
//			vector<float> HogFace=descFace.extractFeatures();
//			Mat visHogFace=descFace.convertDescriptor2Mat(HogFace);
//			vHogFace.push_back(visHogFace);
//			imshow("visHog Q",descFace.getHogVis(visHogFace));
//			waitKey(3);
//			cout<<visHogFace.rows<<endl;
//
//
//			//Extract Gabor Features
//
//			GaborFeatureExtractor gFace(gaborCropped,4);
//			Mat gMat= gFace.extractFeaturesImage();
//			imshow("Gabor Q",gMat);
//
//			//		-----LBP features------------
//
//			LbpFeatureExtractor dlbpLres(noseEyeRegion,4,4);
//			Mat visLbpLres=dlbpLres.extractELbpFeaturesImage();
//			imshow("visLbpLres Q",visLbpLres);
//			waitKey(1);
//
//
//
//		}
//				// If frame is empty break
//		if (frameQ.empty() || frameS.empty() )
//		{
//			break;
//		}
//
//				if ( !crowdsight.process(frameS,roi) )
//		{
//			std::cout << crowdsight.getErrorDescription() << std::endl;
//
//		}
//
//		if ( !crowdsight.getCurrentPeople(people) )
//		{
//			std::cerr << crowdsight.getErrorDescription() << std::endl;
//		}
//
//		cout<<"outer counter "<<counter++<<endl;
//		// For the one person in the frame, do:
//		if (people.size()==0 )
//			break;
//		else
//		{
//			cout<<"inner counter "<<icounter++<<endl;// Get person
//			Person person = people.at(0);
//
//			/*********************************** RETRIEVE PERSN INFO ***********************************/
//
//			cv::Rect    face              = person.getFaceRect();      // Retrieve the person's face
//			cv::Point   right_eye         = person.getRightEye();      // Retrieve left and right eye locations of the person. Eye location is relative to the face rectangle.
//			cv::Point   left_eye          = person.getLeftEye();
//
//			/************************************* DRAW PERSON INFO *************************************/
//
//			// Offset eye position with face position, to get frame coordinates.
//			right_eye.x += face.x;
//			right_eye.y += face.y;
//			left_eye.x  += face.x;
//			left_eye.y  += face.y;
//
//			// Draw circles in the center of left and right eyes
//			//cv::circle( frame, right_eye, 3, cv::Scalar(0,255,0) );
//			//cv::circle( frame, left_eye,  3, cv::Scalar(0,255,0) );
//			Mat faceRect=frameS(face);
//			// Draw a rectangle around person's face on the current frame
//			//cv::rectangle(frame, face, cv::Scalar(255,255,255), 1);
//
//			//Show image
//			imshow("frame S",frameS);
//			namedWindow( "frame S", CV_WINDOW_KEEPRATIO );
//			//Framo preprocessor
//			Mat greyFrame=convert2gray(frameS);
//			//initialize constructor with original eye points
//			FacePreprocessor fprocessor(right_eye,left_eye,faceRect.cols,faceRect.rows);
//			//register image
//			Mat warped=fprocessor.registerImage(greyFrame);
//			
//			Mat tan=fprocessor.tan_triggs_preprocessing(warped);
//			//imshow("warped",warped);
//			//mean normalize image
//			Mat norm=fprocessor.meanNormalizeImage(warped);
//			Mat equalizedNorm;
//			equalizeHist( norm, equalizedNorm );//Equalize the face image
//			//imshow("norm",norm);
//			//cout<<getImgType(normEllipse.type())<<endl;
//			//imshow("Equalized norm",equalizedNorm);
//			//namedWindow( "Equalized norm", CV_WINDOW_KEEPRATIO );
//			waitKey(1);
//			//Extract facial regions from face
//			bool viz=false;
//			FacialRegionExtractor fextractor(fprocessor.getRegisteredRightEye(fprocessor.getTransformMatrix()),fprocessor.getRegisteredLeftEye(fprocessor.getTransformMatrix()),tan,viz);
//
//			//Mat noseEyeRegion=fextractor.getNoseEyeRegion();
//			Mat noseEyeRegion=fextractor.getLowResFace(72,80);
//			//imshow("Nose Eye region",noseEyeRegion);
//			Mat gaborCropped= resizeImage(noseEyeRegion,30,30);
//			//imshow("gabor cropped",gaborCropped);
//			//namedWindow( "gabor cropped", CV_WINDOW_KEEPRATIO );
//			waitKey(1);
//
//			//Extract hog features from resized facial regions
//
//			HogFeatureExtractor descFace(noseEyeRegion,16,16,8,8,8,8);
//			vector<float> HogFace=descFace.extractFeatures();
//			Mat visHogFace=descFace.convertDescriptor2Mat(HogFace);
//			vHogFace.push_back(visHogFace);
//			imshow("visHog S",descFace.getHogVis(visHogFace));
//			waitKey(3);
//			cout<<visHogFace.rows<<endl;
//
//
//			//Extract Gabor Features
//
//			GaborFeatureExtractor gFace(gaborCropped,4);
//			Mat gMat= gFace.extractFeaturesImage();
//			imshow("IGabor S",gMat);
//
//			//		-----LBP features------------
//
//			LbpFeatureExtractor dlbpLres(noseEyeRegion,4,4);
//			Mat visLbpLres=dlbpLres.extractELbpFeaturesImage();
//			imshow("visLbpLres S",visLbpLres);
//			waitKey(1);
//			//Extract fss features----
//
//			
//			Point point(20,20);
//			cv::circle( noseEyeRegion, point,  3, cv::Scalar(255,255,0) );
//			imshow("im",noseEyeRegion);
//			namedWindow( "im", CV_WINDOW_KEEPRATIO );
//			waitKey(1);
//			LSS lss(noseEyeRegion);
//			Mat descriptor = lss();
//			cout<<descriptor<<endl;
//			 //lss.visualize();
//			//imshow("r1",r1);
//			
//	//Mat tan2 = imread("Aaron_Peirsol_0002.jpg",1);
//	//img=resizeImage(img,72,80);
//	//Mat tan=tan_triggs_preprocessing(img,0.1, 10.0,0.2,1,2);
////	Point point(tan.rows/2,tan.cols/2);
////	cv::circle( tan, point,  3, cv::Scalar(0,255,0) );
////	imshow("Tan",tan);
////		waitKey(1);
////	
////LSS lss(point,tan);
////Mat descriptor = lss();
////cout<<descriptor<<endl;
////lss.visualize();
//
//		}
//	}
//
//	return 0; 
//}
//
//
//
