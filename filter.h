#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<math.h>
#include<vector>
#include<string.h>
#include<time.h>
#include<algorithm>
#include<fstream>
using namespace std;
using namespace cv;
/*
void gmean(Mat src, Mat &dst, int m, int n);
几何均值滤波
m、n 代表窗口的行、列
*/
void gmean(Mat src, Mat &dst,int m, int n);
/*
 void harmean(Mat src, Mat &dst, int m, int n);
 谐波均值滤波
 m、n 代表窗口的行、列
*/
void harmean(Mat src, Mat &dst, int m, int n);
/*
 void charmean(Mat src, Mat &dst, int m, int n, float Q);
 逆谐波均值滤波
 m、n 代表窗口的行、列
 Q 称为滤波器的阶数， Q 为正，消除胡椒噪声，Q 为负，消除盐粒噪声， Q 为0，简化为算术均值滤波器  Q 为-1， 简化为谐波均值滤波
*/
void charmean(Mat src, Mat &dst, int m, int n, float Q);
/*
void AdaptiveLocalFilter(Mat src, Mat &dst, int m, int n, float Var);
自适应局部降噪滤波器
  m、n 代表窗口的行、列
  var 代表全部噪声的方差 Var
*/
void AdaptiveLocalFilter(Mat src, Mat &dst, int m, int n, float Var);
/*
void deleteHighPoint(Mat src, Mat &dst, int m, int n, float ratio);
去除孤立的高亮噪声
*/
void deleteHighPoint(Mat src, Mat &dst, int m, int n, float ratio);
void IHLF(Mat src, Mat &dst, float limate);
void AdaptiveMedFilter(Mat src, Mat &dst, int m, int n, int maxwidth, int maxheight);
