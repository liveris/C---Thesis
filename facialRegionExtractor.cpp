#include "imageUtils.h"
#include "facialRegionExtractor.h"

cv::Mat FacialRegionExtractor::getFace(){
return face;
}

cv::Mat FacialRegionExtractor::getLeftEyeRegion(){
	//Get the center between the 2 eyes.
	Point2f eyesCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;

	// Get the D1 distance between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double D1 = sqrt(dx*dx + dy*dy);
	// Get the D2 distance proportion of istance between eye center to eye brow center to distance between eye centers
	double D2=0.33*D1; //based on anthropometric measurement 0.33 detection of facial features paper
	//left region as we look the picture, right og the sightcorp faca that return the real position of the right eye
	double eyePosx=leftEye.x-D2;
	//std::cout<<"Left dist: "<<eyeWidth<<endl;
	if (eyePosx>face.cols){
		eyePosx=face.cols;
	}
	cv::Rect leftEyeRegion = cv::Rect( eyePosx, leftEye.y-1.2*D2, 2.1*D2, 2*D2 );
	if (viz){
	cv::rectangle(face, leftEyeRegion, cv::Scalar(255,255,255), 1);
	cv::circle( face, leftEye,  3, cv::Scalar(255,255,255) );
	}
	Mat leftEyeRoi=face(leftEyeRegion);
	return leftEyeRoi;
}

cv::Mat FacialRegionExtractor::getRightEyeRegion(){
	//Get the center between the 2 eyes.
	Point2f eyesCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;
	// Get the D1 distance between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double D1 = sqrt(dx*dx + dy*dy);
	// Get the D2 distance
	double D2=0.33*D1; //based on anthropometric measurement 0.33 detection of facial features paper
	double	eyePos=rightEye.x-D2;
	//cout<<"Right dist: "<<eyePos<<endl;
	if (eyePos<0)
	{eyePos=0;
	}
	cv::Rect rightEyeRegion = cv::Rect( eyePos, rightEye.y-1.2*D2, 2.1*D2, 2*D2 );
	if(viz){
	cv::rectangle(face, rightEyeRegion, cv::Scalar(255,255,255), 1);
	cv::circle( face, rightEye,  3, cv::Scalar(255,255,255) );
	}
	Mat rightEyeRoi=face(rightEyeRegion);
	return rightEyeRoi;
}

cv::Mat FacialRegionExtractor::getNoseEyeRegion(){
		//Get the center between the 2 eyes.
	Point2f eyesCenter,mouthCenter,noseCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;
	// Get the D1 distance between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double D1 = sqrt(dx*dx + dy*dy);
	// Get the D2 distance
	double D2=0.33*D1; //based on anthropometric measurement 0.33 detection of facial features paper
		double D3=0.33*D1;
	double D4=0.60*D1;
	double D5b=1.1*D1;
	double mouthWidth=D2/0.35;//ICIAR2007
	double Dmnb=abs(D5b-D4);
	double Dmna=abs(mouthWidth*0.65);
	double D5a=1.4*mouthWidth;
	//get the average between the two models
	double D5=(D5a+D5b)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models
	double Dmn=abs(Dmna+Dmnb)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models

	mouthCenter.x=eyesCenter.x;
	mouthCenter.y=eyesCenter.y + D5;
	//cv::circle( face, mouthCenter,  3, cv::Scalar(0,255,0) );

	noseCenter.x=mouthCenter.x;
	noseCenter.y=mouthCenter.y - Dmn;
	double noseWidth=mouthWidth/1.618;/*v63-87 paper,0.3*face.cols;*/double noseHeight=D4;//25;
	double	eyePosxR=rightEye.x-D2;
	double eyePosxL=leftEye.x+D2;
	//std::cout<<"Left dist: "<<eyeWidth<<endl;
	//double eyePosxL=leftEye.x-D2;
	if (eyePosxL>face.cols){
		eyePosxL=face.cols;
	}

	if (eyePosxR>face.cols){
		eyePosxR=face.cols;
	}
	//cout<<"Right dist: "<<eyePos<<endl;
	if (eyePosxR<0)
	{eyePosxR=0;
	}
	Point2f eyeCornerR,eyeCornerL,noseCorner,noseEyeRegion;
	eyeCornerR.x=eyePosxR;
	eyeCornerR.y=rightEye.y-1.2*D2;
	eyeCornerL.x=eyePosxL;
	eyeCornerL.y=leftEye.y+1.2*D2;
	noseCorner.x=noseCenter.x+noseWidth/2;
	noseCorner.y=noseCenter.y+noseHeight/2;
	
	cv::Rect noseEyeRegion2 = cv::Rect( eyeCornerR.x,eyeCornerR.y ,eyeCornerL.x-eyeCornerR.x, noseCorner.y-eyeCornerR.y );
	if(false){
	cv::rectangle(face, noseEyeRegion2, cv::Scalar(255,255,255), 1);
	cv::circle( face, eyeCornerR,  3, cv::Scalar(255,255,255) );
	cv::circle( face, eyeCornerL,  3, cv::Scalar(255,255,255) );
	cv::circle( face, noseCorner,  3, cv::Scalar(255,255,255) );
	}
	Mat rightEyeRoi=face(noseEyeRegion2);
	Mat faceLowRes;
	cv::resize(rightEyeRoi, faceLowRes,cv::Size(72,80));
	//cv::Mat faceLowRes= resizeImage( rightEyeRoi, 72,80);
	return faceLowRes;
}

