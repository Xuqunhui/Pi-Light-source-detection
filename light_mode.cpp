#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include"light_source.h"
#include"xrmotor.h"


int main()
{
	cv::Mat frame; // ��ǰ��Ƶ֡
	// ����Ƶ�ļ�
	cv::VideoCapture capture(0);
	// �����Ƿ�ɹ�
	if (!capture.isOpened())
		return 1;
	// ȡ��֡����
	double  rate = 25.0;

	bool stop(false);
	cv::Point image_center;
	//cv::namedWindow("test");

	capture >> frame;
	int l = 253,c= frame.cols/2;

	// ����֡���ʼ���֮֡��ĵȴ�ʱ�䣬��λms
	int delay = 1000 / rate;
	// ѭ��������Ƶ�е�ȫ��֡
	while (!stop) {
		// ��ȡ��һ֡������У�
		if (!capture.read(frame))
			break;

		capture >> frame;

		image_center = light_center(l, frame, frame);

		//std::cout << "center.x is " << image_center.x << ",center.y is " << image_center.y << " " << c << std::endl;
		if (c - image_center.x > 50)
			left();
		else if (image_center.x - c > 50)
			right();
		else
			forward();
		//cv::imshow("test", frame);

		// �ȴ�һ��ʱ�䣬����ͨ������ֹͣ
		if (cv::waitKey(delay) >= 0)
			stop = true;
	}
	// �ر���Ƶ�ļ�
	// ���Ǳ���ģ���Ϊ����������������
	capture.release();
}
