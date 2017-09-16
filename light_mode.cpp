#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include"light_source.h"
#include"xrmotor.h"


int main()
{
	cv::Mat frame; // 当前视频帧
	// 打开视频文件
	cv::VideoCapture capture(0);
	// 检查打开是否成功
	if (!capture.isOpened())
		return 1;
	// 取得帧速率
	double  rate = 25.0;

	bool stop(false);
	cv::Point image_center;
	//cv::namedWindow("test");

	capture >> frame;
	int l = 253,c= frame.cols/2;

	// 根据帧速率计算帧之间的等待时间，单位ms
	int delay = 1000 / rate;
	// 循环遍历视频中的全部帧
	while (!stop) {
		// 读取下一帧（如果有）
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

		// 等待一段时间，或者通过按键停止
		if (cv::waitKey(delay) >= 0)
			stop = true;
	}
	// 关闭视频文件
	// 不是必需的，因为类的析构函数会调用
	capture.release();
}
