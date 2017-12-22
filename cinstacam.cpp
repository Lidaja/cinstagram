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
bool reset;
void MouseCallbackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		startX = x;
		startY = y;
		mouseClick = true;
	} else if (event == EVENT_LBUTTONUP)
	{
		mouseClick = false;
		reset = true;
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

Mat to3D(Mat img)
{
	Mat now3D;
	vector<Mat> channels;
	channels.push_back(img);
	channels.push_back(img);
	channels.push_back(img);
	merge(channels,now3D);
	return now3D;
}

Mat grayscale(Mat img)
{
	Mat grayscaled;
	cvtColor(img,grayscaled,COLOR_RGB2GRAY);
	Mat now3D = to3D(grayscaled);
	return now3D;
}

Mat normal(Mat img)
{
	return img;
}

Mat overlay(Mat img, Mat over, int xOff, int yOff)
{
	Mat overlayed;
	Mat bgra[4];
	split(over,bgra);
	Mat overAlpha = bgra[3]/255.0;
	return overlayed;
}

int main(int argc, char** argv)
{

	Mat captureUI = imread("assets/ui/capture.png");
	
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
		if (mouseClick){
			if (xRatio > 0.05)
			{
				Mat frameA, frameB;
				int imSplit;
				xRatio = fmin(xRatio,0.95);
				imSplit = floor(width*xRatio);
				Rect rectA(0,0,imSplit,height);
				Rect rectB(imSplit,0,width-imSplit,height);
				frameA = frame(rectA);
				frameB = frame(rectB);
				Mat filteredA = filters[pMod(filterIndex-1, 2)](frameA);
				Mat filteredB = filters[pMod(filterIndex,2)](frameB);
				hconcat(filteredA,filteredB,filtered);
			}else if (xRatio < -0.05){
				Mat frameA, frameB;
				int imSplit;
				xRatio = fmax(xRatio,-0.95);
				imSplit = floor(width*(1+xRatio));
				Rect rectA(0,0,imSplit,height);
				Rect rectB(imSplit,0,width-imSplit,height);
				frameA = frame(rectA);
				frameB = frame(rectB);
				Mat filteredA = filters[pMod(filterIndex,2)](frameA);
				Mat filteredB = filters[pMod(filterIndex+1,2)](frameB);
				hconcat(filteredA,filteredB,filtered);	
			}else{
				filtered = filters[pMod(filterIndex,2)](frame);
			}
		} else if (reset){
			if (xRatio >= 0.5)
			{
				filterIndex--;
			}
			else if (xRatio < 0.5)
			{
				filterIndex++;
			}
			reset = false;
			xRatio = 0;
			filtered = filters[pMod(filterIndex,2)](frame);
		}
		else {
			filtered = filters[pMod(filterIndex,2)](frame);
		}
		imshow("image",filtered);
		if(waitKey(30) >= 0) break;
	}
	return 0;
}
