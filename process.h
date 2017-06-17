#include"common.h"
/**
* imopen is an opening calculation for image, first erode then dilate. The goal is erode the noise
* @src the input image
* @dst the processed image
* @times the times of the open operation
* @return 0 represent input image is null, 1 represent in control
**/
int imopen(Mat src, Mat &dst, Mat element, int times);
/**
* distance_point2lines calculate the distance between one point and a line
* @p the point 
* @lp the parameter of line,lp[0] represents the k value, the gradient of line
		lp[1] represents the b value, the intercept of line
* @return the distance between the point p and the line lp
**/
float distance_point2lines(Point p, Vec2f lp);
/*
* linesegment the information of linesegment
* @ls the point assemblage of linesegment
* @meanvalue the meanvalue of distance
* @variance the variance of distance
* @BelongToID  the segment belong to which lines
*/
struct linesegment
{
	vector<Point3f> ls;
	float meanvalue;
	float variance;
	int BelongToID;
};
/*
* distance_each_line recode distance of point to lines and the ID of lines
*/
struct distance_each_line
{
	float distance;
	int ID;
};
bool SortByDistance(distance_each_line a, distance_each_line b);
/*
* Information the data struct about the lines of lights 
*/
struct Information
{
	vector<Point> contour ;					        //ֱ���ϱ���
//	vector<Point> contour_plus;                     //ֱ���±���
	RotatedRect rect;								//ֱ����С������
	Point2f center;									//ֱ����������
	Vec2f lp;									    //ֱ�߷��̲���,k,b
	vector<Point3f> point_distance_minus;		    //�����������Լ��䵽ֱ�߾���Ϊ����,(x,y,distance)
	vector<Point3f> point_distance_plus;		    //�����������Լ��䵽ֱ�߾���Ϊ����,(x,y,distance)
	float average_distance_minus;				    //�ϱ��ص������ߵ�ƽ������
	float average_distance_plus;                    //�±��ص������ߵ�ƽ������
};
/*
* SortByY sort vec4i data by y axis
*/
bool SortByY(Vec4i &lines1, Vec4i &lines2);
bool SortByY_point3f(Point3f p1, Point3f p2);
/*
* SortByX sort point3f data by x axis
*/
bool SortByX(Point3f p1, Point3f p2);
/**
* selectreion	evaluate the belif of candicated region
* @candicateregion	the region need to be screened
* @variance represent	the threshold
* @selectedregion	the screening result
*/
int selectregion(vector<vector<Point3f>> candicateregion, int length, float variance, vector<linesegment> &selectedregion);
/**
* lightlines	calculate the lines parameter of light
* @src		the input bw image,  
* @LineInformation		output, the data struct--Information
* @thresh_distance   the distance from concave-convex point to the center line
* @numlights   the number of lights
* @showimg     the img show in windows
* @flag_show    if or not show image
**/
int lightlines(Mat src, vector<Information> &LineInformation, float thresh_distance, int numlights, Mat &showimg, bool flag_show);
/**
* lineregress  fitting lines by LR and every times drop some point then fitting again until epochs
* @p the vector of point2f
* @dropratio the dropping ratio, must smaller than 1 
* @epochs iteration times, must bigger than 0
* @return Vec4f represents line parameter
**/
Vec4f lineregress(vector<Point2f> p, float dropratio, int epoches);
bool Sort_distance(Point3f a, Point3f b);
float k_standard[];
float b_standard[];
float theta_standard[];
#define NUM_ALLOW_LINES 22
/*
* singleimprint record these linesegments belong to the same class 
*/
struct singleimprint{
	vector<linesegment> LineSegment;
	int BelongID;
	Vec4f linefun;
};
/*
* SortByLineSegment_Y
*/
bool SortByLineSegment_Y(singleimprint a, singleimprint b);
