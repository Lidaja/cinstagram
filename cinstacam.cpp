#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int filterIndex = 0;
bool mouseClick = false;
float xRatio = 0.0f;
int startX = 0;
int startY = 0;
int width;
int height;
void MouseCallbackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//filterIndex++;
		startX = x;
		startY = y;
		mouseClick = true;
	} else if (event == EVENT_LBUTTONUP)
	{
		mouseClick = false;
	}
	if (mouseClick == true)
	{
		xRatio = (x-startX)/(width*1.0);
	}
}

int pMod(int a, int b)
{
	return (a%b+b)%b;
}
Mat grayscale(Mat img)
{
	Mat grayscaled, gray3D;
	cvtColor(img,grayscaled,COLOR_RGB2GRAY);
	vector<Mat> channels;
	channels.push_back(grayscaled);
	channels.push_back(grayscaled);
	channels.push_back(grayscaled);
	merge(channels,gray3D);
	return gray3D;
}

Mat normal(Mat img)
{
	return img;
}

int main(int argc, char** argv)
{
	typedef Mat (*flt)(Mat);
	static flt filters[] = {normal, grayscale};
	VideoCapture cap(0);
	Mat firstFrame;
	cap >> firstFrame;
	width = firstFrame.cols;
	height = firstFrame.rows;
	namedWindow("image",1);
	setMouseCallback("image",MouseCallbackFunc,NULL);
	while(true)
	{
		Mat frame,filtered;
		if (!cap.isOpened())
		{
			frame = imread("image.jpg",CV_LOAD_IMAGE_COLOR);
		} else {
			cap >> frame;
		}
		if (xRatio > 0.05)
		{
			Mat frameA, frameB;
			int split;
			
			xRatio = fmin(xRatio,0.95);
			split = floor(width*xRatio);
			Rect rectA(0,0,split,height);
			Rect rectB(split,0,width-split,height);
			frameA = frame(rectA);
			frameB = frame(rectB);
			Mat filteredA = filters[pMod(filterIndex-1, 2)](frameA);
			Mat filteredB = filters[pMod(filterIndex,2)](frameB);
			hconcat(filteredA,filteredB,filtered);
		} else {
			filtered = filters[filterIndex % 2](frame);
		}
		imshow("image",filtered);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}
