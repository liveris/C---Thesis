//
//
//
// 
//
//    /*  
//    GWTLib(Gabor Wavelets Transform for Face Recognition) LICENSE  
//      
//    This software is freely available for non-commercial use such as research and education.   
//    Please see the full disclaimer below.   
//      
//    We recommand you cite the reference given below in your publications related to this work..   
//      
//    Jianke Zhu, Mang I Vai and Peng Un Mak, "A New Enhanced Nearest Feature Space (ENFS)   
//    Classifier for Gabor Wavelets Features-Based Face Recognition", ICBA 2004. Lecture Notes in  
//    Computer Science 3072, Springer. pp. 124 - 131.  
//      
//          
//    Copyright (c) 2003-2005 Jackie, Jianke Zhu.  Email:jianke.zhu@gmail.com  
//      
//    http://www.cse.cuhk.edu.hk/~jkzhu  
//      
//      
//    PATENT Warning. The George Mason University has applied for a patent on the Gabor Wavelets   
//    Transform feature extraction method for face recognition. Commercial applications of this   
//    software may require a license from the George Mason University.  
//      
//    3RD PART SOFTWARE  
//      
//    The software is partly based on the following libraries:  
//      
//      - The Intel(tm) OpenCV Library  
//       
//      
//    DISCLAIMER  
//      
//    This software is provided 'as-is', without any express or implied warranty.  
//    In no event will the author be held liable for any damages arising from the  
//    use of this software.  
//      
//    Permission is granted to anyone to use this software for any non-commercial   
//    purpose, and to alter it, subject to the following restrictions:  
//      
//    1. The origin of this software must not be misrepresented; you must not claim  
//       that you wrote the original software.   
//      
//    2. Altered source versions must be plainly marked as such, and must not be   
//       misrepresented as being the original software.  
//      
//    3. This notice may not be removed or altered from any source distribution.  
//      
//    --  
//      
//    No guarantees of performance accompany this software, nor is any   
//    responsibility assumed on the part of the author.  
//      
//    This software is provided by Jianke Zhu ``as is'' and any express or implied  
//    warranties, including, but not limited to, the implied warranties of merchan-  
//    tability and fitness for a particular purpose are disclaimed. In no event shall    
//    Jianke Zhu be liable for any direct, indirect, incidental, special, exemplary,  
//    or consequential damages (including, but not limited to, procurement of substitute  
//    goods or services; loss of use, data, or profits; or business interruption) however   
//    caused and on any theory of liability, whether in contract, strict liability,   
//    or tort (including negligence or otherwise) arising in any way out of the use   
//    of this software, even if advised of the possibility of such damage.  
//      
//      
//    $Revision: 1.1 $   
//    $Date: 2005/12/02 $   
//    */   
//    #define  _CRT_SECURE_NO_DEPRECATE //This line is necessary for VC8.0    
//    #include <stdio.h>   
//    #include <direct.h>   
//#include "opencv2/contrib/contrib.hpp"
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp> 
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/highgui/highgui.hpp"
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
//    #define M_PI 3.14159265359   
//        
//    /**  
//      @author   ZhuJianke  
//      @version  06-26-2003  
//       
//      @memo     Save the CvMat to an bmp image  
//      @doc      Save the CvMat to an bmp image  
//      
//      @param    mat      CvMat   
//      @param    name     bmp file name  
//                                  
//      @return   status  
//    */   
//    int save_mat_image(CvMat* mat, char* name)   
//    {   
//        double maxVal,minVal;   
//        CvPoint minLoc, maxLoc;   
//        cvMinMaxLoc( mat, &minVal, &maxVal,&minLoc, &maxLoc, 0 );   
//        CvSize imsize={mat->rows,mat->cols};   
//        IplImage* tt=cvCreateImage(imsize,IPL_DEPTH_32F,0);    
//        tt= cvGetImage(mat,  tt);   
//        IplImage* ipl=cvCreateImage(imsize ,8,1);   
//        cvConvertScale(tt,ipl,255.0/(maxVal-minVal),-minVal*255.0/(maxVal-minVal));    
//        cvSaveImage(name,ipl);   
//        cvReleaseImage(&ipl);   
//        return 1;   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  8-20-2003  
//      
//      @memo     Calculate the mean value of a vector.  
//      @doc      Calculate the mean value of a vector at length of vSize.  
//      
//      @param    v         input vector.  
//      @param    vSize     vector length.  
//      
//      @return   mean value of the vector.  
//    */   
//    double MeanVector(double* v, int vSize)   
//    {   
//        int i;      
//        double mean=0.0;   
//        double* ptr = v;   
//       
//        for(i=0;i<vSize;i++) {   
//       
//            mean+= *ptr;   
//            ptr++;   
//        }   
//        mean /=(double)vSize;   
//        return mean;   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-7-2003  
//      
//      @memo     Calculate the zero mean unit length of a vector.  
//      @doc      Calculate the zero mean unit length of a vector length of vSize.  
//      
//      @param    v         input vector.  
//      @param    vSize     vector length.  
//        
//      @return   nothing.  
//      */   
//    void ZeroMeanUnitLength( double* v, int vSize) {         
//           
//        double sqsum = 0.0;   
//        double mean = MeanVector(v,vSize);   
//        double* ptr = v;   
//        int i;   
//            
//        for(i=0;i<vSize;i++) {   
//       
//            (*ptr)  -= mean;    
//            sqsum += (*ptr)*(*ptr);   
//            ptr++;   
//        }   
//        double a = 1.0f/(double)(sqrt(sqsum));   
//        ptr = v;   
//        for(i=0;i<vSize;i++) {   
//       
//            (*ptr) *= a;   
//            ptr++;   
//        }       
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Get cvMat2D from file binary mode.  
//      @doc      Get CvMat2D from file binary mode.  
//      @see      WriteMat  
//        
//      @param    fh          File handler.  
//      @para     isVector    m is single row  
//      
//      @return   the matrix contains data or void.  
//    */   
//     CvMat*  GetMat2( FILE *fh, BOOL isVector )   
//    {   
//        FILE *Stream;               //Stream for output.   
//        int cIndex=0;               //Counter for elements in the matrix.   
//        double Dim[2];              //The dimensions of the loaded matrix.   
//        double* Elem;               //Pointer to the elements the matrix.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        if(fread((void*)Dim,sizeof(double),2,Stream) <2 )//Retrive the dimensions.   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//        int rows,cols;   
//        if(!isVector)   
//        {   
//            rows = (int)Dim[0];//Save the dimensions of the matrix.   
//            cols = (int)Dim[1];   
//        }else   {   
//            cols = (int)Dim[0];//Save the dimensions of the matrix.   
//            rows = (int)Dim[1];   
//        }   
//       
//        Elem=(double*)malloc(rows*cols*2*sizeof(double));   //Allocate memory to retrive the data.   
//        if(Elem==NULL)   
//        {   
//            printf("Out of memory.");   
//        }   
//       
//        if(fread((void*)Elem,sizeof(double),rows*cols*2,Stream) < (unsigned)(rows*cols*2) ) //Retrive the data.   
//        {   
//            printf("Problem reading from file.");   
//        }   
//        CvMat* m  = cvCreateMatHeader(rows,cols,CV_64FC2);  cvCreateData( m);   
//        int cI,cJ;   
//        for(cI=0;cI<rows;cI++)           //Put the elements into the matrix.   
//        {   
//            for(cJ=0;cJ<cols;cJ++)   
//            {   
//                ((double*)(m->data.ptr + m->step*cI))[cJ*2]=Elem[cIndex];   
//                cIndex++;   
//                ((double*)(m->data.ptr + m->step*cI))[cJ*2+1]=Elem[cIndex];   
//                cIndex++;   
//            }   
//        }   
//        free(Elem);    
//               
//        return m;//Free allocated memory.   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Get cvMat from file binary mode.  
//      @doc      Get CvMat from file binary mode.  
//      @see      WriteMat  
//        
//      @param    fh          File handler.  
//      @para     isVector    m is single row  
//      
//      @return   the matrix contains data or void.  
//    */   
//    CvMat*  GetMat( FILE *fh, BOOL isVector )   
//    {   
//       
//        FILE *Stream;               //Stream for output.   
//        int cIndex=0;               //Counter for elements in the matrix.   
//        double Dim[2];              //The dimensions of the loaded matrix.   
//        double* Elem;               //Pointer to the elements the matrix.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        if(fread((void*)Dim,sizeof(double),2,Stream) <2 )//Retrive the dimensions.   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//        int rows,cols;   
//        if(!isVector)   
//        {   
//            rows = (int)Dim[0];//Save the dimensions of the matrix.   
//            cols = (int)Dim[1];   
//        }else   {   
//            cols = (int)Dim[0];//Save the dimensions of the matrix.   
//            rows = (int)Dim[1];   
//        }   
//        Elem=(double*)malloc(rows*cols*sizeof(double)); //Allocate memory to retrive the data.   
//        if(Elem==NULL)   
//        {   
//            printf("Out of memory.");   
//        }   
//       
//        if(fread((void*)Elem,sizeof(double),rows*cols,Stream) < (unsigned)(rows*cols) ) //Retrive the data.   
//        {   
//            printf("Problem reading from file.");   
//        }   
//        CvMat* m  = cvCreateMatHeader(rows,cols,CV_64FC1);  cvCreateData( m);   
//        int cI,cJ;   
//        for(cI=0;cI<rows;cI++)           //Put the elements into the matrix.   
//        {   
//            for(cJ=0;cJ<cols;cJ++)   
//            {   
//                CV_MAT_ELEM( *m,double,cI,cJ)=Elem[cIndex];   
//                cIndex++;   
//            }   
//        }   
//        free(Elem);    
//        return m;//Free allocated memory.   
//    }   
//     /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Writes cvMat into file binary mode.  
//      @doc      Writes CvMat into file binary mode.  
//      @see      ReadMat  
//        
//      @param    m           Cv Matrix.  
//      @param    fh          File handler.  
//      @para     isVector    m is single row  
//      
//      @return   true on success, false on file errors.  
//    */   
//    int WriteMat(CvMat* m, FILE *fh, BOOL isVector )    
//    {   
//        FILE *Stream;                   //Stream for output.   
//        int cIndex=2;                   //Counter for elements in the matrix.   
//        int cSize=m->rows *m->cols+2; //The number of elements to save.   
//        double*Elem;                    //Pointer to all the element to save.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        Elem=(double*)malloc(cSize*sizeof(double));//Allocate memory to Elem.   
//        if(Elem==NULL)   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//        if(!isVector)   
//        {   
//            Elem[0]=(double)m->rows;//Save the dimensions of the matrix.   
//            Elem[1]=(double)m->cols;   
//        }else   {   
//            Elem[0]=1.0;//Save the dimensions of the matrix.   
//            Elem[1]=(double)m->rows;   
//        }   
//        int cI,cJ;   
//        for(cI=0;cI<m->rows ;cI++)    //Write the elements of the matrix to Elem.   
//        {   
//            for(cJ=0;cJ<m->cols;cJ++)   
//            {   
//                Elem[cIndex]=CV_MAT_ELEM( *m,double,cI,cJ);   
//                cIndex++;   
//            }   
//        }   
//        if(fwrite((void*)Elem,sizeof(double),cSize,Stream) < (unsigned)cSize) //Save the data.   
//        {   
//            printf("Error, can't save the matrix!");   
//            return(0);   
//        }   
//        free(Elem); //Free memory.   
//        return(1);   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Writes cvMat2D into file binary mode.  
//      @doc      Writes CvMat2D into file binary mode.  
//      @see      ReadMat  
//        
//      @param    m           Cv Matrix.  
//      @param    fh          File handler.  
//      @para     isVector    m is single row  
//      
//      @return   true on success, false on file errors.  
//    */   
//    int WriteMat2(CvMat* m, FILE *fh, BOOL isVector )    
//    {   
//        FILE *Stream;                   //Stream for output.   
//        int cIndex=2;                   //Counter for elements in the matrix.   
//        int cSize=m->rows *m->cols*2+2;   //The number of elements to save.   
//        double*Elem;                    //Pointer to all the element to save.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        Elem=(double*)malloc(cSize*sizeof(double));//Allocate memory to Elem.   
//        if(Elem==NULL)   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//        if(!isVector)   
//        {   
//            Elem[0]=(double)m->rows;//Save the dimensions of the matrix.   
//            Elem[1]=(double)m->cols;   
//        }else   {   
//            Elem[0]=1.0;//Save the dimensions of the matrix.   
//            Elem[1]=(double)m->rows;   
//        }   
//        int cI,cJ;   
//        for(cI=0;cI<m->rows ;cI++)    //Write the elements of the matrix to Elem.   
//        {   
//            for(cJ=0;cJ<m->cols;cJ++)   
//            {     
//                Elem[cIndex]=((double*)(m->data.ptr + m->step*cI))[cJ*2];   
//                cIndex++;   
//                Elem[cIndex]=((double*)(m->data.ptr + m->step*cI))[cJ*2+1];   
//                cIndex++;   
//            }   
//        }   
//        if(fwrite((void*)Elem,sizeof(double),cSize,Stream) < (unsigned)cSize) //Save the data.   
//        {   
//            printf("Error, can't save the matrix!");   
//            return(0);   
//        }   
//        free(Elem); //Free memory.   
//        return(1);   
//    }   
//     /**  
//      @author   ZhuJianke  
//      @version  10-20-2003  
//       
//      @memo     Load gabor fiter bank from a folder  
//      
//      @doc      Load gabor fiter bank from a folder  
//      
//                     
//      @param    fldname        The folder contains gabor filter bank, 40 files in total  
//        
//      @return   40 Gabor Filter bank  
//    */   
//    CvMat** LoadGaborFFT(char* fldname)   
//    {                  
//        int i,j;   
//        char filename[512];   
//        //gabor_filter("gabor");   
//        CvMat** mGabor=(CvMat**)malloc(40*sizeof(CvMat));   
//       
//        for(i=0;i<5;i++)   
//            for(j=0;j<8;j++)   
//            {   
//                sprintf(filename,"%s/FFT%s_%d_%d.data",fldname,"gabor",i,j);   
//                FILE *ifp = fopen(filename, "rb");   
//                mGabor[i*8+j]   = GetMat2(ifp,false);   
//                fclose(ifp);   
//            }   
//        return(mGabor);   
//       
//    }   
//    /**  
//      @author   ZhuJianke  
//      @version  10-20-2003  
//       
//      @memo     Release the gabor fiter bank  
//      
//      @doc      Release the gabor fiter bank  
//      
//                     
//      @param    mGabor          Filter bank data to be released  
//        
//      @return   nothing  
//    */   
//    void UnloadGaborFFT(CvMat** mGabor)   
//    {                  
//        int i,j;   
//        
//        
//        for(i=0;i<5;i++)   
//            for(j=0;j<8;j++)     
//                cvReleaseMat(&mGabor[i*8+j]);    
//                
//        cvReleaseMat(mGabor);   
//       
//    }   
//    /**  
//      @author   ZhuJianke  
//      @version  06-25-2003  
//       
//      @memo     perform multiplication of two FFT factor in OPENCV  
//      @doc      perform multiplication of two FFT factor in OPENCV  
//      
//      @param    srcAarr         input matrix 1  
//      @param    srcBarr         input matrix 2.  
//      @param    dstarr          result matrix  
//        
//      @return   size of the observer.  
//    */   
//    int Mulfft3( const CvArr* srcAarr, const CvArr* srcBarr, CvArr* dstarr )   
//    {   
//        CvMat *srcA = (CvMat*)srcAarr;   
//        CvMat *srcB = (CvMat*)srcBarr;   
//        CvMat *dst = (CvMat*)dstarr;   
//       
//        int i,j, rows, cols;   
//        rows = srcA->rows;   
//        cols = srcA->cols;   
//        double c_re,c_im;   
//       
//        for( i=0; i<rows; i++ )   
//        {   
//            for( j = 0; j < cols; j ++ )   
//            {   
//       
//            c_re = ((double*)(srcA->data.ptr + srcA->step*i))[j*2]*((double*)(srcB->data.ptr + srcB->step*i))[j*2] -   
//                ((double*)(srcA->data.ptr + srcA->step*i))[j*2+1]*((double*)(srcB->data.ptr + srcB->step*i))[j*2+1];   
//            c_im = ((double*)(srcA->data.ptr + srcA->step*i))[j*2]*((double*)(srcB->data.ptr + srcB->step*i))[j*2+1] +   
//        ((double*)(srcA->data.ptr + srcA->step*i))[j*2+1]*((double*)(srcB->data.ptr + srcB->step*i))[j*2];   
//       
//                ((double*)(dst->data.ptr + dst->step*i))[j*2] = c_re;   
//                ((double*)(dst->data.ptr + dst->step*i))[j*2+1]   = c_im;   
//       
//            }   
//        }   
//       
//    return 1;   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Writes double array into file binary mode.  
//      @doc      Writes double array into file binary mode.  
//      @see      ReadData  
//        
//      @param    v           Double array pointer.  
//      @param    length      The length of the v.  
//      @param    fh          File handler.  
//      
//      @return   true on success, false on file errors.  
//    */   
//    int writeData(double* v, int length, FILE* fh)   
//    {   
//        FILE *Stream;                   //Stream for output.   
//        double*Elem;                    //Pointer to all the element to save.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        Elem=(double*)malloc((length+2)*sizeof(double));//Allocate memory to Elem.   
//        if(Elem==NULL)   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//        Elem[0]=(double)1;//Save the dimensions of the matrix.   
//        Elem[1]=(double)length;   
//       	cv::Mat gaborFeat;
//	gaborFeat.create(length,1,CV_32FC1);
//	//cout << "Size is = "<< endl << " "  << descriptors.size() << endl << endl;
//	for(int i=0;i<length;i++)
//	{
//		gaborFeat.at<float>(i)=v[i];
//
//	}
//	cv::FileStorage fs7("gaborFeat2.yml", cv::FileStorage::WRITE);
//	fs7 << "TestImages" << gaborFeat;
//	//fs7 << "LabelImages"<< labels;
//	fs7.release();
//        int i;   
//        for(i=0;i<length ;i++){
//		
//			Elem[i+2]=v[i];
//			std::cout<<v[i]<<std::endl;
//		}
//        if(fwrite((void*)Elem,sizeof(double),(length+2),Stream) < (unsigned)(length+2)) //Save the data.   
//        {   
//            printf("Error, can't save the matrix!");   
//            return(0);   
//        }   
//        free(Elem);    
//        return(1);   
//    }   
//    /**  
//      @author   Zhu Jianke  
//      @version  9-11-2003  
//      
//      @memo     Writes double array into file binary mode.  
//      @doc      Writes double array into file binary mode.  
//      @see      ReadData  
//        
//      @param    v           Double array pointer.  
//      @param    length      The length of the v.  
//      @param    fh          File handler.  
//      
//      @return   true on success, false on file errors.  
//    */   
//    double* getData(FILE* fh, int &length)   
//    {   
//        FILE *Stream;                   //Stream for output.   
//        double*Elem;                    //Pointer to all the element to save.   
//        double Dim[2];              //The dimensions of the loaded matrix.   
//       
//        assert( fh!=NULL );   
//        Stream = fh;   
//       
//        if(fread((void*)Dim,sizeof(double),2,Stream) <2 )//Retrive the dimensions.   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//        
//        length=(int)Dim[1];   
//       
//        Elem=(double*)malloc(length*sizeof(double));//Allocate memory to Elem.   
//        if(Elem==NULL)   
//        {   
//            printf("Can't allocate memory for saving the matrix!");   
//            return(0);   
//        }   
//       
//           
//        if(fread((void*)Elem,sizeof(double),length,Stream) < (unsigned)length) //Save the data.   
//        {   
//            printf("Error, can't save the matrix!");   
//            return(0);   
//        }   
//        return Elem;   
//    }   
//       
//    /**  
//      @author   ZhuJianke  
//      @version  06-25-2003  
//       
//      @memo     perform gabor wavlet transform on the input image  
//      @doc      perform gabor wavlet transform on the input image  
//      
//      @param    img             input image  
//      @param    objecgt         output features.  
//                                  
//        
//      @return   size of the observer.  
//    */   
//    int gabor_extraction(IplImage* img,double* object,CvMat** mGabor)   
//    {   
//       
//        int w,h;   
//        w=128;h=128;   
//        CvSize img_size = cvGetSize( img );   
//        IplImage* imtmp=cvCreateImage(img_size,IPL_DEPTH_64F,0);       
//       
//        cvConvertScale(img,imtmp,1.0,0);   
//       
//        int i,j,x,y,n;   
//       
//        char* coutfilename=(char*)malloc(512*sizeof(char));   
//       
//        CvMat* imdft=cvCreateMatHeader( w*2, h*2, CV_64FC2 );   
//        cvCreateData( imdft);   
//       
//        cvZero(imdft);   
//        for(i=0;i<h;i++)   
//            for(j=0;j<w;j++)   
//            {   
//                ((double*)(imdft->data.ptr + imdft->step*i))[j*2]=   
//                    ((double*)(imtmp->imageData + imtmp->widthStep*i))[j];   
//                ((double*)(imdft->data.ptr + imdft->step*i))[j*2+1]=0.0;   
//            }   
//       
//        cvDFT(imdft,imdft,CV_DXT_FORWARD);   
//       
//       
//        n=w*h/64;   
//       
//       
//        for (i=0;i<5;i++)   
//        {   
//            for (j=0;j<8;j++)    
//            {   
//                CvMat* gout=cvCreateMatHeader( w*2, h*2, CV_64FC2 );   
//                cvCreateData( gout);   
//               
//                Mulfft3(imdft,mGabor[i*8+j],gout);   
//                cvDFT(gout,gout,CV_DXT_INVERSE);   
//       
//       
//               /*downsample sacle factor 64*/   
//              for (x=4;x<w;x+=8)    
//                 for (y=4;y<h;y+=8)    
//                 {   
//                    double sum=((double*)(gout->data.ptr + gout->step*(x+h/2)))[(y+w/2)*2]*   
//                             ((double*)(gout->data.ptr + gout->step*(x+h/2)))[(y+w/2)*2]+   
//                             ((double*)(gout->data.ptr + gout->step*(x+h/2)))[(y+w/2)*2+1]*   
//                             ((double*)(gout->data.ptr + gout->step*(x+h/2)))[(y+w/2)*2+1];   
//                        
//                     object[(i*8+j)*n+x/8*h/8+y/8]=sqrt(sum);   
//                 }   
//               cvReleaseMat(&gout);            
//               ZeroMeanUnitLength(object+(i*8+j)*n,n);   
//            }   
//        }   
//        free(coutfilename);   
//        cvReleaseImage(&imtmp);   
//        cvReleaseMat(&imdft);   
//       
//        return(1);   
//    }   
//    /**  
//      @author   ZhuJianke  
//      @version  06-25-2003  
//      
//      @memo     Extract various the features  
//      @doc      Extract various the features  
//                as large kinds of features as possible                   
//      
//      @param    img             face image to be extracted.  
//      @param    nsize           the size of extracted feature vector              
//      
//      @return   extracted feature vector.   
//    */   
//    double* extract_features(const IplImage* img,int& nsize)   
//    {          
//        nsize = 40*128*128/64;   
//        CvSize size = cvSize(128,128);     
//        CvSize img_size = cvGetSize( img );   
//        IplImage*   ipl=    cvCreateImage(img_size,8,0);   
//        if(img->nChannels==3)   
//        {   
//           cvCvtColor(img,ipl,CV_BGR2GRAY);   
//        }else   
//        {   
//            cvCopy(img,ipl,0);   
//        }   
//        
//        if((size.width!=img_size.width)||(size.height!=img_size.height))   
//        {      
//            IplImage* tmpsize=cvCreateImage(size,8,0);     
//            cvResize(ipl,tmpsize,CV_INTER_LINEAR);   
//            cvReleaseImage( &ipl);   
//            ipl=cvCreateImage(size,8,0);       
//            cvCopy(tmpsize,ipl,0);   
//            cvReleaseImage( &tmpsize);   
//        }   
//       
//       
//        double* object=(double*)malloc(nsize*sizeof(double));   
//        IplImage* tmp=cvCreateImage(size,IPL_DEPTH_64F,0);     
//       
//        cvConvertScale(ipl,tmp,1.0,0);     
//        
//        CvMat** mGabor= LoadGaborFFT("gabor");   
//        /*Gabor wavelet*/   
//        gabor_extraction(tmp,object,mGabor);   
//        
//        ZeroMeanUnitLength(object,nsize);   
//        cvReleaseImage( &tmp);     
//        cvReleaseImage( &ipl);     
//       
//        UnloadGaborFFT(mGabor);   
//       
//        return object;   
//    }   
//       
//    /**  
//      
//      @author   ZhuJianke  
//      @version  06-23-2003  
//       
//      @memo     Gabor Wavelet Transform filter generator,image features extraction  
//      
//      @doc      perform Gabor wavelet transform on the input image  
//                test gabor wavelet transform  
//                orientation = 8;Positive How many orientations?  
//                scale = 5;      Positive How many scales?   
//                kmax = 1.57f;   Positive Pi/2 for the KMax variable for the highest frequency  
//                                with the smallest kernel size   
//                sigma = 6.28f;  Positive 2Pi for the sigma variable for every frequency   
//                centered = 0;   Should it generate the filter with 0,0 at the ll corner or  
//                                the center   
//                normalized = 1; Should the filters be normalized?   
//                mask_size = 128;Positive The size of the Gabor filter masks that it will generate  
//                      
//      @return   filter is saved in the file.  
//    */   
//    int gabor_kernel(int scale,int orientation,int mask_size,double kmax,double sigma,char* filename)   
//    {   
//       
//      FILE *cout, *sout;   
//      register int u,v,x,y;   
//         
//      char coutfilename[512],soutfilename[512];   
//       
//      double* gabor_cos = (double *)malloc (mask_size*mask_size*sizeof (double));   
//      double* gabor_sin = (double *)malloc (mask_size*mask_size*sizeof (double));   
//      int offset = mask_size / 2;   
//       
//      double f=sqrt(2.0);   
//      double sig=sigma*sigma;   
//     // for (v=0;v<scale;v++)   
//	  v=4;//l=4
//	  //to get 0,
//        for (u=0;u<orientation;u=u++) {   
//       
//            double kv=kmax/pow(f,v);   
//            double phiu=u*M_PI/4;   
//            double kv_mag=kv*kv;   
//       
//            for (x = 0; x < mask_size; x++)    
//                for (y = 0; y< mask_size; y++) {   
//                    int i=x-offset;    
//                    int j=y-offset;   
//                    double mag=(double)(i*i+j*j);   
//                    gabor_cos[x*mask_size+y] = kv_mag/sig*exp(-0.5*kv_mag*mag/sig)*   
//                                (cos(kv*(i*cos(phiu)-j*sin(phiu)))-exp(-1.0*sig/2.0));   
//                    gabor_sin[x*mask_size+y] = kv_mag/sig*exp(-0.5*kv_mag*mag/sig)*   
//                                (sin(kv*(i*cos(phiu)-j*sin(phiu)))-exp(-1.0*sig/2.0));   
//                }   
//           
//          /* Let's deal with the cosine part first */   
//       
//          sprintf(coutfilename,"gabor//C%s_%d_%d.data",filename,v,u);   
//          sprintf(soutfilename,"gabor//S%s_%d_%d.data",filename,v,u);   
//       
//          if ((cout = fopen(coutfilename, "wb")) == NULL) {   
//            printf("Cannot open file %s!\n",coutfilename);   
//            return(0);   
//          }   
//       
//          CvMat cmat = cvMat( mask_size, mask_size,CV_64FC1,gabor_cos );   
//          WriteMat(&cmat,cout,false);   
//          fclose(cout);   
//          sprintf(coutfilename,"gabor//C%s_%d_%d.bmp",filename,v,u);   
//          save_mat_image(&cmat,coutfilename);   
//          /* Now let's deal with the sine part */   
//          if ((sout = fopen(soutfilename, "wb")) == NULL) {   
//            printf("Cannot open file %s!\n",soutfilename);   
//            return(0);   
//          }   
//       
//          CvMat smat = cvMat( mask_size, mask_size,CV_64FC1,gabor_sin );   
//          WriteMat(&smat,sout,false);   
//          sprintf(soutfilename,"gabor//S%s_%d_%d.bmp",filename,v,u);   
//          save_mat_image(&smat,soutfilename);   
//          fclose(sout);   
//          
//      }   
//      free(gabor_cos);   
//      free(gabor_sin);   
//      return 1;   
//    }   
//    /**  
//      
//      @author   ZhuJianke  
//      @version  06-23-2003  
//       
//      @memo     Save pre-computed fft of gabor filter  
//      
//      @doc      Save pre-computed fft of gabor filter  
//      
//      @param    base_fld                folder which saved the gabor filter fft.  
//                      
//      @return   status.  
//    */   
//    int gabor_filter(char* base_fld)   
//    {   
//        int img_size=30;   
//        int i,j,x,y;   
//        char coutfilename[512];   
//        fprintf(stdout,"Building gabor filter bank data: ");   
//        gabor_kernel(5,4,img_size,M_PI/2.0,2*M_PI, "gabor");   
//        fprintf(stdout," OK!\n");   
//        fprintf(stdout,"Calculating Gabor filter bank FFT: ");   
//        //for (i=0;i<5;i++)   
//        //{   
//		i=4;//lambda=4
//            for (j=0;j<4;j++)    
//            {   
//                 sprintf(coutfilename,"%s\\C%s_%d_%d.data",base_fld,"gabor",i,j);   
//                 FILE *ifp = fopen(coutfilename, "rb");   
//                 CvMat* mGaborReal  = GetMat(ifp,false);   
//                 fclose(ifp);   
//                 sprintf(coutfilename,"%s\\S%s_%d_%d.data",base_fld,"gabor",i,j);   
//                 FILE *ifps = fopen(coutfilename, "rb");   
//                 CvMat* mGaborImg   = GetMat(ifps,false);   
//                 fclose(ifps);   
//            
//                CvMat* mGabor=cvCreateMatHeader( img_size*2, img_size*2, CV_64FC2 );   
//                cvCreateData( mGabor);   
//                cvZero(mGabor);   
//                 for(x=0;x<img_size;x++)   
//                    for(y=0;y<img_size;y++)   
//                        {      
//                            ((double*)(mGabor->data.ptr + mGabor->step*x))[y*2]=   
//                                    CV_MAT_ELEM(*mGaborReal,double,x,y);   
//                            ((double*)(mGabor->data.ptr + mGabor->step*x))[y*2+1]=   
//                                    CV_MAT_ELEM(*mGaborImg,double,x,y);   
//                        }   
//       
//                cvDFT(mGabor,mGabor,CV_DXT_FORWARD);   
//               
//                sprintf(coutfilename,"%s\\FFT%s_%d_%d.data",base_fld,"gabor",i,j);   
//                FILE *ofps = fopen(coutfilename, "wb");   
//                WriteMat2(mGabor,ofps,false);   
//                fclose(ofps);   
//       
//                cvReleaseMat(&mGabor);   
//                cvReleaseMat(&mGaborReal);   
//                cvReleaseMat(&mGaborImg);   
//            }   
//        //}   
//        fprintf(stdout," OK!\n");   
//        return 1;   
//    }   
//       
//    int main(int argc, char * argv[])   
//    {   
//        // Load parameters   
//        //if (argc<2)   
//        //{   
//        //    fprintf(stdout,"\nSyntax error.\nUse: gabor e <imagefile> <outputfile> or gabor b \n");   
//        //  return 0;   
//        //} 
//        //switch(argv[1][0])   
//        //{   
//        //    case 'e':   
//        //        {   
// //                   IplImage* img= cvLoadImage("original.jpg",-1);   
//	//				cvShowImage("initial", img);
//	//				cv::Mat image(img);
//	//				
//	//				cv::imshow("second",image);
//	//				IplImage* img2=cvCloneImage(&(IplImage)image);
//	//				cvShowImage("last", img2);
//	//				cv::waitKey(4);
// //                   if(img==0)   
// //                   {   
// //                       fprintf(stdout,"Can't load image files!\n");       
// //                       return  0;   
// //                   }   
// //                   int len;   
// //                   double* fea_vec = extract_features(img,len);   
//	//				printf("%d\n",*fea_vec);
// //                   FILE* fp = fopen("featureGaborFile.yml","wb");   
// //                   writeData(fea_vec,len,fp); 
// //                   fclose(fp);   
//		    mkdir("gabor");   
//                    gabor_filter("gabor");  
//
//	///*				cv::FileStorage fs2("featureGaborFile.yml", cv::FileStorage::WRITE);
//	//fs2 << "TestImages" << fea_vec;
//	//fs2.release();*/
// //                   free(fea_vec);   
// //                   fprintf(stdout,"Gabor Wavelets features %s length:%d\n","featureGaborFile.txt",len);   
//
//        //       }   
//        //        break;   
//        //    case 'b':   
//        //            mkdir("gabor");   
//        //            gabor_filter("gabor");   
//        //            break;   
//        //
//        //    default:   
//        //        fprintf(stdout,"\nUse: gabor b <imagefile> <outputfile> or gabor b \n");   
//        //}  
//        return 0;   
//     }  
//
//
