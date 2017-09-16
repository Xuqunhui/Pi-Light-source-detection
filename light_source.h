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
		cv::THRESH_BINARY);  //������ֵ������ֵl���µĽ�Ϊ0
	return output;
}

std::vector<std::vector<cv::Point>> Contour_detection(cv::Mat input)
{
	//�������
	std::vector<std::vector<cv::Point>> contours;  //������������
	cv::findContours(
		input,
		contours,
		CV_RETR_EXTERNAL,  //�����ⲿ����
		CV_CHAIN_APPROX_NONE  //ÿ��������ȫ������
	);

	//ɾ��������������
	int cmin = 50;
	int cmax = 1000;
	std::vector<std::vector<cv::Point>>::
		iterator itc = contours.begin();
	//��������ѭ��
	while (itc != contours.end()) {
		//��֤������С
		if (itc->size() < cmin || itc->size() > cmax)
			itc = contours.erase(itc);
		else
			++itc;
	}
	return contours;
}

cv::Mat Drawing(cv::Mat input, std::vector<std::vector<cv::Point>> contours)
{
	//�ڰ�ɫͼ���ϻ���ɫ����
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
		//�����ı�Ե�����  
		size_t count = contours[i].size();
		//Fitzgibbon����Բ��Ϸ�����Ҫ������6���㣬���ף�Direct Least Squares Fitting of Ellipses[1999]  
		if (count < 6)
			continue;

		cv::Mat pointsf;
		//�������еĵ�ת��Ϊ��Mat��ʽ�洢��2ά�㼯(x,y)  
		cv::Mat(contours[i]).convertTo(pointsf, CV_32F);

		//��С������ϣ�Fitzgibbon�ķ�����  
		//box��������Բ��5��������(x,y,w,h,theta)  
		cv::RotatedRect box = fitEllipse(pointsf);

		//����Щ���������֮�Ⱥܶ����Щ��Բ�޳���  
		if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 8)
			continue;

		//��̫С��̫�����Բ�޳�
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
	cv::circle(result, center, 10, CvScalar(), 2);  //�������㻭Ȧ
	return center;
}