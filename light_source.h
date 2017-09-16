#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>

cv::Mat Threshold(int l, cv::Mat input, cv::Mat output)
{
	cv::threshold(
		input,
		output,
		l,
		255,
		cv::THRESH_BINARY);  //定向阈值，将数值l以下的将为0
	return output;
}

std::vector<std::vector<cv::Point>> Contour_detection(cv::Mat input)
{
	//轮廓检测
	std::vector<std::vector<cv::Point>> contours;  //定义轮廓向量
	cv::findContours(
		input,
		contours,
		CV_RETR_EXTERNAL,  //检索外部轮廓
		CV_CHAIN_APPROX_NONE  //每个轮廓的全部像素
	);

	//删除过滤无用轮廓
	int cmin = 50;
	int cmax = 1000;
	std::vector<std::vector<cv::Point>>::
		iterator itc = contours.begin();
	//所有轮廓循环
	while (itc != contours.end()) {
		//验证轮廓大小
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}
	return contours;
}

cv::Mat Drawing(cv::Mat input, std::vector<std::vector<cv::Point>> contours)
{
	//在白色图像上画黑色轮廓
	cv::Mat result(input.size(), CV_8U, cv::Scalar(255));
	cv::drawContours(result, contours,
		-1,
		0,
		1);
	return result;
}

cv::Point core(std::vector<std::vector<cv::Point>> contours)
{
	cv::Point point;
	for (size_t i = 0; i < contours.size(); i++)
	{
		//轮廓的边缘点个数  
		size_t count = contours[i].size();
		//Fitzgibbon的椭圆拟合方法，要求至少6个点，文献：Direct Least Squares Fitting of Ellipses[1999]  
		if (count < 6)
			continue;

		cv::Mat pointsf;
		//将轮廓中的点转换为以Mat形式存储的2维点集(x,y)  
		cv::Mat(contours[i]).convertTo(pointsf, CV_32F);

		//最小二次拟合（Fitzgibbon的方法）  
		//box包含了椭圆的5个参数：(x,y,w,h,theta)  
		cv::RotatedRect box = fitEllipse(pointsf);

		//把那些长轴与短轴之比很多的那些椭圆剔除。  
		if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 8)
			continue;

		//把太小与太大的椭圆剔除
		if (box.size.width > 500 && box.size.height > 500 && box.size.width < 50 && box.size.height < 50)
			continue;

		point = box.center;
	}
	return point;
}

void output(cv::Mat input, cv::Mat output, cv::Mat result)
{
	cv::namedWindow("Image");
	cv::namedWindow("process");
	cv::namedWindow("contour");
	cv::imshow("Image", input);
	cv::imshow("process", output);
	cv::imshow("contour", result);
	cv::waitKey(0);
}

cv::Point light_center(int l, cv::Mat& image, cv::Mat& result)
{
	cv::Mat gray;
	std::vector<std::vector<cv::Point>> contours;
	cv::Point center;
	if (image.channels() == 3)
		cv::cvtColor(image, image, CV_BGR2GRAY);
	gray = Threshold(l, image, gray);
	contours = Contour_detection(gray);
	center = core(contours);
	//std::cout << "center.x is " << center.x << ",center.y is " << center.y << std::endl;
	cv::circle(result, center, 10, CvScalar(), 2);  //给最亮点画圈
	return center;
}