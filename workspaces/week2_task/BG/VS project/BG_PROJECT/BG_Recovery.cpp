#include "BG_H.h"
#include <opencv2/opencv.hpp>
using namespace cv;
#include <iostream>
using namespace std;

void Recovery::readVideo(string filePath)
{
	video.open(filePath);
	if (!video.isOpened())	//��video�����쳣���
	{
		cout << "video open error!" << endl;
		return;
	}
}

void Recovery::process()
{
	cout << "���ڴ�����Ƶ�����Ե�" << endl;
	double time = static_cast<double>(getTickCount());
	int frame_num = video.get(CV_CAP_PROP_FRAME_COUNT);//��ȡ��֡��
	Mat frame;//��ǰ֡
	Mat before_frame;//ǰһ֡
	Mat result_gray;//�Ҷ�ͼ

	for (int i = 0; i < frame_num; i++)
	{
		if (frame_num > 300)//��֡���϶࣬���Գ�֡����
		{
			for (int j = 0; j < 2; j++) video >> frame;
		}
		video >> frame;
		if (frame.empty())//��֡���м��
		{
			break;
		}

		int frame_position = video.get(CV_CAP_PROP_POS_FRAMES);//��ǰ�ǵڼ�֡
		//cout << "frame_position: " << frame_position << endl;

		if (frame_position == 1 || frame_position == 3) //���Ϊ��һ֡��before_frame��Ϊ�գ�
		{
			result_gray = track(frame, frame);//����track()���м�⣬����ֵ����result_gray
			background = frame.clone();
			before_frame = frame.clone();
		}
		else if (frame_position <= 214)//��֡���ΪԤ����
		{
			result_gray = track(before_frame, frame);//����track()�����˶������⣬����ֵ����result_gray
			before_frame = frame.clone();
		}
		else
		{
			result_gray = track(background, frame);//����track()�����˶������⣬����ֵ����result_gray
		}

		//���� bg
		Mat tmp = frame.clone();
		background.copyTo(tmp, result_gray);
		addWeighted(background, 0.5, tmp, 0.5, 0, background);
	}
	time = ((double)getTickCount() - time) / getTickFrequency();
	cout << "������ȡ�ɹ�" << endl;
	cout << "����ʱ��Ϊ��" << time << "��" << endl;
	return;
}

void Recovery::showResult()
{
	namedWindow("background", 0);
	resizeWindow("background", 1024, 768);
	imshow("background", background);
	waitKey(0);
}

Mat Recovery::track(Mat background, Mat frame)
{
	//תΪ�Ҷ�ͼ
	Mat gray1, gray2;
	cvtColor(background, gray1, CV_BGR2GRAY);
	cvtColor(frame, gray2, CV_BGR2GRAY);

	//����,�ҵ���֮֡�䲻ͬ��
	Mat diff;
	absdiff(gray1, gray2, diff);

	//��ֵ������
	Mat result;
	threshold(diff, result, 50, 255, CV_THRESH_BINARY);

	////��ʴ�����ͣ����������ʹ�������Ĳ��Ҹ����ȶ�����ǿ����׳�ԣ����Ƕ����޸���������������Ƶû�ã����Բ��ã�
	//Mat result_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	//Mat result_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
	//erode(result, result, result_erode);
	//dilate(result, result, result_dilate);

	//��������
	vector<vector<Point>> contours;
	findContours(result, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	//����ͼ��
	vector<Rect> boundRect(contours.size());
	int x, y, w, h;
	// cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		x = boundRect[i].x;
		y = boundRect[i].y;
		w = boundRect[i].width;
		h = boundRect[i].height;
		//����
		rectangle(result, Point(x - 100, y - 100), Point(x + w + 100, y + h + 100), Scalar(255), CV_FILLED);//��x��y��w��h�ϼӼ�����Ϊԭ��ͼ�򲻹����϶�ʱ���кۼ�����
	}
	return result;
}







