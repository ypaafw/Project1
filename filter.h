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
���ξ�ֵ�˲�
m��n �����ڵ��С���
*/
void gmean(Mat src, Mat &dst,int m, int n);
/*
 void harmean(Mat src, Mat &dst, int m, int n);
 г����ֵ�˲�
 m��n �����ڵ��С���
*/
void harmean(Mat src, Mat &dst, int m, int n);
/*
 void charmean(Mat src, Mat &dst, int m, int n, float Q);
 ��г����ֵ�˲�
 m��n �����ڵ��С���
 Q ��Ϊ�˲����Ľ����� Q Ϊ������������������Q Ϊ������������������ Q Ϊ0����Ϊ������ֵ�˲���  Q Ϊ-1�� ��Ϊг����ֵ�˲�
*/
void charmean(Mat src, Mat &dst, int m, int n, float Q);
/*
void AdaptiveLocalFilter(Mat src, Mat &dst, int m, int n, float Var);
����Ӧ�ֲ������˲���
  m��n �����ڵ��С���
  var ����ȫ�������ķ��� Var
*/
void AdaptiveLocalFilter(Mat src, Mat &dst, int m, int n, float Var);
/*
void deleteHighPoint(Mat src, Mat &dst, int m, int n, float ratio);
ȥ�������ĸ�������
*/
void deleteHighPoint(Mat src, Mat &dst, int m, int n, float ratio);
void IHLF(Mat src, Mat &dst, float limate);
void AdaptiveMedFilter(Mat src, Mat &dst, int m, int n, int maxwidth, int maxheight);
