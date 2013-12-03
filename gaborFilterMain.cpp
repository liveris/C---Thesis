//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "imageUtils.h"
//#include <math.h>
//using namespace cv;
//cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps)
//{
//    int hks = (ks-1)/2;
//    double theta = th*CV_PI/180;
//    double psi = ps*CV_PI/180;
//    double del = 2.0/(ks-1);
//    double lmbd = lm;
//    double sigma = sig/ks;
//    double x_theta;
//    double y_theta;
//    cv::Mat kernel(ks,ks, CV_32F);
//    for (int y=-hks; y<=hks; y++)
//    {
//        for (int x=-hks; x<=hks; x++)
//        {
//            x_theta = x*del*cos(theta)+y*del*sin(theta);
//            y_theta = -x*del*sin(theta)+y*del*cos(theta);
//            kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
//        }
//    }
//    return kernel;
//}
//
//int kernel_size=21;
//int pos_sigma= 5;
//int pos_lm = 50;
//int pos_th = 0;
//int pos_psi = 90;
//cv::Mat src_f;
//cv::Mat dest;
//
//void Process(int , void *)
//{
//    double sig = pos_sigma;
//    double lm = 0.5+pos_lm/100.0;
//    double th = pos_th;
//    double ps = pos_psi;
//    cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
//    cv::filter2D(src_f, dest, CV_32F, kernel);
//    cv::imshow("Process window", dest);
//    cv::Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
//    cv::resize(kernel, Lkernel, Lkernel.size());
//    Lkernel /= 2.;
//    Lkernel += 0.5;
//    cv::imshow("Kernel", Lkernel);
//    cv::Mat mag;
//    cv::pow(dest, 2.0, mag);
//    cv::imshow("Mag", mag);
//}
//
//cv::Mat tan_triggs_preprocessing(cv::Mat src,
//	float alpha, float tau , float gamma, int sigma0,
//	int sigma1) {
//
//		// Convert to floating point:
//		cv::Mat X =src;
//		X.convertTo(X, CV_32FC1);
//		// Start preprocessing:
//		cv::Mat I;
//		pow(X, gamma, I);
//		// Calculate the DOG Image:
//		{
//			cv::Mat gaussian0, gaussian1;
//			// Kernel Size:
//			int kernel_sz0 = (3*sigma0);
//			int kernel_sz1 = (3*sigma1);
//			// Make them odd for OpenCV:
//			kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
//			kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
//			GaussianBlur(I, gaussian0, cv::Size(kernel_sz0,kernel_sz0), sigma0, sigma0);
//			GaussianBlur(I, gaussian1, cv::Size(kernel_sz1,kernel_sz1), sigma1, sigma1);
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
//cv::Mat meanNormalizeImage(cv::Mat face){
//	cv::Mat image32f;
//	face.convertTo(image32f,CV_32F);
//
//	cv::Mat mu;
//	blur(image32f, mu,cv::Size(face.cols,face.rows));
//
//	cv::Mat out;
//	cv::subtract(image32f,mu,out);
//
//	return mat2gray(out);
//
//}
//
//int main(int argc, char** argv)
//{
//    cv::Mat image = cv::imread("original.jpg",1);
//	Mat tan=tan_triggs_preprocessing(image, 0.1, 10.0, 0.2, 1, 2);
//	cv::resize( image, image,cv::Size(250,300) );
//	image=meanNormalizeImage(image);
//    cv::imshow("Src", image);
//    cv::Mat src;
//    cv::cvtColor(image, src, CV_BGR2GRAY);
//    src.convertTo(src_f, CV_32F, 1.0/255, 0);
//    if (!kernel_size%2)
//    {
//        kernel_size+=1;
//    }
//    cv::namedWindow("Process window", 1);
//    cv::createTrackbar("Sigma", "Process window", &pos_sigma, kernel_size, Process);
//    cv::createTrackbar("Lambda", "Process window", &pos_lm, 100, Process);
//    cv::createTrackbar("Theta", "Process window", &pos_th, 180, Process);
//    cv::createTrackbar("Psi", "Process window", &pos_psi, 360, Process);
//    Process(0,0);
//    cv::waitKey(0);
//    return 0;
//}