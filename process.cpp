#include"process.h"
int main(){

	float thres_bias_vertical = 15; //电池片偏转程度
	float thresholdvalue = 150; //电池片二值化阈值
	float thresh_distance = 2; //轮廓点与平均距离的差值，如果大于此数值，则选定为疑似点
	for (int name = 1; name < 7; name++){
		char imgpath[200];
		sprintf_s(imgpath, "E:\\项目\\硅片检测\\线痕检测\\Image\\6.13\\9-%d.bmp", name);
		cout << imgpath << endl;
		Mat src = imread(imgpath, 0);
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
		//-----------------------做一次粗略定位，确定ROI--------------------------------------------------
		/*int start_row = 0;
		int end_row = 0;
		for (int i = bw2.rows / 2; i < bw2.rows; i++){
			int num = sum(bw2.row(i)).val[0] / 255;
			if (num > bw2.cols / 3 && start_row == 0){
				start_row = i;
			}
			if (num > bw2.cols / 3){
				end_row = i;
			}
		}
		cout << "roi: from " << start_row << " to " << end_row << endl;*/
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
		//--------------对直线进行分析，获取轮廓最小外界矩形的旋转角度，中心-------------------------------
	/*	vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(ROI_left, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		vector<vector<Point>> cRegion;
		vector<Point2f> cCenter;
		vector<RotatedRect>cRect;
		vector<bool> selected;
		for (size_t i = 0; i < contours.size(); i++){
			for (size_t j = 0; j < contours[i].size(); j++)
			{
				contours[i][j] += Point(x, y);
			}
			vector<Point> single = contours[i];
			double area0 = contourArea(single);
			Point2f center;
			
			center.x = moments(single, true).m10 / moments(single, true).m00;
			center.y = moments(single, true).m01 / moments(single, true).m00;
			RotatedRect rect = minAreaRect(contours[i]);
			if (rect.size.width < rect.size.height){
				rect.angle = 90 + rect.angle;
				float tmp = rect.size.width;
				rect.size.width = rect.size.height;
				rect.size.height = tmp;
			}
			if (area0 > 200){
				char c[100];
				sprintf_s(c, "(%0.0f,%0.0f)", center.x, center.y);
				putText(showimg, c, center - Point2f(20, 20), FONT_HERSHEY_SIMPLEX, 0.8, cvScalar(255, 0, 255), 2);
				cRegion.push_back(contours[i]);
				cCenter.push_back(center);
				cRect.push_back(rect);
				selected.push_back(true);
				drawContours(showimg, cRegion, -1, Scalar(0, 0, 220), 1);
			}
		}
		//-------------筛选区域，只取同一条直线阈值化之后最大的一部分----------------
		for (size_t i = 0; i < cCenter.size() - 1; i++){
			for (size_t j = i + 1; j < cCenter.size(); j++){
				if (abs(cCenter[i].y - cCenter[j].y) <= 5){
					if (cRect[i].size.width > cRect[j].size.width){
						selected[i] = true;
						selected[j] = false;
					}
					else{
						selected[j] = true;
						selected[i] = false;
					}
				}
			}
		}	 
		for (size_t i = 0; i < selected.size(); i++){
			Information tmp;
			if (selected[i]){
				tmp.center = cCenter[i];
				tmp.contour = cRegion[i];
				tmp.rect = cRect[i];
				line_result.push_back(tmp);
			}
		}
		for (int i = 0; i < line_result.size() - 1; i++){
			for (int j = i + 1; j < line_result.size(); j++){
				Information tmp;
				if (line_result[i].center.y > line_result[j].center.y){
					tmp = line_result[i];
					line_result[i] = line_result[j];
					line_result[j] = tmp;
				}
			}
		} 		
		cout << "旋转角度与位置" << endl;
		line_result[0].rect.angle = (line_result[0].rect.angle + line_result[1].rect.angle + line_result[2].rect.angle)/3;
		//line_result[0].center.y = 51;
		line_result[1].rect.angle = line_result[0].rect.angle;
		//line_result[1].center.y = 84;
		line_result[2].rect.angle = line_result[0].rect.angle;
		for (int i = 0; i < line_result.size(); i++){
			Information tmp = line_result[i];
			float k = tanh((float)(tmp.rect.angle) / 180 * 3.1415926);
			float b = tmp.center.y - k*(float)tmp.center.x;
			Point start;
			start.x = 0;
			start.y = b;
			Point end;
			end.x = bw2.cols;
			end.y = end.x*k + b;
			line_result[i].lp = Vec2f(k, b);
			cout << line_result[i].center << ", width = " << line_result[i].rect.size.width
				<< ", height = " << line_result[i].rect.size.height << ", angle = " <<
				line_result[i].rect.angle << " ,直线方程为： y = " << k << " * x + " <<
				b << endl;
		 	line(showimg, start, end, Scalar(0, 255, 0), 1);
		}
		*/
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
				/*if (t.x != x + 1 && t.x != x && t.x != x + width - 1 && t.x != x + width - 2)*/{
					float distance = distance_point2lines(t, line_result[i].lp);
					char c[100];
					sprintf_s(c, "%0.0f", (distance));
					double frontscale = 0.25;
					if (j%2 == 0){
						dis_minus = Point3f(t.x, t.y,  (distance));
						dminus += distance;
						line_result[i].point_distance_minus.push_back(dis_minus);						
					}
					else{
						dis_plus = Point3f(t.x, t.y, (distance));
						dplus += distance;
						line_result[i].point_distance_plus.push_back(dis_plus);						
					}
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
		
		vector<singleimprint> finalresult2(NUM_ALLOW_LINES), finalresult;
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
		for (int i = 0; i < finalresult.size(); i++){
			cout << "线痕编号为："<<finalresult[i].BelongID<<" ,线痕位置："<<finalresult[i].LineSegment[0].ls[0]<<" , 线痕长度为："
				<<finalresult[i].LineSegment[0].ls.size()<<" , 线痕凸起高度为："<< finalresult[i].LineSegment[0].meanvalue<< endl;
		}
		for (int i = 0; i < NUM_ALLOW_LINES; i++){
			Point2f p1, p2;
			p1.y = 0;
			p1.x = -b_standard[i] / k_standard[i];
			p2.y = src.rows;
			p2.x = (p2.y - b_standard[i]) / k_standard[i];
			char txt[100];
			sprintf_s(txt, "%d", i);
			putText(showimg, txt, p1+Point2f(0,10), 1, 0.8, Scalar(255, 0, 255), 1);
			line(showimg, p1, p2, Scalar(0, 255, 0), 1);
		}
		namedWindow("content", WINDOW_NORMAL);
		imshow("content", showimg);
		imwrite("content.bmp", showimg);
		waitKey();
	}
	system("pause");
	return 1;
}
