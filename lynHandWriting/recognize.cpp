//本文件功能：使用openCV实现在窗口内输入手写数字进行识别
#define WINDOW_NAME "predict"
#include "recognize.h"
//鼠标处理：
BoxExtractor::BoxExtractor() {

}
void BoxExtractor::mouseHandler(int event, int x, int y, int flags, void* param) {
	BoxExtractor* self = static_cast<BoxExtractor*>(param);
	self->opencv_mouse_callback(event, x, y, flags, param);
}
void BoxExtractor::opencv_mouse_callback(int event, int x, int y, int, void* param) {
	handlerT* data = (handlerT*)param;
	switch (event)
	{
	case EVENT_MOUSEMOVE:
		if (data->isDrawing) {
			data->points.push_back(cv::Point(x, y));
		}
		break;
	case EVENT_LBUTTONDOWN:
		data->isDrawing = true;
		data->points.clear();
		data->points.push_back(cv::Point(x, y));
		break;
	case EVENT_LBUTTONUP:
		data->isDrawing = false;
		break;
	}
}
Mat BoxExtractor::MouseDraw(Mat& img) {
	MouseDraw("画数字", img, Scalar(0, 0, 0), 4);
	return img;
}
int BoxExtractor::MouseDraw(const std::string& windowName, Mat& img, Scalar color, int border) {
	int key = 0;
	imshow(windowName, img);
	cout << "鼠标写下数字，按回车输出预测结果，按esc退出" << endl;
	params.image = img.clone();
	setMouseCallback(windowName, mouseHandler, (void*)&params);
	int cnt = 0;
	while (!(key == 32 || key == 27 || key == 13)) {
		int length = params.points.size();
		for (int i = 0; i < length && length>2; i++) {
			if (i == length - 1) {
				cv::line(params.image, params.points.at(i), params.points.at(i), color, border);
			}
			else {
				cv::line(params.image, params.points.at(i), params.points.at(i + 1), color, border);
			}
		}
		imshow(windowName, params.image);
		key = waitKey(1);

	}
	params.image.copyTo(img);
	return key;
}
//数字分割函数 用于写下多个数字时的情况
bool cmp(const Rect& a, const Rect& b) {
	return a.x < b.x;
}
bool cmp2(const Rect& a, const Rect& b) {
	return a.tl().y < b.tl().y;
}
void sortRect(vector<Rect>& arr) {
	sort(arr.begin(), arr.end(), cmp2);
	vector<Rect>::iterator s = arr.begin();
	vector<Rect>::iterator e = arr.end();
	vector<Rect>::iterator ptr = arr.begin();
	vector<Rect>::iterator preptr = ptr++;
	for (;; ++ptr, ++preptr) {
		if (ptr == arr.end() || ptr->tl().y > preptr->br().y) {
			e = ptr;
			sort(s, e, cmp);
			s = ptr;
			if (ptr == arr.end())
				break;
		}
	}
}
void getSegment(const Mat& srcImage, vector<Mat>& arr, Mat& showImage) {
	Mat tmpImage = srcImage.clone();
	if (tmpImage.type() != CV_8UC1) {
		cvtColor(tmpImage, tmpImage, CV_BGR2GRAY);
	}
	threshold(tmpImage, tmpImage, 0, 255, CV_THRESH_BINARY);
	vector<vector<Point>> contours;
	findContours(tmpImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//边缘检测
	Mat tmpImage3 = srcImage.clone();
	vector<Rect> rectArr;
	for (int i = 0; i < contours.size(); ++i) {
		Rect rec = boundingRect(contours[i]);
		if (rec.area() >= 8 * 8) {
			rectArr.push_back(rec);
		}
	}
	sortRect(rectArr);
	for (int i = 0; i < rectArr.size(); ++i) {
		Mat tmp = tmpImage3(rectArr[i]);
		rectangle(showImage, rectArr[i], Scalar(100), 2);//根据边缘绘制矩形
		normal(tmp, tmp);

		arr.push_back(tmp);
	}
}

void handWritingRecognize(BPnet& bpNeuronNet) {
	Mat srcImage(500, 500, CV_8UC1, Scalar::all(0)), image;
	namedWindow(WINDOW_NAME);
	while (true) {
		BoxExtractor box;
		srcImage.copyTo(image);
		int key = box.MouseDraw(WINDOW_NAME, image, Scalar::all(255), 5);
		if (key != 13) {
			break;
		}
		vector<Mat> arr;
		getSegment(image, arr, image);
		cout << "预测结果" << endl;
		for (int i = 0; i < arr.size(); ++i) {
			resize(arr[i], arr[i], Size(20, 20));//归一化
			copyMakeBorder(arr[i], arr[i], 4, 4, 4, 4, BORDER_CONSTANT, Scalar::all(0));//边界填充，填充纯黑边界
			uchar** array = new uchar * [arr[i].rows];
			for (int j = 0; j < arr[i].rows; j++) {
				array[j] = new uchar[arr[i].cols];
			}
			for (int j = 0; j < arr[i].rows; j++) {
				for (int k = 0; k < arr[i].cols; k++) {
					array[j][k] = arr[i].at<uchar>(j, k);
				}
			}
			double netTest[28 * 28];
			
			for (int j = 0; j < 28; j++) {
				for (int k = 0; k < 28; k++) {
					netTest[j * 28 + k] = (double)array[j][k];
					//> 0 ? 1.0 : 0.0;
					
				}
				
			}
			double temp[10] = {};
			double predict_res[10] = {};
			bpNeuronNet.setset(netTest);//类似SetData函数
			int id=bpNeuronNet.predict_recognize(predict_res);
			cout << id<< " ";
		}
		cout << endl;
		imshow(WINDOW_NAME, image);
		cout << "输入任意字符进行下一次的输入，输入esc回到上一级" << endl;
		key = waitKey(0);
		if (key == 27) {
			break;
		}
		else {
			continue;
		}
		cout << endl;

	}
	destroyWindow(WINDOW_NAME);
}


//feature
void resizeImage(Mat& srcImage, Size size) {
	if (srcImage.rows * srcImage.cols < size.area()) {
		resize(srcImage, srcImage, size, 0, 0, INTER_CUBIC);
	}
	else if (srcImage.rows * srcImage.cols > size.area()) {
		resize(srcImage, srcImage, size, 0, 0, INTER_AREA);
	}
}
//二值化
void binaryInit(Mat& srcDataMat, Size size) {
	srcDataMat.convertTo(srcDataMat, CV_8UC1);
	normal(srcDataMat, srcDataMat);
	resizeImage(srcDataMat, size);
	threshold(srcDataMat, srcDataMat, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	srcDataMat.convertTo(srcDataMat, CV_32FC1);
}
//归一化
void normal(Mat& srcImage, Mat& dstImage) {
	int bottom = srcImage.rows + 1;
	int top = 0;
	int left = srcImage.cols + 1;
	int right = 0;
	for (int i = 0; i < srcImage.rows; ++i) {
		for (int j = 0; j < srcImage.cols; ++j) {
			if (srcImage.at<uchar>(i, j) > 0) {
				bottom = min(bottom, i);
				top = max(top, i);
				left = min(left, j);
				right = max(right, j);
			}
		}
	}
	Rect rec = Rect(left, bottom, right - left + 1, top - bottom + 1);
	Mat roi = srcImage(rec);
	int width = roi.cols;
	int height = roi.rows;
	int longLen = max(width, height);
	int shortLen = width + height - longLen;
	dstImage = Mat(longLen, longLen, CV_8UC1, Scalar::all(0));
	roi.copyTo(dstImage(Rect((longLen - width) / 2, (longLen - height) / 2, width, height)));

}
//统计大于阈值的像素个数
float calBigNumber(const Mat& srcDataMat, float thresh) {
	float sum = 0.0;
	for (int i = 0; i < srcDataMat.rows; ++i) {
		for (int j = 0; j < srcDataMat.cols; ++j) {
			if (srcDataMat.at<float>(i, j) > thresh) {
				sum += 1.0;
			}
		}
	}
	return sum;
}
void getHistogram(const Mat& srcMat, Mat& histogramMat, int flag) {//flag为0时计算行的直方图，为1时计算列的直方图
	float sum = 0.0;
	int size = flag == 0 ? srcMat.rows : srcMat.cols;
	Mat listOfMat;
	for (int i = 0; i < size; ++i) {
		listOfMat = flag == 0 ? srcMat.row(i) : srcMat.col(i);
		sum = calBigNumber(listOfMat, 0);
		histogramMat.at<float>(0, i) = sum;
	}
	normalize(histogramMat, histogramMat, 1.0, 0.0, NORM_MINMAX);

}
void getAllPixelsFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size) {
	Mat tmpDataMat = srcDataMat.clone();
	binaryInit(tmpDataMat, size);
	image2Vec(tmpDataMat, dstDataMat);
}
void getHistogramFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size) {
	Mat tmpDataMat = srcDataMat.clone();
	binaryInit(tmpDataMat, size);
	Mat rowHistogram = Mat::zeros(1, tmpDataMat.rows, CV_32FC1), colHistogram = Mat::zeros(1, tmpDataMat.cols, CV_32FC1);
	getHistogram(tmpDataMat, rowHistogram, 0);
	getHistogram(tmpDataMat, colHistogram, 1);
	int cnt = 0;
	Mat tmpDstMat = Mat::zeros(1, rowHistogram.cols + colHistogram.cols, CV_32FC1);
	for (int i = 0; i < rowHistogram.cols; ++i) {
		tmpDstMat.at<float>(0, cnt++) = rowHistogram.at<float>(0, i);
	}
	for (int i = 0; i < colHistogram.cols; ++i) {
		tmpDstMat.at<float>(0, cnt++) = colHistogram.at<float>(0, i);
	}
	dstDataMat = tmpDstMat.clone();
}
void image2Vec(const Mat& srcImage, Mat& dstImage) {
	if (srcImage.isContinuous()) {
		dstImage = srcImage.reshape(1, 1);
	}
	else {
		for (int i = 0; i < srcImage.rows; ++i) {
			for (int j = 0; j < srcImage.cols; ++j) {
				dstImage.at<float>(i * srcImage.cols + j) = srcImage.at<float>(i, j);
			}
		}
	}
}
void getMixedFeature(const Mat& srcDataMat, Mat& dstDataMat, Size size) {
	Mat tmpDataMat1;
	getAllPixelsFeature(srcDataMat, tmpDataMat1, Size(8, 8));
	normalize(tmpDataMat1, tmpDataMat1, 1.0, 0.0, NORM_MINMAX);//归一化为0，1区间
	Mat tmpDataMat2;
	getHistogramFeature(srcDataMat, tmpDataMat2, size);
	Mat tmpDataMat3 = Mat::zeros(1, tmpDataMat1.cols + tmpDataMat2.cols, CV_32FC1);
	int cnt = 0;
	for (int i = 0; i < tmpDataMat1.cols; ++i) {
		tmpDataMat3.at<float>(0, cnt++) = tmpDataMat1.at<float>(i);
	}
	for (int i = 0; i < tmpDataMat2.cols; ++i) {
		tmpDataMat3.at<float>(0, cnt++) = tmpDataMat2.at<float>(i);
	}
	dstDataMat = tmpDataMat3.clone();

}