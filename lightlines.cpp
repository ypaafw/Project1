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
int process(Mat src, float thresholdvalue, float thresh_distance, vector<singleimprint> &finalresult){
	if (src.empty())
	{
		printf("图像读取失败！");
		system("pause");
		return 0;
	}
	Mat showimg = src.clone();
	cvtColor(showimg, showimg, CV_GRAY2RGB);
	Mat bw, bw2;
	threshold(src, bw2, thresholdvalue, 255, CV_THRESH_BINARY);

	int x = 0;
	int y = 0;
	int width = src.cols;
	int height = src.rows;
	rectangle(showimg, Rect(x, y, width, height), Scalar(0, 0, 255), 2);
 
	Mat ROI_left = bw2(Range(y, y + height), Range(x, x + width)).clone();

	vector<Information> line_result;
	////-------------------------------------拟合直线，提取轮廓-------------------------------------
	lightlines(ROI_left, line_result, 3, 3, showimg, true);
	//----------计算ROI区域内直线轮廓的起伏变化,上下边沿分别存放--------------
	for (size_t i = 0; i < line_result.size(); i++){
		vector<Point> contour = line_result[i].contour;
		Point3f dis_plus, dis_minus;
		float dplus = 0;
		float dminus = 0;
		for (size_t j = 0; j < contour.size(); j++){
			Point t = contour[j];
			float distance = distance_point2lines(t, line_result[i].lp);
			char c[100];
			sprintf_s(c, "%0.0f", (distance));
			double frontscale = 0.25;
			if (j % 2 == 0){
				dis_minus = Point3f(t.x, t.y, (distance));
				dminus += distance;
				line_result[i].point_distance_minus.push_back(dis_minus);
			}
			else{
				dis_plus = Point3f(t.x, t.y, (distance));
				dplus += distance;
				line_result[i].point_distance_plus.push_back(dis_plus);
			}
		}
		line_result[i].average_distance_minus = float(dminus / width);
		line_result[i].average_distance_plus = float(dplus / width);
		//	cout << "上边沿平均距离为：" << line_result[i].average_distance_minus << " ,下边沿平均距离为：" << line_result[i].average_distance_plus << endl;
	}
	//-----------------筛选疑似区域-------------------------
	vector<vector<Point3f>> candicateregion;
	for (size_t i = 0; i < line_result.size(); i++){
		vector<Point3f> minus;
		for (size_t j = 0; j < line_result[i].point_distance_minus.size(); j++){
			Point3f t = line_result[i].point_distance_minus[j];
			if (abs(t.z - (line_result[i].average_distance_minus)) >= thresh_distance){
				minus.push_back(t);
				line(showimg, Point(t.x - 1, t.y), Point(t.x + 1, t.y), Scalar(255, 0, 0), 1);
				line(showimg, Point(t.x, t.y - 1), Point(t.x, t.y + 1), Scalar(255, 0, 0), 1);
			}
		}
		vector<Point3f> plus;
		for (size_t j = 0; j < line_result[i].point_distance_plus.size(); j++){
			Point3f t = line_result[i].point_distance_plus[j];
			if (abs(t.z - (line_result[i].average_distance_plus)) >= thresh_distance){
				plus.push_back(t);
				line(showimg, Point(t.x - 1, t.y), Point(t.x + 1, t.y), Scalar(255, 0, 0), 1);
				line(showimg, Point(t.x, t.y - 1), Point(t.x, t.y + 1), Scalar(255, 0, 0), 1);
			}
		}
		if (minus.size()){
			candicateregion.push_back(minus);
		}
		if (plus.size()){
			candicateregion.push_back(plus);
		}
	}
	//-----------------计算候选区域的方差---------------
	vector<linesegment> selectedregion;
	int a = selectregion(candicateregion, 3, 0, selectedregion);

	//--------------计算每一个线段到离其最近的标准直线的距离，将其归属到最近的直线---------------------
	for (size_t i = 0; i < selectedregion.size(); i++){
		Point2f c;
		for (int j = 0; j < selectedregion[i].ls.size(); j++){
			c += Point2f(selectedregion[i].ls[j].x, selectedregion[i].ls[j].y);
		}
		c.x /= selectedregion[i].ls.size();
		c.y /= selectedregion[i].ls.size();
		vector<distance_each_line> disInf;
		for (int j = 0; j < NUM_ALLOW_LINES; j++){
			float dis = distance_point2lines(c, Vec2f(k_standard[j], b_standard[j]));
			distance_each_line del;
			del.distance = abs(dis);
			del.ID = j;
			disInf.push_back(del);
		}
		sort(disInf.begin(), disInf.end(), SortByDistance);
		selectedregion[i].BelongToID = disInf[0].ID;
		char txt[100];
		sprintf_s(txt, "%d", disInf[0].ID);
		putText(showimg, txt, c, 1, 0.8, Scalar(255, 0, 255), 1);
	}
	int Num_Belong_Line[NUM_ALLOW_LINES] = { 0 };
	for (int i = 0; i < selectedregion.size(); i++){
		Num_Belong_Line[selectedregion[i].BelongToID]++;
	}

	vector<singleimprint> finalresult2(NUM_ALLOW_LINES);
	for (int i = 0; i < selectedregion.size(); i++){
		if (Num_Belong_Line[selectedregion[i].BelongToID] >= 2){
			singleimprint tmp;
			tmp.BelongID = selectedregion[i].BelongToID;
			finalresult2[tmp.BelongID].LineSegment.push_back(selectedregion[i]);
			finalresult2[tmp.BelongID].BelongID = tmp.BelongID;
		}
	}
	for (int i = 0; i < NUM_ALLOW_LINES; i++){
		if (!finalresult2[i].LineSegment.empty()){
			finalresult.push_back(finalresult2[i]);
		}
	}
	//----------对同属一条线痕的线段进行排序，按照y轴方向进行排序----------------------
	sort(finalresult.begin(), finalresult.end(), SortByLineSegment_Y);
	for (int i = 0; i < finalresult.size(); i++){
		cout << "线痕编号为：" << finalresult[i].BelongID << " ,线痕位置：" << finalresult[i].LineSegment[0].ls[0] << " , 线痕长度为："
			<< finalresult[i].LineSegment[0].ls.size() << " , 线痕凸起高度为：" << finalresult[i].LineSegment[0].meanvalue << endl;
	}
	for (int i = 0; i < NUM_ALLOW_LINES; i++){
		Point2f p1, p2;
		p1.y = 0;
		p1.x = -b_standard[i] / k_standard[i];
		p2.y = src.rows;
		p2.x = (p2.y - b_standard[i]) / k_standard[i];
		char txt[100];
		sprintf_s(txt, "%d", i);
		putText(showimg, txt, p1 + Point2f(0, 10), 1, 0.8, Scalar(255, 0, 255), 1);
		line(showimg, p1, p2, Scalar(0, 255, 0), 1);
	}
	namedWindow("content", WINDOW_NORMAL);
	imshow("content", showimg);
	imwrite("content.bmp", showimg);
	waitKey();

	system("pause");
	return 1;
}