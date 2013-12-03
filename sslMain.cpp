//#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/contrib/contrib.hpp"
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
// #include "imageUtils.h"
// 
//using namespace std;
//using namespace cv;
//
//class LSS
//{
//public:
//    LSS(){}
// 
//    LSS( Point p, Mat im )
//    {
//        this->compute ( p, im );
//    }
// 
//    Mat_<float> operator()(void)
//    {
//        return this->d;
//    }
// 
//    Mat_<float> compute( const Point& p, const Mat& input )
//    {
//         
//        if( p.x-odim/2 < 0 || p.y-odim/2 < 0 || p.x+odim/2 >= input.cols || p.y+odim/2 >= input.rows)
//            throw Exception( 1 , "point too close to border" , "compute_descriptor" , "lss.h" , 22 );
//         
//        Mat_<Vec3b> image(input);
//        Mat_<Vec3b> ipatch = image( Range(p.y-idim/2,p.y+idim/2), Range(p.x-idim/2, p.x+idim/2)  );
//        Mat_<Vec3b> opatch = image( Range(p.y-odim/2,p.y+odim/2), Range(p.x-odim/2, p.x+odim/2)  );
//         
//        Mat_<Vec3b> w;
//        Mat_<float> distance_surface( odim/idim, odim/idim );
//        Mat_<float> polar;
//         
//        float epsilon, eta, corr, current;
//        float* angle = new float[angle_steps];
//        float* radii = new float[radii_steps];
//         
//        for (int i = 0 ; i < angle_steps ; ++i)
//            angle[i] = i * 2. * CV_PI/angle_steps;
//        for(int i  = 0 ; i < radii_steps ; ++i)
//            radii[i] = i*(odim/2)/radii_steps;
// 
//        for(int i = 0, ii=0 ; i < odim ; i += idim, ++ii)
//        {
//            for(int j = 0, jj = 0 ; j < odim ; j += idim, ++jj)
//            {
//                w = opatch(Range(i,i+idim),Range(j,j+idim));
//                distance_surface(ii,jj) = ssd(ipatch,w);
//            }
//        }
//         
//        //cv::normalize
//        normalize( distance_surface, distance_surface, 0., 1., NORM_MINMAX );
//		Mat viz;
//		resize( distance_surface, viz, cvSize( distance_surface.rows*20, distance_surface.cols*20 ));
//        imshow( "Dist Surf" , mat2gray(viz ));
// 
//        waitKey( );
//        //log-polar
//        int n, m;
//        polar = Mat_<float>::zeros( radii_steps, angle_steps );
//        for(int i = 0 ; i < distance_surface.rows ; ++i)
//        {
//            for(int j = 0 ; j < distance_surface.cols ; ++j)
//            {
//                epsilon = sqrt( double(i*i+j*j) );
//                if(i)
//                    eta = atan( j/double(i) );
//                else
//                    eta = CV_PI/2.;
//         
//                n = 0; m = 0;
//                while(radii[n++]<epsilon && n < radii_steps){}
//                while(angle[m++]<eta && m < angle_steps){}
//                --n;--m;
//                corr = exp(-distance_surface(i,j)/(varnoise*.1));
//                //current = polar(n,m);
//         
//                polar(n,m) += corr;
//            }
//        }
//        //cv::normalize polar
//        normalize( polar, polar, 0., 1., NORM_MINMAX );
//        this->d = polar;
//        return polar;
//    }
// 
//    Mat visualize() const
//    {
//        return visualize( this->d );
//    }
// 
//    Mat visualize( const Mat_<float>& polar ) const
//    {
//        int M = 20;
//        Mat_<float> canvas;
// 
//        resize( polar, canvas, cvSize( polar.rows*M, polar.cols*M ), 0, 0, INTER_AREA );
// /*       imshow( "LSS Descriptor" , mat2gray(canvas.t()) );
// 
//        waitKey( );*/
//		return mat2gray(canvas.t());
//    }
// 
//    static const int idim = 3;
//    static const int odim = 12;
//    static const int radii_steps =3 ;
//    static const int angle_steps = 8;
//    static const int varnoise = 1;
// 
//protected:
//    Mat_<float> d;//descriptor
//    float ssd( const Mat_<Vec3b>& a, const Mat_<Vec3b>& b )
//    {
//        float c = 0;
//        for(int i = 0 ; i<a.rows ; ++i)
//            for(int j = 0 ; j<a.cols ; ++j)
//                c += ( a(i,j)-b(i,j) ).dot( a(i,j)-b(i,j) );
//        return c;
//    }
// 
//};
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
//int main ( int argc, char *argv[] )
//{
//	//Mat img = imread("Aaron_Peirsol_0002.jpg",1);
//	Mat img = imread("D:/CroppedYale/CroppedYale/yaleB11/yaleB11_P00A+000E+00.pgm",1);
//	//img=resizeImage(img,50,50);
//	Mat tan=tan_triggs_preprocessing(img,0.1, 10.0,0.2,1,2);
//	imshow("Tan",tan);
//		waitKey(1);
//	Point point(tan.rows/2,tan.cols/2);
//LSS lss(point,tan);
//Mat descriptor = lss();
//cout<<descriptor<<endl;
//Mat r1=lss.visualize();
//imshow("r1",r1);
//
//	Mat img2 = imread("D:/CroppedYale/CroppedYale/yaleB11/yaleB11_P00A-070E+45.pgm",1);
//	//img2=resizeImage(img2,50,50);
//	Mat tan2=tan_triggs_preprocessing(img2,0.1, 10.0,0.2,1,2);
//	imshow("Tan2",tan2);
//		waitKey(1);
//	Point point2(tan2.rows/2,tan2.cols/2);
//LSS lss2(point2,tan2);
//Mat descriptor2 = lss();
//cout<<descriptor<<endl;
//Mat r2=lss2.visualize();
//imshow("r2",r2);
//waitKey(0);
//	return 0;
//}