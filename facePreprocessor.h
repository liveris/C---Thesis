#if !defined FACEPREPROCESSOR
#define FACEPREPROCESSOR

#include <opencv2/core/core.hpp>
#include "imageUtils.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
class FacePreprocessor {

private:

	int registeredFaceWidth;
	int registeredFaceHeight;
	cv::Point2f rightEye,leftEye;
	cv::Point2f registeredRightEye,registeredLeftEye;

public:
	// constructor
	FacePreprocessor(cv::Point2f right_eye,cv::Point2f left_eye,int width,int height) { 
		registeredFaceWidth=width;//according to paper
		registeredFaceHeight=height;
		 leftEye=left_eye;
		 rightEye=right_eye;
		// default parameter initialization here
	}

	// empty destructor
	~FacePreprocessor() { 

		// default parameter initialization here
	}

	// Getters and setters

	// Sets the dim of face to be registered
	void setFaceDim(int width,int height) {
		registeredFaceWidth=width;
		registeredFaceHeight=height;
	}

	// Gets the height of registered face
	int getFaceHeight() {
		return registeredFaceHeight;
	}

	// Gets the width of registered face
	int getFaceWidth() {
		return registeredFaceWidth;
	}

	// Transform the image according to eye centers.Return the transformed image
	cv::Mat FacePreprocessor::registerImage(cv::Mat face);

	//Get the transformation matrix
	cv::Mat FacePreprocessor::getTransformMatrix();

	//Apply local mean normalization
	cv::Mat FacePreprocessor::normalizeImage(cv::Mat face);

	cv::Mat FacePreprocessor::ellipticalMask (cv::Mat face);

	cv::Point2f FacePreprocessor::getRegisteredLeftEye(cv::Mat rot_mat);

	cv::Point2f FacePreprocessor::getRegisteredRightEye(cv::Mat rot_mat);

	cv::Mat FacePreprocessor::meanNormalizeImage(cv::Mat face);

	cv::Mat FacePreprocessor::tan_triggs_preprocessing(InputArray src,
	float alpha = 0.1, float tau = 10.0, float gamma = 0.2, int sigma0 = 1,
	int sigma1 = 2);
};


#endif