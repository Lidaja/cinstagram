#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	VideoCapture cap(0);
	namedWindow("image");
	while(true)
	{
		Mat frame;
		if (!cap.isOpened())
		{
			frame = imread("image.jpg",CV_LOAD_IMAGE_COLOR);
		} else {
			cap >> frame;
		}
		imshow("image",frame);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}
