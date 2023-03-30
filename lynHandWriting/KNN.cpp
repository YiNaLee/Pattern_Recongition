//���ļ����ܣ�����ڷ�ʽʵ����д����ʶ��
#include "readMINIST.h"
#include "KNN.h"
#include <algorithm>
#include<utility> 
#include<vector>
#define TRAIN_NUM 1000//60000
#define TEST_NUM 1000//10000

void KNN::test(int k, vector<double> testimage, vector<double> trainimage, vector <double> testlabels,vector<double> trainlabels)
{
    int sum = 0;
    int realvalue = 0;
    //init(testlabels, TEST_NUM);
    for (int i = 0; i < TEST_NUM; i++)
    {
        int predict = knnfunc(i, k, testimage, trainimage, trainlabels);
        //printf("pre:%d label:%d\n",predict,(int)labels1[i]);
        for (int j = 0; j < 10; j++) {
            if (fabs(testlabels[i * 10 + j]-1)<1e-5) {
                realvalue = j;
                break;
            }
        }
        if (predict == realvalue)
            sum++;
    }
     printf("k=%d   \n",k);
     printf("precision: % .5f\n", 1.0 * sum / TEST_NUM);
}
bool KNN::cmpdis(pair<int, double>a, pair<int, double> b)
{
    return a.second < b.second;//<?
}
//�Ե�number�Ų���ͼƬ��Ԥ��
int KNN::knnfunc(int number, int k, vector<double> testimage, vector<double> trainimage, vector<double> trainlabels)//Ԥ�⺯��
{
    memset(a, 0, sizeof(a));//������գ�����debug�þ�
    nodes.clear();
    nodes.resize(TRAIN_NUM);
    double dis = 0;
   // int single_Dis = 0;
    for (int i = 0; i < TRAIN_NUM; i++)
    {
        for (int j = 0; j < 784; j++) {
            if (trainimage[i * 784 + j] != 0) {
                trainimage[i * 784 + j] = 1;
            }
            if (testimage[number * 784 + j] != 0){
                testimage[number * 784 + j] = 1;
             }
           dis+= (trainimage[i * 784 + j] - testimage[number * 784 + j]) *
                (trainimage[i * 784 + j] - testimage[number * 784 + j]);
            
        }
        dis=sqrt(dis);//���ŷʽ����
        nodes[i].second= dis;
        for (int j = 0; j < 10; j++) {
            if (trainlabels[i * 10 + j] == 1) {
                nodes[i].first = j;
                break;
            }
        }
      
    }
    std::sort(nodes.begin(), nodes.end(),cmpdis);
   /* for (auto ix : nodes) {
        cout << ix.second << endl;
    }*/
   // sort(nodes, nodes + TRAIN_NUM, cmpdis);
    for (int i = 0; i < k; i++)
    {
        //ȥ��debug����
        /*cout << "debug:distance["<<i<<"]:" << nodes[i].second <<";"<<
            "labels"<<nodes[i].first<<
            endl;*/ 
        a[nodes[i].first]++;//NODES��label��ǩ��INITʱ��ֵ ȡǰK��С��
    }
    int ans = -1, minn = -1;
    for (int i = 0; i < 10; i++)
    {
        if (a[i] > minn)
        {
            minn = a[i];
            ans = i;
        }//ͶƱ���㷨
    }
    //remove debug:
   // cout << "vote to label" << ans << endl;
   // cout << endl;
    return ans;
    
}



//��ʼ��nodes��label��ǩ
void KNN::init(vector<double>labels,int num)
{
    memset(a, 0, sizeof(int) * 10);
    nodes.resize(num);
    for (int i = 0; i < num; i++) {
       // nodes[i].first = 0;
        nodes[i].second = 0;
        for (int j = 0; j < 10; j++) {
            if (labels[i * 10 + j] == 1) {
                nodes[i].first = j;
                break;
            }
        }
    }
  /*  cout << "debug:test nodes��\n";
    for (int i = 0; i < num; i++) {
        cout << i<<":"<<nodes[i].first << endl;
    }*/
    //test��ȷ

}