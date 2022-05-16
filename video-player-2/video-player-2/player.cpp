#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int g_slider_position = 0;
int g_run = 1, g_dontset = 0;
cv::VideoCapture g_cap;

void onTrackbarSlide(int pos, void *) {
	g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);

	if (!g_dontset)
		g_run = 1;
	g_dontset = 0;
}

void onMouse(int event, int x, int y, int flag, void *param) {
	switch (event) {
	case EVENT_LBUTTONDOWN://鼠标左键按下
		cout << "鼠标左键按下" << endl;
		if (g_run == 0) g_run = -1;
		else g_run = 1;
		break;
	}
}

void openPlayer() {
	std::string s = "E:/code/C/video/video-player-2/video-player-2/demo.mp4";
	cv::namedWindow("player", WINDOW_AUTOSIZE);
	g_cap.open(s);  //视频读取结构，通过传入字符串
	
	if (!g_cap.isOpened()) {
		cout << "打开视频失败" << endl;
		exit(-1);
	}
	else {
		cout << "打开视频成功" << endl;
	}

	int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
	int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cout << "视频有" << frames << "帧大小为(" << tmpw << "," << tmph << ")的图片" << endl;

	cv::createTrackbar("Position", "player", &g_slider_position, frames, onTrackbarSlide);

	cv::Mat frame;
	for (;;) {
		if (g_run != 0) {
			g_cap >> frame;
			if (frame.empty()) break;
			int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
			g_dontset = 1;
			cv::setTrackbarPos("Position", "player", current_pos);
			cv::imshow("player", frame);
			g_run -= 1;
		}

		cv::setMouseCallback("player", onMouse, 0);
		cv::waitKey(1);
	}
	cv::destroyWindow("player");
}