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
������ int enhanceImage(Mat img, Mat &dst, int low_in, int high_in, int low_out, int high_out,char *method, bool flag_imshow, bool saveimage );
���룺 img���� ԭʼͼ��
	   dst ���� ����ͼ��
	   low_in ���� �Ҷ����췶Χ���±߽�
	   high_in ���� �Ҷ����췶Χ���ϱ߽�
	   low_out ���� �Ҷ�����������±߽�
	   high_out ���� �Ҷ�����������ϱ߽�
	   method ���� �Ҷ����췽����"line" �������Ա任�� "gamma" ����٤��任							 
	   flag_imshow ���� true �����ͼ
	   saveimage ���� true ������ͼ��
����ֵ��
      1 ���� ����ɹ�ִ�г���
	  0 ���� �������
*/
extern"C"_declspec(dllexport) int enhanceImage(Mat img, Mat &dst, float low_ratio, float high_ratio, int low_out, int high_out,char *method, float paramter_gamma, bool flag_imshow, bool saveimage );
extern"C"_declspec(dllexport) int calparameter(Mat src);
extern"C"_declspec(dllexport) int readparameter(float parameter[1024000]);
extern"C"_declspec(dllexport) int applyparameter(Mat &src, float parameter[1024000]);
void gmean(Mat src, Mat &dst, int m, int n);
extern"C"_declspec(dllexport) int contrast_bright(Mat src, Mat &dst, float contrast, float brightness, bool showflag, bool saveflag);