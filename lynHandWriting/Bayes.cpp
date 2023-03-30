//本文件功能：用bayes分类器实现手写数字识别
#include "readMINIST.h"
#include "Bayes.h"
#include <algorithm>
#include<utility> 
#include<vector>
#define TRAIN_NUM 1000//60000
#define TEST_NUM 400//10000

void rprint(vector<double>train_images);
void Bayes::init(vector<double>&Train_Data, vector<double>&Test_Data)
{
	//train_pics.resize(TRAIN_NUM*7*7);
	//dealed_train_pics.resize(TRAIN_NUM *7*7);
	//test_pics.resize(TEST_NUM*7*7);
	prior_probability.resize(7840);
	posterior_probability.resize(10);
	//把像素做二值化处理
	int threshold = 0;
	int j = 0;
	//图片压缩 28*28-》7*7
	for (int cnt = 0; cnt < TRAIN_NUM; cnt++) {
		int x = 0, y = 0;
		for (int k = 1; k < 50; k++)
		{
			int sign = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					if (Train_Data[28 * (x + i) + y + j] > 127) sign += 1;
				}
			if (sign >= 6)                                
				//img_pixel += '1';
				train_pics.push_back(1);

			else
				train_pics.push_back(0);
			y = (y + 4) % 28;                        
			if (k % 7 == 0)
			{
				x += 4;
				y = 0;
			}
		}
	}
	cout << "debug,train_pics.size应该是" << 49 * 60000 << "实际是:" << train_pics.size() << endl;
	//getchar();
	for (int cnt = 0; cnt < TEST_NUM; cnt++) {
		int x = 0, y = 0;
		for (int k = 1; k < 50; k++)
		{
			int sign = 0;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					if (Test_Data[28 * (x + i) + y + j] > 127) sign += 1;
				}
			if (sign >= 6)
				//img_pixel += '1';
				test_pics.push_back(1);

			else
				test_pics.push_back(0);
			y = (y + 4) % 28;
			if (k % 7 == 0)
			{
				x += 4;
				y = 0;
			}
		}
	}
	cout << "debug,test_pics.size应该是" << 49 * TEST_NUM<< "实际是:" << test_pics.size() << endl;
	//rprint(train_pics);
	//getchar();
}

	//贝叶斯算法
void Bayes::bbBasyes(vector<double> Train_Label, vector<double> Test_Label)
{
	int xkj[10][49] = { 0 };
	for (int i = 0; i < TRAIN_NUM; i++) {

		int real = 0;
		for (int j = 0; j < 10; j++) {
			if (Train_Label[i * 10 + j] == 1) {
				real = j;
				break;
			}
		}
		num_of_numbers[real]++;
	}
	double proportion[10][49];
	for (int i = 0; i < 49; i++)
	{
		for (int j = 0; j < TRAIN_NUM; j++)
		{
			int label = 0;//每张训练图的真实值
			for (int k = 0; k < 10; k++) {
				if (Train_Label[j * 10 + k] == 1) {
					label = k;
					break;
				}
			}
			//cout << "训练的图片是" << label << endl;
			if (train_pics[i + j * 49] == 1) {
				xkj[label][i]++;
			}
		}
		for (int m = 0; m < 10; m++)
		{
			proportion[m][i] = xkj[m][i] / (6000*1.0);   //计算第i列的的比例
			
		}
	}

	double c_num = 0;//识别正确个数
	double f_num = 0;//识别错误个数
	double r_num = 0;//拒绝识别个数
	
	double px[10];                 //记录后验概率
	for (int i = 0; i < TEST_NUM; i++)          //200张测试图片，遍历
	{
		int labell = 0;
		//判定测试的真实值
		for (int label = 0; label < 10; label++) {
			if (Test_Label[i * 10 + label] == 1) {
				labell = label;
				break;
			}
		}
		
		for (int x = 0; x < 10; x++)px[x] = 1.0;
		int m = 0;
		
		for (int o = 0; o < 10; o++) {
			for (int j = 0; j < 49; j++)            //每个图片49个二值 
			{
				if (test_pics[j+i*49] == 1) {
					px[o] *= 1.0+ proportion[o][j];
				}
				else
				{
					px[o] *= (2.0 - proportion[o][j]);
				}

			}
		}

		int recognize_num = 0;
		bool refused = false;
		double temp_prob = 0;
		for (int i = 0; i < 10; i++)   
		{
			if (px[i] > temp_prob)
			{
				temp_prob = px[i];
				recognize_num = i;
				refused = false;
			}
			else if (px[i] == temp_prob)
			{
				refused = true;
			}
		}
		if (refused)
		{
		
			r_num++;
		}
		else
		{
			if (labell == recognize_num)
			{
				//cout << "识别为：" << recognize_num << endl;
				c_num++;
			}
			else
			{
				//cout << "识别错误！" << endl;
				f_num++;
			}
		}
	}
	cout << "presicion：" << f_num / TEST_NUM << endl;
}



void rprint(vector<double>train_images)
{
	//测试一下读入是否正确：打印第一张图片；
	cout << "trainng picture" << endl;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {//fabs(x)<=1e-6
			cout << (fabs(train_images[i * 7 + j]) <= 1e-5 ? " " : "*");
		}
		cout << endl;
	}
}

