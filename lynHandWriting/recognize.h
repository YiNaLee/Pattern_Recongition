#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "BP.h"
#include <opencv2\imgproc\types_c.h>
using namespace cv;
using namespace cv::ml;
using namespace std;
//鼠标处理函数
class BoxExtractor {
public:
	BoxExtractor();
	Mat MouseDraw(Mat& img);
	int MouseDraw(const std::string& windowName, Mat& img, Scalar color, int border);
	struct handlerT {
		bool isDrawing;
		std::vector<cv::Point> points;
		Mat image;
		handlerT() :isDrawing(false) {};
	}params;
private:
	static void mouseHandler(int event, int x, int y, int flags, void* param);
	void opencv_mouse_callback(int event, int x, int y, int, void* param);
};
//分割数字函数
void getSegment(const Mat& srcImage, vector<Mat>& arr, Mat& showImage);
bool cmp(const Rect& a, const Rect& b);
bool cmp2(const Rect& a, const Rect& b);

void handWritingRecognize(BPnet& bpNeuronNet);

typedef void(*getBPfeature)(const Mat& srcDataMat, Mat& dstDataMat, Size size);
void resizeImage(Mat& srcImage, Size size);
void binaryInit(Mat& srcDataMat, Size size);
void getHistogramFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size);
void getAllPixelsFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size);
void getMixedFeature(const Mat& srcImage, Mat& dstImage, Size size);
void image2Vec(const Mat& srcImage, Mat& dstImage);
void normal(Mat& srcImage, Mat& dstImage);
