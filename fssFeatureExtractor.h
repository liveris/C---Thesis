	#include <opencv2/opencv.hpp>
	#include "opencv2/core/core.hpp"
	#include "opencv2/contrib/contrib.hpp"
	#include <opencv2/objdetect/objdetect.hpp>
	#include <opencv2/nonfree/nonfree.hpp>
	#include <opencv2/nonfree/features2d.hpp>
	#include "opencv2/highgui/highgui.hpp"
	#include "opencv2/imgproc/imgproc.hpp"
	#include "imageUtils.h"

	using namespace std;
	using namespace cv;

	class LSS
	{
	private:
	Mat image;
	public:
	LSS(){}

	LSS( Mat im )
	{
		image=im;
		this->spatialFss(image);
		//this->compute ( p, im );
	}

	~LSS() { 

		// default parameter initialization here
	}
	Mat_<float> operator()(void)
	{
		return this->spatialFss(image);
	}

	Mat_<float> compute( const Point& p, const Mat& input )
	{
		float odim=input.cols;
		//if( p.x-odim/2 < 0 || p.y-odim/2 < 0 || p.x+odim/2 > input.cols || p.y+odim/2 > input.rows)
		//   throw Exception( 1 , "point too close to border" , "compute_descriptor" , "lss.h" , 22 );

		Mat_<uchar> imageTemp(input);

		Mat_<uchar> ipatch = imageTemp( Range(p.y-idim/2,p.y+idim/2), Range(p.x-idim/2, p.x+idim/2)  );
		//		imshow("ipatch",ipatch);
		//		waitKey(1);

		Mat_<uchar> opatch = imageTemp( Range(p.y-(odim/2),p.y+(odim/2)), Range(p.x-(odim/2), p.x+(odim/2))  );
		Mat_<uchar> w;
		Mat_<float> distance_surface( odim/idim, odim/idim );
		Mat_<float> polar;

		float epsilon, eta, corr, current;
		float* angle = new float[angle_steps];
		float* radii = new float[radii_steps];

		for (int i = 0 ; i < angle_steps ; ++i)
			angle[i] = i * 2. * CV_PI/angle_steps;
		for(int i  = 0 ; i < radii_steps ; ++i)
			radii[i] = i*(odim/2)/radii_steps;

		for(int i = 0, ii=0 ; i < odim ; i += idim, ++ii)
		{

			for(int j = 0, jj = 0 ; j < odim ; j += idim, ++jj)
			{

				w = opatch(Range(i,i+idim),Range(j,j+idim));
				distance_surface(ii,jj) = ssd(ipatch,w);
			}
		}

		//cv::normalize
		normalize( distance_surface, distance_surface, 0., 1., NORM_MINMAX );
		//Mat viz;
		//resize( distance_surface, viz, cvSize( distance_surface.rows*20, distance_surface.cols*20 ));
		//imshow( "Dist Surf" , mat2gray(viz ));

		//waitKey( );
		//log-polar
		int n, m;
		polar = Mat_<float>::zeros( radii_steps, angle_steps );
		for(int i = 0 ; i < distance_surface.rows ; ++i)
		{
			for(int j = 0 ; j < distance_surface.cols ; ++j)
			{
				epsilon = sqrt( double(i*i+j*j) );
				if(i)
					eta = atan( j/double(i) );
				else
					eta = CV_PI/2.;

				n = 0; m = 0;
				while(radii[n++]<epsilon && n < radii_steps){}
				while(angle[m++]<eta && m < angle_steps){}
				--n;--m;
				corr = exp(-distance_surface(i,j)/(varnoise*.1));
				//current = polar(n,m);

				polar(n,m) += corr;
			}
		}
		//cv::normalize polar
		normalize( polar, polar, 0., 1., NORM_MINMAX );
		this->d = polar;
		return polar;
	}

	Mat  spatialFss( Mat image) {
		int len = 256;
		int step= 6;
		int gridx=2, gridy=2;
		int width =40; //image.cols/gridx;
		int height =40; //image.rows/gridy;
		//Mat resultTemp = Mat::zeros(0, 0, CV_32F);
		Mat result = Mat::zeros(0, 0, CV_32F);
		// iterate through grid
		//for(int i = 0; i < gridy; i++) {
		//for(int j = 0; j < gridx; j++) {
		for(int i = 0; i < image.rows; i=i+step) {
			for(int j = 0; j < image.cols; j=j+step) {
				if((i+height)<=image.rows&& (j+width)<=image.cols){
					//cout<<i<<endl;
					//cout<<j<<endl;
					Mat patch(image, Range(i,i+height), Range(j,j+width));
				
					Point point(patch.rows/2,patch.rows/2);
			/*		cv::circle( patch, point,  3, cv::Scalar(255,255,0) );
						imshow("patch",patch);
					waitKey(1);*/
					Mat hist= compute(  point, patch );
					//Mat hist = Mat::zeros(1,len,CV_32F);
					// !! your code here (minus the img loading) :  lbp(patch,hist,lookup);
					//hist=patchELbp(patch);
					//cout<<hist<<endl;
					result.push_back(hist);
					//result=resultTemp.reshape(0,1);
				}
			}
		}
		//cout<<result.size<<endl;
		return result.reshape(0,1);
	}




	void visualize() const
	{
		visualize( this->d );
	}

	void visualize( const Mat_<float>& polar ) const
	{
		int M = 20;
		Mat_<float> canvas;

		resize( polar, canvas, cvSize( polar.rows*M, polar.cols*M ), 0, 0, INTER_AREA );
		imshow( "LSS Descriptor" , mat2gray(canvas.t()) );

		waitKey( );
	}

	static const int idim = 5;
	// static const int odim = 12;
	static const int radii_steps =4 ;
	static const int angle_steps = 20;
	static const int varnoise = 1;

	protected:
	Mat_<float> d;//descriptor
	float ssd( const Mat_<uchar>& a, const Mat_<uchar>& b )
	{
		float c = 0;
		for(int i = 0 ; i<a.rows ; ++i)
			for(int j = 0 ; j<a.cols ; ++j)
				c += ( a(i,j)-b(i,j) )*( a(i,j)-b(i,j) );
		return c;
	}

	};