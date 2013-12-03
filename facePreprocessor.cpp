#include "facePreprocessor.h"
#include <iostream>
cv::Mat FacePreprocessor::registerImage (cv::Mat face)
{
	int DESIRED_FACE_WIDTH=registeredFaceWidth;
	int DESIRED_FACE_HEIGHT=registeredFaceHeight;
	// Get the center between the 2 eyes.
	cv::Point2f eyesCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;

	// Get the angle between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double len = sqrt(dx*dx + dy*dy);
	// Convert Radians to Degrees.
	double angle = atan2(dy, dx) * 180.0/CV_PI;

	// Hand measurements shown that the left eye center should
	// ideally be roughly at (0.16, 0.14) of a scaled face image.
	//const double DESIRED_LEFT_EYE_X = 0.16;
	const double DESIRED_RIGHT_EYE_X = 0.25;     // Controls how much of the face is visible after preprocessing
	const double DESIRED_LEFT_EYE_X = (1.0f -DESIRED_RIGHT_EYE_X);
	const double DESIRED_RIGHT_EYE_Y = 0.3;
	// Get the amount we need to scale the image to be the desired
	// fixed size we want.
	double desiredLen = (DESIRED_LEFT_EYE_X- DESIRED_RIGHT_EYE_X);
	double scale = desiredLen * DESIRED_FACE_WIDTH / len;

	// Get the transformation matrix for the desired angle & size.
	cv::Mat rot_mat =cv::getRotationMatrix2D(eyesCenter, angle, scale);
	// Shift the center of the eyes to be the desired center.
	double ex = DESIRED_FACE_WIDTH * 0.5f - eyesCenter.x;
	double ey = DESIRED_FACE_HEIGHT * DESIRED_RIGHT_EYE_Y-eyesCenter.y;
	rot_mat.at<double>(0, 2) += ex;
	rot_mat.at<double>(1, 2) += ey;
	cv::Point newEyeCenter;
	newEyeCenter.x=ex; newEyeCenter.y=ey;

	// Transform the face image to the desired angle & size &
	// position! Also clear the transformed image background to a
	// default grey.
	cv::Mat warped = cv::Mat(DESIRED_FACE_HEIGHT, DESIRED_FACE_WIDTH,CV_8U, cv::Scalar(128));
	cv::Mat gray;
	//cv::circle( face, rightEye, 3, cv::Scalar(0,255,0) );
	//if (viz)	{
	//	Draw circles in the center of left and right eyes
	//	cv::circle( face, rightEye, 3, cv::Scalar(0,255,0) );
	//	cv::circle( face, leftEye,  3, cv::Scalar(0,255,0) );
	//	cv::circle( warped, result,  3, cv::Scalar(126,255,0) );
	//}

	warpAffine(face, warped, rot_mat, warped.size());
	//cv::Point result;
	//result.x = rot_mat.at<double>(0,0)*eyesCenter.x + rot_mat.at<double>(0,1)*eyesCenter.y + rot_mat.at<double>(0,2);
	//result.y = rot_mat.at<double>(1,0)*eyesCenter.x + rot_mat.at<double>(1,1)*eyesCenter.y + rot_mat.at<double>(1,2);

	return warped;
}


cv::Mat FacePreprocessor::getTransformMatrix ()
{
	int DESIRED_FACE_WIDTH=registeredFaceWidth;
	int DESIRED_FACE_HEIGHT=registeredFaceHeight;
	// Get the center between the 2 eyes.
	cv::Point2f eyesCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;

	// Get the angle between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double len = sqrt(dx*dx + dy*dy);
	// Convert Radians to Degrees.
	double angle = atan2(dy, dx) * 180.0/CV_PI;

	// Hand measurements shown that the left eye center should
	// ideally be roughly at (0.16, 0.14) of a scaled face image.
	//const double DESIRED_LEFT_EYE_X = 0.16;
	const double DESIRED_RIGHT_EYE_X = 0.25;     // Controls how much of the face is visible after preprocessing
	const double DESIRED_LEFT_EYE_X = (1.0f -DESIRED_RIGHT_EYE_X);
	const double DESIRED_RIGHT_EYE_Y = 0.3;
	// Get the amount we need to scale the image to be the desired
	// fixed size we want.
	double desiredLen = (DESIRED_LEFT_EYE_X- DESIRED_RIGHT_EYE_X);
	double scale = desiredLen * DESIRED_FACE_WIDTH / len;

	// Get the transformation matrix for the desired angle & size.
	cv::Mat rot_mat =cv::getRotationMatrix2D(eyesCenter, angle, scale);
	// Shift the center of the eyes to be the desired center.
	double ex = DESIRED_FACE_WIDTH * 0.5f - eyesCenter.x;
	double ey = DESIRED_FACE_HEIGHT * DESIRED_RIGHT_EYE_Y-eyesCenter.y;
	rot_mat.at<double>(0, 2) += ex;
	rot_mat.at<double>(1, 2) += ey;
	return rot_mat;
}

cv::Mat FacePreprocessor::normalizeImage(cv::Mat face){
	cv::Mat image32f;
	face.convertTo(image32f,CV_32F);

	cv::Mat mu;
	blur(image32f, mu,cv::Size(registeredFaceWidth,registeredFaceWidth));

	cv::Mat mu2;
	blur(image32f.mul(image32f), mu2,cv::Size(registeredFaceWidth,registeredFaceWidth));

	cv::Mat sigma;
	cv::sqrt(mu2 - mu.mul(mu), sigma);
	cv::Mat out;
	cv::Mat filtered;
	cv::subtract(image32f,mu,out);
	//cv::imshow("Subtract mean", mat2gray(out));
	cv::Mat out2;
	cv::divide(out,sigma,out2);
	//imshow("divide", mat2gray(out2));
	//cout<<out2<<endl;
	//cv::imshow("initial image", mat2gray(image32f));
	//cv::imshow("mu", mat2gray(mu));
	//cv::imshow("sigma",mat2gray(sigma));
	//waitKey();
	//cout<<mat2gray(out2)<<endl;
	return mat2gray(out2);

}

