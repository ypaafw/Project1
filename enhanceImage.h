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
#include"opencv\cxcore.hpp"
#include"highgui.h"
#include<opencv2/features2d/features2d.hpp>
using namespace std;
using namespace cv;
/*
函数： int enhanceImage(Mat img, Mat &dst, int low_in, int high_in, int low_out, int high_out,char *method, bool flag_imshow, bool saveimage );
输入： img—— 原始图像
	   dst —— 返回图像
	   low_in —— 灰度拉伸范围，下边界
	   high_in —— 灰度拉伸范围，上边界
	   low_out —— 灰度拉伸输出，下边界
	   high_out —— 灰度拉伸输出，上边界
	   method —— 灰度拉伸方法，"line" 代表线性变换， "gamma" 代表伽马变换							 
	   flag_imshow —— true 代表绘图
	   saveimage —— true 代表保存图像
返回值：
      1 —— 代表成功执行程序
	  0 —— 代表出错
*/
extern"C"_declspec(dllexport) int enhanceImage(Mat img, Mat &dst, float low_ratio, float high_ratio, int low_out, int high_out,char *method, float paramter_gamma, bool flag_imshow, bool saveimage );
extern"C"_declspec(dllexport) int calparameter(Mat src);
extern"C"_declspec(dllexport) int readparameter(float parameter[1024000]);
extern"C"_declspec(dllexport) int applyparameter(Mat &src, float parameter[1024000]);
void gmean(Mat src, Mat &dst, int m, int n);
extern"C"_declspec(dllexport) int contrast_bright(Mat src, Mat &dst, float contrast, float brightness, bool showflag, bool saveflag);