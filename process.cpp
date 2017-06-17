#include"process.h"

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