#include "filter.h";
void sortGrayValue(vector<int> &src)
{
	 int temp;
	for(int i = 0; i < src.size() - 1; i++)
	{
		for(int j = i+ 1; j < src.size(); j++)
		{
			if( src[i] < src[j])
			{
				temp = src[i];
				src[i] = src[j];
				src[j] = temp;
			}
		}
	}
}
void gmean(Mat src, Mat &dst,int m, int n)
{
	Mat dst2 = dst.clone();
	Mat src_expand = Mat::ones(src.rows + (m-1), src.cols + (n-1), src.type());
	if( src.depth() == CV_16U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<ushort>(i,j) = src.at<ushort>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 1;
				 for(int p = i - (m-1)/2; p <= i+(m-1)/2; p++)
				 {
					 for(int q = j - (n-1)/2; q <= j+(n-1)/2; q++)
					 {
						  temp = temp *src_expand.at<ushort>(p,q);
					 }
				 }
				 temp = pow(temp, (double)1/(double)m/(double)n);
				 dst2.at<ushort>(i-(m-1)/2,j-(n-1)/2) = (ushort)(temp);
			}
		}
		dst = dst2;
	}
	else if( src.depth() == CV_8U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<uchar>(i,j) = src.at<uchar>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 1;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp = temp *src_expand.at<uchar>(p,q);
					 }
				 }
				 temp = pow(temp, (double)1/(double)m/(double)n);
				 dst2.at<uchar>(i-(m-1)/2,j-(n-1)/2) = (uchar)(temp);
			}
		}
		dst = dst2;
	}
}
void harmean(Mat src, Mat &dst, int m, int n)
{
	Mat dst2 = dst.clone();
	Mat src_expand = Mat::ones(src.rows + (m-1), src.cols + (n-1), src.type());
	if( src.depth() == CV_16U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<ushort>(i,j) = src.at<ushort>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp += 1/(double)src_expand.at<ushort>(p,q);
					 }
				 }
				 temp = (double)(m*n)/temp;
				 dst2.at<ushort>(i-(m-1)/2,j-(n-1)/2) = (ushort)(temp);
			}
		}
		dst = dst2;
	}
	else if(src.depth() == CV_8U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<uchar>(i,j) = src.at<uchar>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp += 1/(double)src_expand.at<uchar>(p,q);
					 }
				 }
				 temp = (double)(m*n)/temp;
				 dst2.at<uchar>(i-(m-1)/2,j-(n-1)/2) = (uchar)(temp);
			}
		}
		dst = dst2;
	}
}
void charmean(Mat src, Mat& dst, int m, int n, float Q)
{
	Mat dst2 = dst.clone();
	Mat src_expand = Mat::ones(src.rows + (m-1), src.cols + (n-1), src.type());
	if( src.depth() == CV_16U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<ushort>(i,j) = src.at<ushort>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp1 = 0;
				 double temp2 = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp1 += pow(src_expand.at<ushort>(p,q),Q+1);
						  temp2 += pow(src_expand.at<ushort>(p,q),Q);
					 }
				 }
				 temp1 = temp1/temp2;
				 dst2.at<ushort>(i-(m-1)/2,j-(n-1)/2) = (ushort)(temp1);
			}
		}
		dst = dst2;
	} 
	else if( src.depth() == CV_8U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<uchar>(i,j) = src.at<uchar>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
 		//src_expand(Range((m-1)/2, src.rows + (m-1)/2),Range((n-1)/2, src.cols +(n-1)/2)) = src.clone();
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp1 = 0;
				 double temp2 = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp1 += pow(src_expand.at<uchar>(p,q),Q+1);
						  temp2 += pow(src_expand.at<uchar>(p,q),Q);
					 }
				 }
				 temp1 = temp1/temp2;
				 dst2.at<uchar>(i-(m-1)/2,j-(n-1)/2) = (uchar)(temp1);
			}
		}
		dst = dst2;
	}
}
void deleteHighPoint(Mat src, Mat &dst, int m, int n, float ratio)
{
	 Mat dst2 = dst.clone();
	Mat src_expand = Mat::ones(src.rows + (m-1), src.cols + (n-1), src.type());
	int count = 0;
	if( src.depth() == CV_16U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<ushort>(i,j) = src.at<ushort>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 0;
				 double meanvalue = 0;
				 double maxvalue = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp += src_expand.at<ushort>(p,q);
						  if( src_expand.at<ushort>(p,q) >maxvalue)
							  maxvalue = (double)src_expand.at<ushort>(p,q);
					 }
				 }
				 meanvalue = (temp - src_expand.at<ushort>(i,j))/(m*n-1);
				 if( /*abs(src_expand.at<ushort>(i,j) - maxvalue) < 1 &&*/ src_expand.at<ushort>(i,j) > ratio* meanvalue)
				 {
					 dst2.at<ushort>(i-(m-1)/2,j-(n-1)/2) = (ushort)(meanvalue);
					 count++;
				 }
				 else
				 {
					  dst2.at<ushort>(i-(m-1)/2,j-(n-1)/2) = src_expand.at<ushort>(i,j);  
				 }
			}
		}
		dst = dst2;
		cout<<"count = "<<count<<endl;
	}
	else if( src.depth() == CV_8U)
	{
		for(int  i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j =(n-1)/2; j < src.cols +(n-1)/2; j++)
			{
				src_expand.at<uchar>(i,j) = src.at<uchar>(i -(m-1)/2,j-(n-1)/2); 
			}
		}
		for(int i = (m-1)/2; i < src.rows + (m-1)/2; i++)
		{
			for(int j = (n-1)/2; j < src.cols + (n-1)/2; j++)
			{
				 double temp = 0;
				 double meanvalue = 0;
				 double maxvalue = 0;
				 for(int p = i - (m-1)/2; p < i+(m-1)/2+1; p++)
				 {
					 for(int q = j - (n-1)/2; q < j+(n-1)/2+1; q++)
					 {
						  temp += src_expand.at<uchar>(p,q);
						  if( src_expand.at<uchar>(p,q) >maxvalue)
							  maxvalue = (double)src_expand.at<uchar>(p,q);
					 }
				 }
				 meanvalue = (temp -(double)( src_expand.at<uchar>(i,j)))/(m*n-1);
				 if( /*abs(src_expand.at<uchar>(i,j) - maxvalue) < 1 &&*/ src_expand.at<uchar>(i,j) > ratio* meanvalue)
				 {
					 dst2.at<uchar>(i-(m-1)/2,j-(n-1)/2) = (uchar)(meanvalue);
					 count++;
				 }
				 else
				 {
					  dst2.at<uchar>(i-(m-1)/2,j-(n-1)/2) = src_expand.at<uchar>(i,j);  
				 }
			}
		}
		dst = dst2;
		cout<<"count ="<<count<<endl;
	}
};
void IHLF(Mat src, Mat &dst, float limate)
{
	int m = getOptimalDFTSize(src.rows);   //2,3,5的倍数有更高效率的傅里叶转换  
	int n = getOptimalDFTSize(src.cols);
	//把灰度图像放在左上角，在右边和下边扩展图像，扩展部分填充为0；  
     copyMakeBorder(src,dst,0,m-src.rows,0,n-src.cols,BORDER_CONSTANT,Scalar::all(0));  
     cout<<dst.size()<<endl;  
}
void AdaptiveMedFilter( Mat src, Mat &dst, int m, int n, int maxwidth, int maxheight )
{
	Mat dst1 = src.clone();
	Mat src_expand = Mat(src.rows + (maxwidth-1), src.cols + (maxheight-1), CV_8UC1);
	int medvalue = 0;
	int minvalue = 0;
	int maxvalue = 0;
	int A1, B1, A2, B2;
	for(int  j = (maxwidth-1)/2; j < src.cols + ( maxwidth-1)/2 - 1; j++)
	{
		for(int i = (maxheight-1)/2;i < src.rows + ( maxheight-1)/2 - 1; i++)
		{
			src_expand.at<uchar>(i,j) = src.at<uchar>((int)(i -(maxheight-1)/2),(int)(j-(maxwidth-1)/2)); 
		//	cout<<(int)src.at<uchar>((int)(i -(maxheight-1)/2),(int)(j-(maxwidth-1)/2))<<" "; 
		}
	//	cout<<endl;
	}
	//src_expand(Range((maxwidth-1)/2,src.rows + ( maxwidth-1)/2),Range((maxheight-1)/2,src.cols + ( maxheight-1)/2)) = src.clone(); 
	for(int i = ( maxheight - 1 ) / 2; i < src.rows; i++)
	{
		for(int j = ( maxwidth - 1 ) / 2; j < src.cols ; j++)
		{
			 int temp_m = m;
			 int temp_n = n;
			while( temp_m < maxheight && temp_n < maxwidth )
			{
				vector<int> S_xy;
				for(int p = i - ( temp_m - 1 ) / 2; p <= i + ( temp_m - 1 ) / 2; p++)
				{
					for(int q = j - ( temp_n - 1 ) / 2; q <= j + ( temp_n - 1 ) / 2; q++)
					{
						S_xy.push_back((int)(src_expand.at<uchar>(p,q)));
					//   cout<<(int)(src_expand.at<uchar>(p,q))<<" ";
					}
				}
				sortGrayValue(S_xy);
				maxvalue = S_xy[0];
				minvalue = S_xy[temp_m * temp_n-1];
				medvalue = S_xy[(int)((temp_m * temp_n + 1)/2)];
			 //	cout<<maxvalue <<" "<<minvalue<<" "<< medvalue<<endl;
			//	cout<<i<<" "<<j<<endl;
				A1 = medvalue - minvalue;
				A2 = medvalue - maxvalue;
				if( A1 > 0 && A2 < 0)
				{
					B1 = (int)(src_expand.at<uchar>(i,j)) - minvalue;
					B2 = (int)(src_expand.at<uchar>(i,j)) - maxvalue;
					if(B1 > 0 && B2 < 0)
					{
						dst1.at<uchar>(i,j) = src_expand.at<uchar>(i,j);
						break;
					}
					else
					{
						dst1.at<uchar>(i,j) = (uchar) medvalue;
						break;
					}
				}
				else
				{
					 temp_n += 2;
					 temp_m += 2;
					 if(temp_m > maxheight && temp_n > maxwidth)
					 {
						dst1.at<uchar>(i,j) = (uchar) medvalue;
						break;
					 }
				}
			}
		}
	}
	dst = dst1;
}
