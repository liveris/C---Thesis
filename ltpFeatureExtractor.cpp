 #include "ltpFeatureExtractor.h"

void LtpFeatureExtractor::init()
    {
        unsigned short cnt = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 3; j++)
                lut[i][j] = cnt++;
            cnt++; //we skip the 4th number (only three patterns)
        }
    }
void LtpFeatureExtractor::extractFeatures( cv::Mat src, cv::Mat dst)
    {
        Mat m;
		src.convertTo(m, CV_32F); 
		assert(m.isContinuous() && (m.channels() == 1));

        Mat n=cv::Mat::zeros(m.rows, m.cols, CV_16U);
        cout<<n<<endl;
        float thresholdNeg = -1.0 * threshold; //compute once (can move to init)

        const float *p = (const float*)m.ptr();
        float diff;
        for (int r=radius; r<m.rows-radius; r++) {
            for (int c=radius; c<m.cols-radius; c++) {
                const float cval = (p[(r+0*radius)*m.cols+c+0*radius]);
				//cout << cval<<endl;
				//unsigned char cval = m.at< unsigned char>(r,c);

                diff = p[(r-1*radius)*m.cols+c-1*radius] - cval;
				//cout <<diff<<endl;
                if (diff > threshold) 
				{n.at<unsigned short>(r,c) = lut[0][0]; 
				//cout<<n<<endl;
				}
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) = lut[0][1];
                else n.at<unsigned short>(r,c) = lut[0][2];

                diff = p[(r-1*radius)*m.cols+c+0*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[1][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[1][1];
                else n.at<unsigned short>(r,c) += lut[1][2];

                diff = p[(r-1*radius)*m.cols+c+1*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[2][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[2][1];
                else n.at<unsigned short>(r,c) += lut[2][2];

                diff = p[(r+0*radius)*m.cols+c+1*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[3][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[3][1];
                else n.at<unsigned short>(r,c) += lut[3][2];

                diff = p[(r+1*radius)*m.cols+c+1*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[4][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[4][1];
                else n.at<unsigned short>(r,c) += lut[4][2];

                diff = p[(r+1*radius)*m.cols+c+0*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[5][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[5][1];
                else n.at<unsigned short>(r,c) += lut[5][2];

                diff = p[(r+1*radius)*m.cols+c-1*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[6][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[6][1];
                else n.at<unsigned short>(r,c) += lut[6][2];

                diff = p[(r+0*radius)*m.cols+c-1*radius] - cval;
                if (diff > threshold) n.at<unsigned short>(r,c) += lut[7][0];
                else if (diff < thresholdNeg) n.at<unsigned short>(r,c) += lut[7][1];
                else n.at<unsigned short>(r,c) += lut[7][2];
            }
        }
		imshow("n",(mat2gray(n)));
		cout<<n<<endl;
		cout<<n.cols<<endl;
		cout<<n.rows<<endl;
			cout<<lut[1][1]<<endl;
			waitKey(2);
        dst += n;
    }