cv::Mat FacePreprocessor::meanNormalizeImage(cv::Mat face){
	cv::Mat image32f;
	face.convertTo(image32f,CV_32F);

	cv::Mat mu;
	blur(image32f, mu,cv::Size(registeredFaceWidth,registeredFaceWidth));

	cv::Mat out;
	cv::subtract(image32f,mu,out);

	return mat2gray(out);

}

cv::Mat FacePreprocessor::ellipticalMask (cv::Mat face){
	// Filter out the corners of the face, since we mainly just care about the middle parts.
	// Draw a filled ellipse in the middle of the face-sized image.
	Mat mask = Mat(face.size(), CV_8U, Scalar(0)); // Start with an empty mask.
	double FACE_ELLIPSE_CY = 0.40;
	double FACE_ELLIPSE_W = 0.50;         // Should be atleast 0.5
	double FACE_ELLIPSE_H = 0.70;
	//double desired_face_width=60;
	//double desired_face_height=60;
	Point faceCenter = Point(  registeredFaceWidth/2, cvRound(registeredFaceHeight * FACE_ELLIPSE_CY) );
	Size size = Size( cvRound(registeredFaceWidth * FACE_ELLIPSE_W), cvRound(registeredFaceHeight * FACE_ELLIPSE_H) );
	ellipse(mask, faceCenter, size, 0, 0, 360, Scalar(255), CV_FILLED);
	//imshow("mask", mask);

	// Use the mask, to remove outside pixels.
	Mat dstImg = Mat(face.size(), CV_8U, Scalar(128)); // Clear the output image to a default gray.
	/*
	namedWindow("filtered");
	imshow("filtered", filtered);
	namedWindow("dstImg");
	imshow("dstImg", dstImg);
	namedWindow("mask");
	imshow("mask", mask);
	*/
	// Apply the elliptical mask on the face.
	face.copyTo(dstImg, mask);  // Copies non-masked pixels from filtered to dstImg.
	//imshow("dstImg", dstImg);

	return dstImg;
}

cv::Point2f FacePreprocessor::getRegisteredRightEye(cv::Mat rot_mat){
	cv::Point2f transf_right_eye;
	transf_right_eye.x = rot_mat.at<double>(0,0)*rightEye.x + rot_mat.at<double>(0,1)*rightEye.y + rot_mat.at<double>(0,2);
	transf_right_eye.y = rot_mat.at<double>(1,0)*rightEye.x + rot_mat.at<double>(1,1)*rightEye.y + rot_mat.at<double>(1,2);
	return transf_right_eye;
}


cv::Point2f FacePreprocessor::getRegisteredLeftEye(cv::Mat rot_mat){
	Point2f transf_left_eye;
	transf_left_eye.x = rot_mat.at<double>(0,0)*leftEye.x + rot_mat.at<double>(0,1)*leftEye.y + rot_mat.at<double>(0,2);
	transf_left_eye.y = rot_mat.at<double>(1,0)*leftEye.x + rot_mat.at<double>(1,1)*leftEye.y + rot_mat.at<double>(1,2);
	return transf_left_eye;
}

//
// Calculates the TanTriggs Preprocessing as described in:
//
// Tan, X., and Triggs, B. "Enhanced local texture feature sets for face
// recognition under difficult lighting conditions.". IEEE Transactions
// on Image Processing 19 (2010), 1635–650.
//
// Default parameters are taken from the paper.
//
cv::Mat FacePreprocessor::tan_triggs_preprocessing(InputArray src,
	float alpha, float tau , float gamma, int sigma0,
	int sigma1) {

		// Convert to floating point:
		Mat X = src.getMat();
		X.convertTo(X, CV_32FC1);
		// Start preprocessing:
		Mat I;
		pow(X, gamma, I);
		// Calculate the DOG Image:
		{
			Mat gaussian0, gaussian1;
			// Kernel Size:
			int kernel_sz0 = (3*sigma0);
			int kernel_sz1 = (3*sigma1);
			// Make them odd for OpenCV:
			kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
			kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
			GaussianBlur(I, gaussian0, Size(kernel_sz0,kernel_sz0), sigma0, sigma0);
			GaussianBlur(I, gaussian1, Size(kernel_sz1,kernel_sz1), sigma1, sigma1);
			subtract(gaussian0, gaussian1, I);
		}

		{
			double meanI = 0.0;
			{
				Mat tmp;
				pow(abs(I), alpha, tmp);
				meanI = mean(tmp).val[0];

			}
			I = I / pow(meanI, 1.0/alpha);
		}

		{
			double meanI = 0.0;
			{
				Mat tmp;
				pow(min(abs(I), tau), alpha, tmp);
				meanI = mean(tmp).val[0];
			}
			I = I / pow(meanI, 1.0/alpha);
		}

		// Squash into the tanh:
		{
			for(int r = 0; r < I.rows; r++) {
				for(int c = 0; c < I.cols; c++) {
					I.at<float>(r,c) = tanh(I.at<float>(r,c) / tau);
				}
			}
			I = tau * I;
		}
		return mat2gray(I);
}