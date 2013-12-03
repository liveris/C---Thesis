#include "gaborFeatureExtractor.h"

cv::Mat mkKernelg(int ks, double sig, double th, double lm, double ps)
{
	int hks = (ks-1)/2;
	double theta = th*CV_PI/180;
	double psi = ps*CV_PI/180;
	double del = 2.0/(ks-1);
	double lmbd = lm;
	double sigma = sig/ks;
	double x_theta;
	double y_theta;
	cv::Mat kernel(ks,ks, CV_32F);
	for (int y=-hks; y<=hks; y++)
	{
		for (int x=-hks; x<=hks; x++)
		{
			x_theta = x*del*cos(theta)+y*del*sin(theta);
			y_theta = -x*del*sin(theta)+y*del*cos(theta);
			kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
		}
	}
	return kernel;
}

Mat GaborFeatureExtractor::extractFeatures(){
	cv::Mat image32f;
	imageRegion.convertTo(image32f,CV_32F);
	Mat result= Mat::zeros( 5,imageRegion.cols * imageRegion.rows, CV_32FC1);
	// return matrix with zeros if no data was given
	if(imageRegion.empty())
		return result.reshape(1,1);

	int kernel_size=(image32f.rows+image32f.cols)/2;
	if (!kernel_size%2)
	{
		kernel_size+=1;
	}
	// initial result_row
	int resultRowIdx = 0;
	// iterate through orientations
	for(int j = 0; j <= 180; j=j+45) {
		Mat cell_hist ;
		if(j<180){
			Mat kernel=mkKernelg( kernel_size, 6.28,  j, lambda, 90);
			cv::filter2D(image32f, cell_hist, CV_32F, kernel);
		}
		else{
			cell_hist=image32f;
		}
		//imshow("image",mat2gray(cell_hist));
		// copy to the result matrix 
		Mat result_row = result.row(resultRowIdx);
		cell_hist.reshape(1,1).convertTo(result_row, CV_32FC1);
		/*		imshow("image reshape",mat2gray(result));
		waitKey(1);*/
		// increase row count in result matrix
		resultRowIdx++;

	}
	// return result as reshaped feature vector
	//std::cout<<result.reshape(1,1).cols<<std::endl;
	//std::cout<<result.reshape(1,1).rows<<std::endl;
	/*	imshow("image reshape f",mat2gray(result.reshape(1,1)));
	waitKey(1);*/
	return result.reshape(1,1);

}

Mat GaborFeatureExtractor::extractFeaturesImage(){
	cv::Mat image32f;
	imageRegion.convertTo(image32f,CV_32F);
	Mat result = Mat::zeros(0, 0, CV_32F);
	// return matrix with zeros if no data was given
	if(imageRegion.empty())
		return result.reshape(1,1);
	int kernel_size=image32f.rows;
	if (!kernel_size%2)
	{
		kernel_size+=1;
	}
	// initial result_row
	int resultRowIdx = 0;
	// iterate through orientations
	for(int j = 0; j <= 180; j=j+45) {
		Mat cell_hist ;
		if(j<180){
			Mat kernel=mkKernelg( kernel_size, 6.28,  j, lambda, 90);
			cv::filter2D(image32f, cell_hist, CV_32F, kernel);
		}
		else{
			cell_hist=image32f;
		}

		result.push_back(mat2gray(cell_hist));
	}
	return mat2gray(result);
}
static Mat spatial_histogram(InputArray _src, int numPatterns,
	int grid_x, int grid_y, bool /*normed*/)
{

}