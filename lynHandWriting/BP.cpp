#include "BP.h"
#include "readMINIST.h"
#include <iostream>
using namespace std;
//���ļ����ܣ�����BP������
void BPnet::init()
{

    memset(dest, 0, sizeof(dest));
    memset(output, 0, sizeof(output));
    memset(hidden, 0, sizeof(hidden));
    memset(input, 0, sizeof(input));
    memset(delta_in_to_hide, 0, sizeof(delta_in_to_hide));
    memset(delta_hide_to_out, 0, sizeof(delta_hide_to_out));

    for (int i = 0; i < IN_NUM; i++)
    {
        for (int j = 0; j < HID_NUM; j++)
        {
            V[i][j] = getRandom();
        }
    }

    for (int j = 0; j < HID_NUM; j++)
    {
        for (int k = 0; k < OUT_NUM; k++)
        {
            W[j][k] = getRandom();
        }
    }
}

//����������������
void BPnet::setData(double *in, double *labeled)
{
    for (int i = 0; i < 10; i++) {
        dest[i] = labeled[i];
    }
    for (int i = 0; i < IN_NUM; i++)
    {
        input[i] = in[i];
        input[i] = (input[i] - 128.0) / 128.0;//-1~1
    }
}
void BPnet::setset(double* in)
{
    for (int i = 0; i < IN_NUM; i++)
    {
        input[i] = in[i];
        input[i] = (input[i] - 128.0) / 128.0;//������-1 ~1�ŷ����ҵ�����
       
    }
}
//ǰ�򣬼�Ȩ��ͼ���
void BPnet::forward()
{
    //����㵽����
    for (int j = 0; j < HID_NUM; j++)
    {
        double sum = 0;
        for (int i = 0; i < IN_NUM; i++)
        {
            sum += input[i] * V[i][j];
        }
        hidden[j] = sigmoid(sum);
    }

    //���㵽�����
    for (int k = 0; k < OUT_NUM; k++)
    {
        double sum = 0;
        for (int j = 0; j < HID_NUM; j++)
        {
            sum += hidden[j] * W[j][k];
        }
        output[k] = sigmoid(sum);
    }

}
//�����ݶ��½������
void BPnet::backward()
{
    //�������㵽���������
    for (int k = 0; k < OUT_NUM; k++)
    {
        delta_hide_to_out[k] = (dest[k] - output[k]) * output[k] * (1 - output[k]);
    }
    //��������㵽��������
    for (int j = 0; j < HID_NUM; j++)
    {
        double sum = 0;
        for (int k = 0; k < OUT_NUM; k++)
        {
            sum += delta_hide_to_out[k] * W[j][k];
        }
        delta_in_to_hide[j] = sum * hidden[j] * (1 - hidden[j]);
    }
}
void BPnet::update()
{
    //�������㵽���������
    for (int j = 0; j < HID_NUM; j++)
    {
        for (int k = 0; k < OUT_NUM; k++)
        {
            W[j][k] += ALPHA * delta_hide_to_out[k] * hidden[j];
        }
    }
    //���´�����㵽��������
    for (int i = 0; i < IN_NUM; i++)
    {
        for (int j = 0; j < HID_NUM; j++)
        {
            V[i][j] += ALPHA * delta_in_to_hide[j] * input[i];
        }
    }
}

//���ݸ����ı�ǩ���ж��Ƿ���Ԥ���������
bool BPnet::predict()
{
    this->forward();
    double maxP = -1.0;
    int label = 0;
    for (int k = 0; k < OUT_NUM; k++)
    {
        if (maxP < output[k])
        {
            maxP = output[k];
            label = k;
        }
    }
    if (fabs(dest[label]-1.0)<1e-5 ){
       // cout << "hit\n";
        return true;
    }
        
    else
        return false;
}
//������д�Ľ�������ж�,����֮ǰҪ��set test data
int BPnet::predict_recognize(double outputs[])
{
    this->forward();
    double maxP = -1.0;
    int label = 0;
    for (int k = 0; k < OUT_NUM; k++)
    {
        if (maxP < output[k])
        {
            maxP = output[k];
            label = k;
        }
    }
    outputs[0] = label;
    return label;//return Ԥ��ֵ
}
