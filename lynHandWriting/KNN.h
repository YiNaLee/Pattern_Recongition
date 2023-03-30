#pragma once
#include <iostream>
#include<vector>
#include "readMINIST.h"
using namespace std;
//pair<int, int>node;

class KNN {
public:
	vector<pair<int, double>>nodes;//first存储label，second存distance
	int a[10];//存放vote结果
	vector<double>testlabels;
	
	//函数部分：
	void init(vector<double>labels,int num);
	static bool cmpdis(pair<int,double>a, pair<int, double> b);
	int knnfunc(int number, int k, vector<double> testimage, vector<double> trainimage, vector<double> trainlabels);//预测函数
	void test(int k, vector<double> testimage, vector<double> trainimage, vector <double> testlabels, vector<double> trainlabels);
};