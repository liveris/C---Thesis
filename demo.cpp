
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

#include <list>

#if MSVC
#include <windows.h>
#endif

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "crowdsight.h"
#include "imageUtils.h"
#include "lbpUniform.h"
#include "facePreprocessor.h"
#include "facialRegionExtractor.h"
#include "hogFeatureExtractor.h"
#include "lbpFeatureExtractor.h"
#include "GaborFeatureExtractor.h"
#include <string>
#include "ltpFeatureExtractor.h"
#include "fssFeatureExtractor.h"
#if defined( _MSC_VER )
#define snprintf _snprintf
#endif

#ifndef HUMAN_NAME
// Extract date and time and append to window name
std::time_t result = std::time( NULL );
char buf[50];
int a = snprintf( buf,
	sizeof buf,
	"%s%s",
	"CrowdSight Demo\t",
	std::asctime( std::localtime(&result) ) );
#define HUMAN_NAME buf
#endif

// Turn on/off recording of output video and csv
#define RECORDING 0
//#define DESIRED_RIGHT_EYE_X 0.25


using namespace cv;
using namespace std;

Mat imHist(Mat hist, float scaleX=1, float scaleY=1){
	double maxVal=0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);
	int rows = 64; //default height size
	int cols = hist.rows; //get the width size from the histogram
	Mat histImg = Mat::zeros(rows*scaleX, cols*scaleY, CV_8UC3);
	//for each bin
	for(int i=0;i<cols-1;i++) {
		float histValue = hist.at<float>(i,0);
		float nextValue = hist.at<float>(i+1,0);
		Point pt1 = Point(i*scaleX, rows*scaleY);
		Point pt2 = Point(i*scaleX+scaleX, rows*scaleY);
		Point pt3 = Point(i*scaleX+scaleX, (rows-nextValue*rows/maxVal)*scaleY);
		Point pt4 = Point(i*scaleX, (rows-nextValue*rows/maxVal)*scaleY);

		int numPts = 5;
		Point pts[] = {pt1, pt2, pt3, pt4, pt1};

		fillConvexPoly(histImg, pts, numPts, Scalar(255,255,255));
	}
	return histImg;
}


