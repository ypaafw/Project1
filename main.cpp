#include"process.h"

int main(){
	float thres_bias_vertical = 15; //���Ƭƫת�̶�
	float thresholdvalue = 150; //���Ƭ��ֵ����ֵ
	float thresh_distance = 2; //��������ƽ������Ĳ�ֵ��������ڴ���ֵ����ѡ��Ϊ���Ƶ�
	vector<singleimprint> finalresult;
	for (int name = 1; name < 7; name++){
		char imgpath[200];
		sprintf_s(imgpath, "E:\\��Ŀ\\��Ƭ���\\�ߺۼ��\\Image\\6.13\\9-%d.bmp", name);
		cout << imgpath << endl;
		Mat src = imread(imgpath, 0);
		int a = process(src,  thresholdvalue, thresh_distance, finalresult);
	}
	return 1;
}