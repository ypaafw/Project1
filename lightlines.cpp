#include"process.h"
/**
* imopen is an opening calculation for image, first erode then dilate. The goal is erode the noise
* @src the input image
* @dst the processed image
* @times the times of the open operation
* @return 0 represent input image is null, 1 represent in control
**/
int imopen(Mat src, Mat &dst, Mat element, int times)
{
	if (src.empty() || element.empty())
	{
		cout << "开运算出错" << endl;
		return 0;
	}
	erode(src, src, element, Point(-1, -1), times);
	dilate(src, src, element, Point(-1, -1), times);
	dst = src;
	return 1;
}
int selectregion(vector<vector<Point3f>> candicateregion, int length, float variance, vector<linesegment> &selectedregion)
{
	//--------先将point集合沿x轴方向进行排序--------------
	for (size_t i = 0; i < candicateregion.size(); i++){
		sort(candicateregion[i].begin(), candicateregion[i].end(), SortByX);
	}
	//--------分割区域,将长度大于3的线段视作候选区域------------
	for (size_t i = 0; i < candicateregion.size(); i++){
		int flag_begin = 0, flag_end = 0;
		for (size_t k = 0; k < candicateregion[i].size() - 1; k++){
			float d = sqrt(pow(candicateregion[i][k].x - candicateregion[i][k + 1].x, 2) + pow(candicateregion[i][k].y - candicateregion[i][k + 1].y, 2));
			if (abs(candicateregion[i][k].x - candicateregion[i][k + 1].x) > 5 || k == (candicateregion[i].size() - 2)){
				flag_end = k;
				linesegment tmp;
				if (flag_end - flag_begin >= length || k == (candicateregion[i].size() - 2)){
					float meanvalue = 0;
					float var = 0;
					for (int j = flag_begin; j <= flag_end; j++){
						tmp.ls.push_back(candicateregion[i][j]);
						meanvalue += candicateregion[i][j].z;
					}
					meanvalue /= (flag_end - flag_begin);
					tmp.meanvalue = meanvalue;
					for (int j = flag_begin; j < flag_end; j++){
						var += pow(candicateregion[i][j].z - meanvalue, 2);
					}
					var /= (flag_end - flag_begin);
					tmp.variance = var;
					flag_begin = k + 1;
					if (var >= variance){
						selectedregion.push_back(tmp);
					}
				}
				else{
					flag_begin = k + 1;
				}
			}
		}
	}
	//将候选区域沿y轴方向进行排序
	for (size_t i = 0; i < selectedregion.size(); i++)
	{
		for (size_t j = i; j < selectedregion.size(); j++){
			linesegment tmp;
			if (selectedregion[i].ls[0].y > selectedregion[j].ls[0].y){
				tmp = selectedregion[i];
				selectedregion[i] = selectedregion[j];
				selectedregion[j] = tmp;
			}
		}
	}
	/*for (size_t i = 0; i < selectedregion.size(); i++){
	cout << i << "," << selectedregion[i].ls[0] << "," << selectedregion[i].ls[selectedregion[i].ls.size() - 1] <<
	" ,线段长度为：" << selectedregion[i].ls.size() << " ,平均值为：" << selectedregion[i].meanvalue << "，方差为：" << selectedregion[i].variance << endl;
	}*/
	return 1;
}
bool SortByY(Vec4i &lines1, Vec4i &lines2)
{
	return lines1[1] < lines2[1];
}
bool SortByX(Point3f p1, Point3f p2)
{
	return p1.x < p2.x;
}
bool SortByY_point3f(Point3f p1, Point3f p2)
{
	return p1.y < p2.y;
}
bool SortByDistance(distance_each_line a, distance_each_line b){
	return a.distance < b.distance;
}
float distance_point2lines(Point p, Vec2f lp)
{
	float distance = (lp[0] * p.x - p.y + lp[1]) / sqrt(lp[0] * lp[0] + 1);
	return distance;
}

