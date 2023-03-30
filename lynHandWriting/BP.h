#pragma once

/*
����ֻ��һ�������BP������
�������Լ���ȥ
*/
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace std;

#define IN_NUM 784 //����784ά����
#define HID_NUM 20 //����20ά����
#define OUT_NUM 10 //���10ά���� 0-9
#define ALPHA 0.01//ѧϰ�� 0.2ʱ׼ȷ��Ϊ91.49
//ѧϰ��Ϊ0.1ʱ׼ȷ��92.22
//0.05ʱ׼ȷ��91.74

inline double sigmoid(double x)
{
    return 1 / (1 + exp(-1 * x));
}

//����-1 ~ 1֮������ֵ
inline double getRandom()
{
    return ((2.0 * (double)rand() / RAND_MAX) - 1);
}
class BPnet {
public:
    double dest[OUT_NUM];//Ŀ����� ������
    double output[OUT_NUM];//���
    double hidden[HID_NUM];
    double input[IN_NUM];

    double V[IN_NUM][HID_NUM];//����㵽�����Ȩ��
    double W[HID_NUM][OUT_NUM];//���ز㵽������Ȩ��

    double delta_in_to_hide[HID_NUM]; //���������������
    double delta_hide_to_out[OUT_NUM]; //���������������


    void init();
    //���������������㣨�ṩ����û�й�һ�������ݣ�
    //void setData(vector<int> in, int label);
    void setData(double* in, double* labeled);
    void setset(double* in);//������Ҫ��
    void forward();

    void backward();

    void update();
    //�����������Ȩ�ؼ�������㣬�����ʵ���������ϣ��򷵻�true
    bool predict();
    int predict_recognize(double  outputs[]);
};
