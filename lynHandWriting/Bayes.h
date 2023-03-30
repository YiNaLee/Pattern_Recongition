#pragma once
#include <iostream>
#include<vector>
#include "readMINIST.h"
using namespace std;
class Bayes {
public:
	int distribution_arr[10] = {0};//0-9样本个数
	int num_of_numbers[10] = { 0 };
	double p10x49[10][49] = { 0 };
	vector<double>train_pics;
	vector<double>test_pics;
	//vector<double>dealed_train_pics;
	double ppro[10] = {};//先验概率
	vector<double>prior_probability;//[10]*[728]
	vector<double>posterior_probability;//后验 10
	void init(vector<double>&Train_Data, vector<double>&Test_Data);
	//void bayes_func(vector<double> Train_Label, vector<double> Test_Label);
	void bbBasyes(vector<double> Train_Label, vector<double> Test_Label);
	//void bayes_s(vector<double> Train_Label, vector<double> Test_Label);
};