bool Sort_distance(Point3f a, Point3f b)
{
	return a.z > b.z;
}
Vec4f lineregress(vector<Point2f> p, float dropratio, int epoches)
{	
	vector<Point2f> p1 = p;
	Vec4f L;
	for (int i = 0; i < epoches; i++){		
		fitLine(p1, L, CV_DIST_L2, 0, 0.01, 0.01);
		float k = L[1] / L[0];
		float b = L[3] - k * L[2];
		Vec2f LP = Vec2f(k, b);
		vector<Point3f> distance;
		for (int j = 0; j < p1.size(); j++){
			float d = abs(distance_point2lines(p1[j], LP));
			distance.push_back(Point3f(p1[j].x, p1[j].y, d));
		}
		sort(distance.begin(), distance.end(), Sort_distance);
		int beginID = (int)(distance.size() * dropratio);
		vector<Point2f> p2;
		for (int j = beginID; j < distance.size(); j++)
		{
			p2.push_back(Point2f(distance[j].x,distance[j].y));
		}
		p1 = p2;
	}
	return L;
}
int lightlines(Mat src, vector<Information> &LineInformation, float thresh_distance, int numlights, Mat &showimg, bool flag_show)
{
	for (int i = 0; i < numlights; i++){
		Information L;
		LineInformation.push_back(L);
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	Mat topedge = src(Range(0, src.rows - 1), Range(0, src.cols)) - src(Range(1,src.rows), Range(0, src.cols));
	Mat buttomedge = src(Range(1, src.rows), Range(0, src.cols)) - src(Range(0, src.rows-1), Range(0, src.cols));
	Mat edge = Mat::zeros(src.rows, src.cols, CV_8UC1);
	topedge.copyTo(edge(Range(0, src.rows - 1), Range(0, src.cols)));
	edge(Range(1, src.rows), Range(0, src.cols)) += buttomedge;
	/*namedWindow("content2", WINDOW_NORMAL);
	imshow("content2", edge);
	imwrite("edge.bmp", edge);*/
	vector<vector<int>> edgepoint;
	for (size_t i = 0; i < src.cols; i++){
		vector<int> y;
	//	cout << sum(edge.col(i)) << endl;
		int sum_y = sum(edge.col(i)).val[0] / 255;
		if (sum_y != 2*numlights){
			for (size_t r = 0; r < src.rows; r++){
				if (edge.at<uchar>(r, i) != 0){
					y.push_back(r);
				}
			}
		}
		else{
			for (size_t r = 0; r < src.rows; r++){
				if (edge.at<uchar>(r, i) != 0){
					y.push_back(r);
				}
			}
		}
		edgepoint.push_back(y);
	}

	vector<vector<Point2f>> linecenter;
	for (size_t i = 0; i < numlights; i++){
		vector<Point2f> center;
		for (size_t j = 0; j < edgepoint.size(); j++){
			if (edgepoint[j].size() == 2 * numlights){
				Point2f c, p_top,p_button;
				p_top = Point2f(j, edgepoint[j][i * 2]);
				p_button = Point2f(j, edgepoint[j][2 * i + 1]);
				c.y = (float)((p_top.y +p_button.y) / 2);
				c.x = j;
				center.push_back(c);
				LineInformation[i].contour.push_back(p_top);
				LineInformation[i].contour.push_back(p_button);
			}
		}
		if (center.size() <= src.cols/3){
			cout << "图像阈值化有问题" << endl;
			system("pause");
			return 0;
		}
		linecenter.push_back(center);
	}
	vector<Vec4f> linefun;
	for (size_t i = 0; i < linecenter.size(); i++)
	{
		Vec4f L;
		L = lineregress(linecenter[i], 0.2, 5);
		linefun.push_back(L);
	}
	
	for (int i = 0; i < linefun.size(); i++)
	{		 
		float k = linefun[i][1] / linefun[i][0];
		float b = linefun[i][3] -  k*linefun[i][2];
		Point x1, x2;
		x1.x = 0;
		x1.y = b;
		x2.x = src.cols;
		x2.y = k * x2.x + b;
		LineInformation[i].lp = Vec2f(k, b);
		line(showimg, x1, x2, Scalar(0, 0, 250), 1);
	}
	return 1;
}
 
bool SortByLineSegment_Y(singleimprint a, singleimprint b){
	return a.LineSegment[0].ls[0].y < b.LineSegment[0].ls[0].y;
}
/**
* k 各个栅线直线的斜率
* b 各个栅线直线的截距
* theta 各个栅线直线的与左下角所成的角度
*/
float k_standard[] = { -2.45454545454545, - 3.11111111111111, -4.50, -5.40, -7, -9.6666, -15.50, -30,	35,	11.3333333333333,	12.6666666666667,	6.66666666666667,	4.33333333333333,	3.54545454545455,	3.25000000000000,	2.78571428571429,	2.53333333333333,	2.16666666666667,	1.95238095238095,	1.78260869565217,	1.66666666666667,	1.50000000000000 };
float b_standard[] = { 100.818181818182,	230.888888888889,	474.500000000000,	767,	1235,	2040.00000000000,	3821,	8469, -11090, -3972, -4925.33333333333, -2813.33, -1969.66666666667, -1730.72727272727, -1703.250, -1554.21428571429, -1503.33333333333, -1357.66666666667, -1290.47619047619, -1238.69565217391, -1216, -1144 };
float theta_standard[] = { -18.8054360938530, -17.7037906665889, -13.0958577342240, -11.1337171736866, -8.14522541310849, -5.71230773958281, -3.48786548851375, -2.32219044039169, -1.30491813353010, 3.89791025385838, 6.59117953575484, 6.99922440141442, 9.72085830488280, 11.7494764629142, 13.5657734339880, 19.8856025931478, 21.9844742038338, 23.0980229093105, 26.3619400994865, 13.1107823258198, 30.2752224909725 };