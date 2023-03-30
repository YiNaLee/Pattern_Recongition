#include "BP.h"
#include "readMINIST.h"
#include <iostream>
using namespace std;
//本文件功能：构建BP神经网络
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

//设置输入层与输出层
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
        input[i] = (input[i] - 128.0) / 128.0;//必须变成-1 ~1才符合我的网络
       
    }
}
//前向，加权求和即可
void BPnet::forward()
{
    //输入层到隐层
    for (int j = 0; j < HID_NUM; j++)
    {
        double sum = 0;
        for (int i = 0; i < IN_NUM; i++)
        {
            sum += input[i] * V[i][j];
        }
        hidden[j] = sigmoid(sum);
    }

    //隐层到输出层
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
//反向：梯度下降求误差
void BPnet::backward()
{
    //计算隐层到输出层的误差
    for (int k = 0; k < OUT_NUM; k++)
    {
        delta_hide_to_out[k] = (dest[k] - output[k]) * output[k] * (1 - output[k]);
    }
    //计算输入层到隐层的误差
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
    //更新隐层到输出层的误差
    for (int j = 0; j < HID_NUM; j++)
    {
        for (int k = 0; k < OUT_NUM; k++)
        {
            W[j][k] += ALPHA * delta_hide_to_out[k] * hidden[j];
        }
    }
    //更新从输入层到隐层的误差
    for (int i = 0; i < IN_NUM; i++)
    {
        for (int j = 0; j < HID_NUM; j++)
        {
            V[i][j] += ALPHA * delta_in_to_hide[j] * input[i];
        }
    }
}

//根据给定的标签，判断是否与预测结果相符合
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
//根据手写的结果进行判断,在这之前要先set test data
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
    return label;//return 预测值
}
