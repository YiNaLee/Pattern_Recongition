#pragma once
#include <iostream>
#include<vector>
#include "readMINIST.h"
using namespace std;
//pair<int, int>node;

class KNN {
public:
	vector<pair<int, double>>nodes;//first�洢label��second��distance
	int a[10];//���vote���
	vector<double>testlabels;
	
	//�������֣�
	void init(vector<double>labels,int num);
	static bool cmpdis(pair<int,double>a, pair<int, double> b);
	int knnfunc(int number, int k, vector<double> testimage, vector<double> trainimage, vector<double> trainlabels);//Ԥ�⺯��
	void test(int k, vector<double> testimage, vector<double> trainimage, vector <double> testlabels, vector<double> trainlabels);
};