cv::Mat FacialRegionExtractor::getMouthRegion(){
	//Get the center between the 2 eyes.
	Point2f eyesCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;
	// Get the D1 distance between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double D1 = sqrt(dx*dx + dy*dy);
	// Get the D2 distance
	double D2=0.33*D1; //based on anthropometric measurement 0.33 detection of facial features paper
	double D3=0.33*D1;
	double D4=0.60*D1;
	double D5b=1.1*D1;
	double mouthWidth=D2/0.35;//ICIAR2007
	double Dmnb=abs(D5b-D4);
	double Dmna=abs(mouthWidth*0.65);
	double D5a=1.4*mouthWidth;
	//get the average between the two models
	double D5=(D5a+D5b)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models
	double Dmn=abs(Dmna+Dmnb)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models
	cv::Point2f mouthCenter;
	mouthCenter.x=eyesCenter.x;
	mouthCenter.y=eyesCenter.y + D5;

	double mouthHeight=mouthWidth/1.5;
	double mouthHeight2=mouthHeight;
	double mouthPosy=mouthCenter.y+mouthHeight/2;
	//cout<<"Mouth Center: "<<mouthCenter<<endl;
	if (mouthPosy>face.rows)
	{//mouthDim=frame.rows;
		//mouthHeight2=2*(mouthDim-mouthCenter.y)
		mouthHeight2=face.rows-abs(mouthCenter.y-mouthHeight/2);
	}
	cv::Rect mouth = cv::Rect( mouthCenter.x -mouthWidth/2,mouthCenter.y-mouthHeight/2, mouthWidth, mouthHeight2 );
	if(viz){
	cv::rectangle(face, mouth, cv::Scalar(255,255,255), 1);
	}
	Mat mouthRoi=face(mouth);
	return mouthRoi;
}

cv::Mat FacialRegionExtractor::getNoseRegion(){
	//Get the center between the 2 eyes.
	Point2f eyesCenter,noseCenter,mouthCenter;
	eyesCenter.x = (rightEye.x + leftEye.x) * 0.5f;
	eyesCenter.y = (rightEye.y + leftEye.y) * 0.5f;
	// Get the D1 distance between the 2 eyes.
	double dy = (leftEye.y - rightEye.y);
	double dx = (leftEye.x - rightEye.x);
	double D1 = sqrt(dx*dx + dy*dy);
	// Get the D2 distance
	double D2=0.33*D1; //based on anthropometric measurement 0.33 detection of facial features paper
	double D3=0.33*D1;
	double D4=0.60*D1;
	double D5b=1.1*D1;
	double mouthWidth=D2/0.35;//ICIAR2007
	double Dmnb=abs(D5b-D4);
	double Dmna=abs(mouthWidth*0.65);
	double D5a=1.4*mouthWidth;
	//get the average between the two models
	double D5=(D5a+D5b)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models
	double Dmn=abs(Dmna+Dmnb)/2;//average distance between midpoint eye center and mouth center of two difrent anthropometric models

	mouthCenter.x=eyesCenter.x;
	mouthCenter.y=eyesCenter.y + D5;
	//cv::circle( face, mouthCenter,  3, cv::Scalar(0,255,0) );

	noseCenter.x=mouthCenter.x;
	noseCenter.y=mouthCenter.y - Dmn;
	double noseWidth=mouthWidth/1.618;/*v63-87 paper,0.3*face.cols;*/double noseHeight=D4;//25;
	//1/4 face width
	//cout<<"Nose Center: "<<noseCenter<<endl;
	cv::Rect nose = cv::Rect( noseCenter.x-noseWidth/2, noseCenter.y-noseHeight/2, 1.2*noseWidth, noseHeight );
	if(viz){
	cv::rectangle(face, nose, cv::Scalar(255,255,255), 1);
	}
	Mat noseRoi=face(nose);
	return noseRoi;
}

cv::Mat FacialRegionExtractor::getLowResFace( int width,int height){
cv::Mat faceLowRes= resizeImage( face, width,height);
return faceLowRes;

}