#include"process.h"
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
	/*namedWindow("content2", WINDOW_NORMAL);
	imshow("content2", bw2);
	imwrite("content.bmp", showimg);*/
	//waitKey();
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
int main(){
	float thres_bias_vertical = 15; //电池片偏转程度
	float thresholdvalue = 150; //电池片二值化阈值
	float thresh_distance = 2; //轮廓点与平均距离的差值，如果大于此数值，则选定为疑似点
	vector<singleimprint> finalresult;
	for (int name = 1; name < 7; name++){
		char imgpath[200];
		sprintf_s(imgpath, "E:\\项目\\硅片检测\\线痕检测\\Image\\6.13\\9-%d.bmp", name);
		cout << imgpath << endl;
		Mat src = imread(imgpath, 0);
		process(src,  thresholdvalue, thresh_distance, finalresult);
	}
	return 1;
}