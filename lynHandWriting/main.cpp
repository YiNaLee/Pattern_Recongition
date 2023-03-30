#include <vector>
#include <iostream>
#include<ctime>
#include<Windows.h>
#include"readMINIST.h"
#include "BP.h"
#include "KNN.h"
#include "Bayes.h"
#include "recognize.h"
#define TRAIN_NUM 60000//60000
#define TEST_NUM 1000 //10000
#define TRAINING_TIMES 20
#define BP_mode 1  //Bp神经网络算法
#define KNN_mode 0 //KNN算法
#define Bayes_mode 0 //贝叶斯算法
#define Recognize_mode 1  //利用BP在线识别
int main()
{
	readImage MINIST;
    MINIST.read_data_train_image();
    MINIST.read_data_test_image();
    MINIST.read_data_train_label();
    MINIST.read_data_test_label();
    MINIST.print_readMINIST();//DEBUG 后续可去除
    cout << "debug:\n";
    cout<<MINIST.Train_Data.size()<<" " << MINIST.Test_Data.size() <<endl;
    cout << MINIST.Train_Label.size() << " " << MINIST.Test_Label.size() << endl;
    cout << "输入回车开始训练\n";
    getchar();
    cout << "start\n";
   // 多次训练，对于每一个训练数据
    int times = TRAINING_TIMES;
    if (BP_mode) {
        BPnet bp;
        bp.init();//初始化各个参数
        DWORD start_time = GetTickCount();
        for (int n = 0; n < times; n++)
        {
            DWORD end_time = GetTickCount();
            cout << "第" << n + 1 << "/20轮训练 ，耗时" << end_time - start_time << "ms." << endl;
            //读入，初始化BPnet的输入层与标准输出
            for (int m = 0; m < TRAIN_NUM; m++)
            {
                //MINIST 数据读入到
                vector<double>temp;
                bp.setData(&MINIST.Train_Data[m * 784], &MINIST.Train_Label[m * 10]);
                //前向传播
                bp.forward();
                //反向传播
                bp.backward();
                //更新权重
                bp.update();
            }
        }
        //清空，防止出问题
       // MINIST.Train_Data.clear();
       // MINIST.Train_Label.clear();
        //进行测试
        int ac_num = 0;
        for (int m = 0; m < TEST_NUM; m++)
        {
            bp.setData(&MINIST.Test_Data[m * 784], &MINIST.Test_Label[m * 10]);
            bool isOk = bp.predict();
            if (isOk)
            {
                ac_num++;
            }
        }
        cout << "accuray:" << (double)ac_num / TEST_NUM << endl;

        if (Recognize_mode) {
            cout << "手写数字识别测试" << endl;
            handWritingRecognize(bp);
        }
    }
    if (KNN_mode) {//KNN算法，准确度高，缺点是慢
        int k = 5;
        KNN knn;
        knn.init(MINIST.Train_Label, TRAIN_NUM);
        knn.test(k, MINIST.Test_Data, MINIST.Train_Data, MINIST.Test_Label,MINIST.Train_Label);

    }
    if (Bayes_mode) {//Bayes分类器，
        Bayes bayes;
        bayes.init(MINIST.Train_Data, MINIST.Test_Data);
        cout << "pass init\n";
       // bayes.bayes_s(MINIST.Train_Label, MINIST.Test_Label);
    
        bayes.bbBasyes(MINIST.Train_Label,MINIST.Test_Label);
    }
   
    return 0;
}
