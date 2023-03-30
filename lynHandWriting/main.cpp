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
#define BP_mode 1  //Bp�������㷨
#define KNN_mode 0 //KNN�㷨
#define Bayes_mode 0 //��Ҷ˹�㷨
#define Recognize_mode 1  //����BP����ʶ��
int main()
{
	readImage MINIST;
    MINIST.read_data_train_image();
    MINIST.read_data_test_image();
    MINIST.read_data_train_label();
    MINIST.read_data_test_label();
    MINIST.print_readMINIST();//DEBUG ������ȥ��
    cout << "debug:\n";
    cout<<MINIST.Train_Data.size()<<" " << MINIST.Test_Data.size() <<endl;
    cout << MINIST.Train_Label.size() << " " << MINIST.Test_Label.size() << endl;
    cout << "����س���ʼѵ��\n";
    getchar();
    cout << "start\n";
   // ���ѵ��������ÿһ��ѵ������
    int times = TRAINING_TIMES;
    if (BP_mode) {
        BPnet bp;
        bp.init();//��ʼ����������
        DWORD start_time = GetTickCount();
        for (int n = 0; n < times; n++)
        {
            DWORD end_time = GetTickCount();
            cout << "��" << n + 1 << "/20��ѵ�� ����ʱ" << end_time - start_time << "ms." << endl;
            //���룬��ʼ��BPnet����������׼���
            for (int m = 0; m < TRAIN_NUM; m++)
            {
                //MINIST ���ݶ��뵽
                vector<double>temp;
                bp.setData(&MINIST.Train_Data[m * 784], &MINIST.Train_Label[m * 10]);
                //ǰ�򴫲�
                bp.forward();
                //���򴫲�
                bp.backward();
                //����Ȩ��
                bp.update();
            }
        }
        //��գ���ֹ������
       // MINIST.Train_Data.clear();
       // MINIST.Train_Label.clear();
        //���в���
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
            cout << "��д����ʶ�����" << endl;
            handWritingRecognize(bp);
        }
    }
    if (KNN_mode) {//KNN�㷨��׼ȷ�ȸߣ�ȱ������
        int k = 5;
        KNN knn;
        knn.init(MINIST.Train_Label, TRAIN_NUM);
        knn.test(k, MINIST.Test_Data, MINIST.Train_Data, MINIST.Test_Label,MINIST.Train_Label);

    }
    if (Bayes_mode) {//Bayes��������
        Bayes bayes;
        bayes.init(MINIST.Train_Data, MINIST.Test_Data);
        cout << "pass init\n";
       // bayes.bayes_s(MINIST.Train_Label, MINIST.Test_Label);
    
        bayes.bbBasyes(MINIST.Train_Label,MINIST.Test_Label);
    }
   
    return 0;
}
