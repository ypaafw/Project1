#include "enhanceImage.h";
#include"filter.h"
#define imgshow 
int enhanceImage( Mat src, Mat &dst, float low_ratio, float high_ratio, int low_out, int high_out, char *method, float paramter_gamma,bool flag_imshow, bool saveimage )
{
	if(src.empty() == 1)
	{
		cout<<"图像输入出错！"<<endl;
		system("pause");
		return 0;
	}
	if( (low_ratio + high_ratio) >= 1 || low_out > high_out)
	{
		cout<<"参数输入错误！"<<endl;
		system("pause");
		return 0;
	}
	if(src.depth() == CV_8U)
	{
		float hist[256][2];
		for(int i = 0; i < 256; i++)
		{
			hist[i][0] = i;
			hist[i][1] = 0;
		}
		for( int i = 0; i < src.rows; i++)
		{
			for(int j = 0; j < src.cols; j++)
			{
				hist[src.at<uchar>(i,j)][1]++;
			}
		}
		float sum_low = 0,sum_high = 0;
		int low_in = 0, high_in = 0;
		for(int i = 0; i < 256; i++)
		{
			hist[i][1] = hist[i][1] /(float) (src.rows * src.cols);  
		}
		for(int i = 0; i < 256; i++)
		{
			sum_low += hist[i][1];
			sum_high += hist[255-i][1];
			if(sum_low >= low_ratio && low_in == 0)
			{
				low_in = i;
			}
			if(sum_high >= high_ratio && high_in == 0)
			{
				high_in = 255 - i;
			}
		}
		cout<<"low_in = "<<low_in<<" ,high_in = "<<high_in<<endl;
		if( strcmp(method,"line") == 0)
		{
			for( int i = 0; i < src.rows; i++)
			{
				for(int j = 0; j < src.cols; j++)
				{
					if( src.at<uchar>(i,j) >= low_in && src.at<uchar>(i,j) <= high_in)
					{
						dst.at<uchar>(i,j) = uchar((float)(src.at<uchar>(i,j) - low_in)/(float)(high_in - low_in) *(float) (high_out - low_out) + low_out);					
					}
					else if( src.at<uchar>(i, j) < low_in)
					{
						dst.at<uchar>(i,j) = 0;
					}
					else
					{
						dst.at<uchar>(i, j) = high_out;
					}
				}
			}
	  //    deleteHighPoint(dst,dst,3,3,1.2);
	//	   medianBlur(dst,dst,3);
		   GaussianBlur(dst, dst, Size(3,3), 0.5);
	//		bilateralFilter(dst, dst, 3, 1, 1) ;
	//	 AdaptiveMedFilter(dst, dst, 5, 5, 9, 9);
		}
		else if( strcmp(method,"gamma") == 0)
		{
			for( int i = 0; i < src.rows; i++)
			{
				for(int j = 0; j < src.cols; j++)
				{
					if( src.at<uchar>(i,j) >= low_in && src.at<uchar>(i,j) <= high_in)
					{
						dst.at<uchar>(i,j) =  (uchar)(pow(src.at<uchar>(i,j),paramter_gamma)+0.5);
					}
					else if( src.at<uchar>(i, j) < low_in)
					{
						dst.at<uchar>(i,j) = 0;
					}
					else
					{
						dst.at<uchar>(i, j) = high_out;
					}
				}
			}	 
		 //   deleteHighPoint(dst,dst,3,3,1.5);
		//	 medianBlur(dst,dst,5);
		}  
		else if(strcmp(method,"histeq") == 0)
		{
			equalizeHist(src, dst);
		//	AdaptiveMedFilter(dst, dst, 5, 5, 9, 9);
		}
		else
		{
			cout<<"没有这种灰度拉伸方式！"<<endl;
			system("pause");
			return 0;
		}
		if( flag_imshow )
		{
			cout<<"输入图像位深为8位"<<endl;
			int ratio = dst.cols / 255 - 1;
			Mat showimage;
			cvtColor(dst, showimage, CV_GRAY2BGR);
			CvFont font;
			cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1,1,0,2,8);
			for(int i = 0; i < 254; i++)
			{
				line(showimage,Point( i * ratio ,showimage.rows - 50 - ratio * hist[i][1] * dst.rows),Point( (i+1) * ratio, showimage.rows -50-ratio*hist[i+1][1]*dst.rows),Scalar(0,255,0),3);
				char num[10];
				sprintf_s(num, "%d",i);
				if(i %5 == 0)
				{
					putText(showimage, num, Point(i * ratio - 12, showimage.rows - 30), 1, 1, Scalar(0, 0, 255));	
					line( showimage, Point( i * ratio, showimage.rows -50), Point( i * ratio, 0), Scalar(255, 0, 0),1);
				}
			}
			namedWindow("拉伸后的图像",WINDOW_NORMAL);
			imshow( "拉伸后的图像", showimage );
			waitKey();
		}
	}
	else if( src.depth() == CV_16U )
	{
		const int maxvalue = 65536;
	    float hist[maxvalue][2];
		for(int i = 0; i < maxvalue; i++)
		{
			hist[i][0] = i;
			hist[i][1] = 0;
		}
		for( int i = 0; i < src.rows; i++)
		{
			for(int j = 0; j < src.cols; j++)
			{
				hist[src.at<ushort>(i,j)][1]++;
			}
		}
		for(int i = 0; i < maxvalue; i++)
		{	
		//	cout<<i<<"="<<hist[i][1]<<" ";
			hist[i][1] = (float)hist[i][1] /(float) (src.rows * src.cols); 		
		}
		float sum_low = 0,sum_high = 0;
		int low_in = 0, high_in = 0;
		for(int i = 0; i < 65536; i++)
		{
			sum_low += hist[i][1];
			sum_high += hist[65535-i][1];
			if(sum_low > low_ratio && low_in == 0)
			{
				low_in = i;
			}
			if(sum_high > high_ratio && high_in == 0)
			{
				high_in = 65535 - i;
			}
		}
			cout<<"low_in = "<<low_in<<" , high_in = "<<high_in<<endl;
		if( strcmp(method,"line") == 0)
		{
			for( int i = 0; i < src.rows; i++)
			{
				for(int j = 0; j < src.cols; j++)
				{
					if( src.at<ushort>(i,j) >= low_in && src.at<ushort>(i,j) <= high_in)
					{
						dst.at<uchar>(i,j) = (uchar)( ((float)(src.at<ushort>(i,j) - low_in)/(float)(high_in - low_in) *(float) (high_out - low_out) + low_out));
					}
					else if( src.at<ushort>(i,j) < low_in)
					{
						//dst.at<uchar>(i,j) = (uchar)((float) src.at<ushort>(i,j)/(float)65535*255);
						dst.at<uchar>(i,j) = 0;
					}
					else
					{
						dst.at<uchar>(i, j) =  high_out;
					}
				}
			}
	 	//	deleteHighPoint(dst,dst,3,3,1.5);		
			AdaptiveMedFilter(dst, dst,3, 3, 9,9);
		}
		else if( strcmp(method,"gamma") == 0)
		{
			for( int i = 0; i < src.rows; i++)
			{
				for(int j = 0; j < src.cols; j++)
				{
					if( src.at<ushort>(i,j) >= low_in && src.at<ushort>(i,j) <= high_in)
					{
						dst.at<uchar>(i,j) =  (uchar)((pow(src.at<ushort>(i,j),paramter_gamma)+0.5)/(float)65535*(float)255);
					}
					else if( src.at<ushort>(i,j) < low_in)
					{
						//dst.at<uchar>(i,j) = (uchar)((float) src.at<ushort>(i,j)/(float)65535*255);
						dst.at<uchar>(i,j) = 0;
					}
					else
					{
						dst.at<uchar>(i, j) = src.at<ushort>(i,j);
					}
				}
			}
	//		deleteHighPoint(dst,dst,3,3,1.5);
		//	 medianBlur(dst,dst,5);
		}
		else if(strcmp(method,"histeq") == 0)
		{
			equalizeHist(src, dst);
		}
		else
		{
			cout<<"没有这种灰度拉伸方式！"<<endl;
			system("pause");
			return 0;
		}
		if( saveimage )
			imwrite( "6.4-enhance.tiff", dst );
		if( flag_imshow )
		{
			cout<<"输入图像位深为16位"<<endl;
			int ratio = maxvalue/src.cols ;
			Mat showimage;
			cvtColor(dst, showimage, CV_GRAY2BGR);
			CvFont font;
			cvInitFont( &font, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 2, 8);
			for(int i = 0; i <src.cols; i++)
			{
				line(showimage,Point( i ,showimage.rows - 50 - 100* hist[i*ratio][1] * dst.rows),Point((i+1), showimage.rows - 50 - 100*hist[(i+1)*ratio][1]*dst.rows),Scalar(0,255,0),2);
				char num[10];
				sprintf_s(num, "%d",(int)(i*ratio));
				if(i %100 == 0)
				{
					putText(showimage, num, Point(i - 12, showimage.rows - 30), 1, 1, Scalar(0, 0, 255));	
				 	line( showimage, Point( i, showimage.rows -50), Point(i, 0), Scalar(255, 0, 0),1);
				}
			}
			namedWindow("拉伸后的图像",WINDOW_NORMAL);
			imshow( "拉伸后的图像", showimage );
			waitKey();
		}
	}		
	return 1;
}
int contrast_bright(Mat src, Mat &dst, float contrast, float brightness, bool showflag, bool saveflag)
{
	double M = 0;
	if( src.depth() == CV_8U)
	{
		src.convertTo(src, CV_32F);
		for(int i = 0; i < src.rows; i++)
		{
			for(int j = 0; j < src.cols; j++)
			{
				 M +=  src.at<float>(i,j);
			}
		}
		M = M / (float)(src.rows * src.cols);
		cout << "图像均值为" << M << endl;
		Mat meanimg = Mat::ones(src.rows, src.cols, CV_32F)*M;
		Mat diff = Mat(src.rows, src.cols, CV_32FC1);
		diff = src - meanimg;
	//	cout << diff.at<float>(0, 0) << endl;
		Mat contrastvalue = diff * contrast;
		Mat brightvalue = Mat(src.rows, src.cols, CV_32F);
		brightvalue = contrastvalue + meanimg * brightness;
	// 	cout << brightvalue.at<float>(0,0) << endl;
	/*	Mat test1 = Mat::ones(2, 2,CV_32F);
		test1 += 1;
		cout << test1 << endl;*/
		float minvalue = 10000, maxvalue = 0;
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				if (brightvalue.at<float>(i, j) > maxvalue)maxvalue = brightvalue.at<float>(i, j);
				if (brightvalue.at<float>(i, j) < minvalue)minvalue = brightvalue.at<float>(i, j);
			}
		}
	//	cout << maxvalue << "," << minvalue << endl;
		brightvalue = ((abs(minvalue)*Mat::ones(src.rows, src.cols, CV_32F) + brightvalue) / (maxvalue + abs(minvalue))) * 255;
	//	cout << brightvalue(Range(30, 60), Range(30, 60)) << endl;
		brightvalue.convertTo(dst, CV_8U);
		if(saveflag)
		{
			imwrite("img-cv_8u.tiff",dst);
		}
	}
	else if( src.depth() == CV_16U)	
	{
		 for(int i = 0; i < src.rows; i++)
		{
			for(int j = 0; j < src.cols; j++)
			{
				 M += src.at<ushort>(i,j);
			}
		}
		M = M/(src.rows * src.cols);
		Mat diff = Mat(src.rows, src.cols, CV_32FC1);
		src.convertTo(src, CV_32FC1);
		diff = (src - M * Mat::ones(src.rows, src.cols, CV_32FC1));
		/*for(int i = 0; i < diff.rows; i++)
		{
			for(int j = 0; j < diff.cols; j++)
			{
				if(i%10 == 0 && j%10 == 0)
				  cout<<diff.at<float>(i,j)<<" ";
			}
			cout<<endl;
		}*/
		Mat contrastvalue = diff * contrast;
		Mat brightvalue = Mat(src.rows, src.cols, CV_32FC1);
		brightvalue = contrastvalue + M * brightness;
		brightvalue = brightvalue/(float)4095 * 65535;
		
		brightvalue.convertTo(dst, CV_8U);
		if(saveflag)
		{
			imwrite("img-cv_16u.tiff",dst);
		}
	}
	if(showflag)
	{
		namedWindow("dst",WINDOW_NORMAL);
		imshow("dst",dst);
		waitKey();
	}
	return 1;
}