int main ( int argc, char *argv[] )
{
	cv::VideoCapture cap;
	std::string auth_key;
	std::string file_name;
	std::string input;
	std::string data_dir = "../data/";
	std::string resources_dir = "../resources/";
	bool is_webcam_input = false;
	

	vector<Mat> normLeftEyeRegion,normRightEyeRegion,normNoseRegion,normLowResFaceRegion;
	vector<Mat> vHogLres,vHogReye,vHogLeye,vHogNose,vLbpLres,vLbpReye,vLbpLeye,vLbpNose,vHogFace,vEqualizedNorm,vGabor,vFss;
	vector<float> headPose,headYaw;
	vector <int> labels;
	// Create crowdsight instance
	CrowdSight crowdsight( data_dir );

	// Authenticate crowdsight instance
	if ( !crowdsight.authenticate( auth_key ) )
	{
		std::cerr << crowdsight.getErrorDescription() << std::endl;
		std::cout << "hello" << std::endl;
		return -1;
	}

	static CvScalar colors[] =
	{
		{{0,   0,   255}},
		{{0,   128, 255}},
		{{0,   255, 255}},
		{{0,   255, 0  }},
		{{255, 128, 0  }},
		{{255, 255, 0  }},
		{{255, 0,   0  }},
		{{255, 0,   255}}
	};

	// Get the path to your CSV.
	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/orl_faces/data_faces.csv";
	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/colorferet/dvd1/data/images/faces_colorferet_train_all_withoutprofile_withouthalf_withoutrerd.txt";
	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/faces/faces_0_30.txt";
	string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/pubfig83lfw_raw_in_dirs/pubfig83lfw_raw_in_dirs/train/faces_publig83lfw_distract.txt";
	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/faces_yaleB.txt";
	//string fn_csv = "C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/lfw/faces_lfw_5.txt";
	// These vectors hold the images and corresponding labels.

	// Read in the data. This can fail if no valid
	// input filename is given.
	std::ifstream file(fn_csv.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	int counter=0,icounter=0;
	char separator = ';';
	string line, path, classlabel;std::string filename;
	int i=1;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		//path="C:/Users/Liveris/Documents/MasterThesis/FaceDatasets/pubfig83lfw_raw_in_dirs/pubfig83lfw_raw_in_dirs/test/Harrison Ford/test__000034-000011.jpg";
		//iterate through all images
		//for ( unsigned int i = 0; i < images.size();++i )
		//{
		cv::Mat frame;
		int label;
		i++ ;
		// Grab one frame
		if(!path.empty() && !classlabel.empty()) {
			frame = ((imread(path)));
			label=(atoi(classlabel.c_str()));
			filename= path;
		}

		//frame =images[i]; //imread("C:/Users/Liveris/Downloads/lena.jpg");//images[i]; // //Give the full path to the image file#	namedWindow( "Display window 1", CV_WINDOW_KEEPRATIO );// Create a window for display. 
		// Define region of interest here if required
		int marginTop     = 0;
		int marginBottom  = 0;
		int marginLeft    = 0;
		int marginRight   = 0;
		cv::Rect roi;

		int  maxNumPeople = 10;

		//Start main processing loop

		crowdsight.setMaxNumPeople( maxNumPeople );


		// If frame is empty break
		if ( frame.empty() )
		{
			break;
		}

		//cv::pyrDown(frame,frame);

		// Specify the roi that will be extracted from the frame
		roi.x      = marginLeft;                    // pixels to skip from the left
		roi.y      = marginTop;                     // pixels to skip from the top
		roi.width  = frame.cols-roi.x-marginRight;  // width of roi
		roi.height = frame.rows-roi.y-marginBottom; // height of roi

		// Change settings if required

		// Proces current image.
		// Process function evaluates the frames contents and
		// must be called before getCurrentPeople();

		//show the current face image
		//cv::imshow("frame",frame);
		//namedWindow( "frame", CV_WINDOW_KEEPRATIO );


		if ( !crowdsight.process(frame,roi) )
		{
			std::cout << crowdsight.getErrorDescription() << std::endl;

		}

		// Get the list of people in the last processed frame
		std::vector<Person> people;
		if ( !crowdsight.getCurrentPeople(people) )
		{
			std::cerr << crowdsight.getErrorDescription() << std::endl;
		}

		cout<<"outer counter "<<counter++<<endl;
		// For the one person in the frame, do:
		if (people.size()>0 )
		{
			cout<<"inner counter "<<icounter++<<endl;// Get person
			Person person = people.at(0);

			/*********************************** RETRIEVE PERSN INFO ***********************************/

			cv::Rect    face              = person.getFaceRect();      // Retrieve the person's face
			cv::Point   right_eye         = person.getRightEye();      // Retrieve left and right eye locations of the person. Eye location is relative to the face rectangle.
			cv::Point   left_eye          = person.getLeftEye();

			/************************************* DRAW PERSON INFO *************************************/

			// Offset eye position with face position, to get frame coordinates.
			right_eye.x += face.x;
			right_eye.y += face.y;
			left_eye.x  += face.x;
			left_eye.y  += face.y;

			// Draw circles in the center of left and right eyes
			//cv::circle( frame, right_eye, 3, cv::Scalar(0,255,0) );
			//cv::circle( frame, left_eye,  3, cv::Scalar(0,255,0) );
			Mat faceRect=frame(face);
			// Draw a rectangle around person's face on the current frame
			//cv::rectangle(frame, face, cv::Scalar(255,255,255), 1);

			//Show image
			imshow("frame",frame);
			namedWindow( "frame", CV_WINDOW_KEEPRATIO );
			//Framo preprocessor
			Mat greyFrame=convert2gray(frame);
			//initialize constructor with original eye points
			FacePreprocessor fprocessor(right_eye,left_eye,72,80);
			//register image
			Mat warped=fprocessor.registerImage(greyFrame);
			//imshow("warped",warped);
			Mat tanTriggs=	fprocessor.tan_triggs_preprocessing(warped);
			imshow("Tan Triggs",(tanTriggs));
			//mean normalize image
			//Mat norm=fprocessor.meanNormalizeImage(warped);
			//Mat equalizedNorm;
			//equalizeHist( norm, equalizedNorm );//Equalize the face image
			//imshow("norm",norm);
			//cout<<getImgType(normEllipse.type())<<endl;
			//imshow("Equalized norm",equalizedNorm);
			//namedWindow( "Equalized norm", CV_WINDOW_KEEPRATIO );
			//waitKey(1);
			//Extract facial regions from face
			bool viz=false;
			FacialRegionExtractor fextractor(fprocessor.getRegisteredRightEye(fprocessor.getTransformMatrix()),fprocessor.getRegisteredLeftEye(fprocessor.getTransformMatrix()),tanTriggs,viz);

			Mat faceImage=fextractor.getFace();
			Mat gaborCropped= resizeImage(faceImage,30,30);
			imshow("gabor cropped",gaborCropped);
			namedWindow( "gabor cropped", CV_WINDOW_KEEPRATIO );
			waitKey(1);

			// add the labels
			labels.push_back(label);

			//puth them to vectors for opencv lbp uniform
			vEqualizedNorm.push_back(faceImage);

			//Extract hog features from resized facial regions
			HogFeatureExtractor descFace(faceImage,16,16,8,8,8,8);
			vector<float> HogFace=descFace.extractFeatures();
			Mat visHogFace=descFace.convertDescriptor2Mat(HogFace);
			vHogFace.push_back(visHogFace);
			imshow("visHog",descFace.getHogVis(visHogFace));
			waitKey(3);
			cout<<visHogFace.rows<<endl;

			//Extract Gabor Features
			GaborFeatureExtractor gFace(gaborCropped,4);
			Mat gMat= gFace.extractFeatures();
			vGabor.push_back(gMat);

			//Extract fss features
			LSS lss(faceImage);
			Mat descriptor = lss();
			//lss.visualize();
			vFss.push_back(descriptor);
	
		}
	}

	//Create lbp uniform decriptor
	LBPH lbph(2,8,9,10,DBL_MAX,true);
	lbph.train(vEqualizedNorm, labels);
	//FileStorage fs1("lbpUniformFaceTanTrain.yml", FileStorage::WRITE);
	lbph.save("lbpUniformFaceTanDistract.yml");
	//fs1.release();

	//Save the hog face
	FileStorage fs2("hogFaceTanDistract.yml", FileStorage::WRITE);
	fs2 << "TestImages" << vHogFace;
	fs2 << "LabelImages"<< labels;
	fs2.release();

	//Save the gabor face
	FileStorage fs3("gaborFaceTanDistract.yml", FileStorage::WRITE);
	fs3 << "TestImages" << vGabor;
	fs3 << "LabelImages"<< labels;
	fs3.release();

	//Save the fss face
	FileStorage fs4("fssFaceTanDistract.yml", FileStorage::WRITE);
	fs4 << "TestImages" << vFss;
	fs4 << "LabelImages"<< labels;
	fs4.release();

	return 0; 
}



