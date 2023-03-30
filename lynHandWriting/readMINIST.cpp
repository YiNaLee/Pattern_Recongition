#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>
#include "readMINIST.h"

using namespace std;
//本文件功能：将MINIST数据集读入内存
const char* train_images_idx3_ubyte_file = "train-images.idx3-ubyte";
const char* train_labels_idx1_ubyte_file = "train-labels.idx1-ubyte";
const char* test_images_idx3_ubyte_file = "t10k-images.idx3-ubyte";
const char* test_labels_idx1_ubyte_file = "t10k-labels.idx1-ubyte";
void readImage::read_data_train_image()
{
    char32_t MAGIC_NUMBER = 0;
    char32_t NUM_OF_IMAGES = 0;
    char32_t NUM_OF_ROWS = 0;
    char32_t NUM_OF_COLS = 0;
    FILE* fp = fopen(train_images_idx3_ubyte_file, "rb");
    if (fp == NULL)
    {
        printf("unable open train-images.idx3-ubyte");
    }
    else
    {
        unsigned char NUM_OF_IMAGES_TMP;
        fread(&MAGIC_NUMBER, sizeof(char32_t), 1, fp);


        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES = NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP;


        fread(&NUM_OF_ROWS, sizeof(char32_t), 1, fp);
        fread(&NUM_OF_COLS, sizeof(char32_t), 1, fp);
        unsigned char tmp;
        for (int i = 0; i < NUM_OF_IMAGES * 28 * 28 / TEST_LIUKE1; i++)
        {//6w张图片，读入像素后归一化到[0,1]
            fread(&tmp, sizeof(char), 1, fp);
            Train_Data.push_back((double)tmp );
        }
    }
  
}

void readImage::read_data_test_image()
{
    char32_t MAGIC_NUMBER = 0;
    char32_t NUM_OF_IMAGES = 0;
    char32_t NUM_OF_ROWS = 0;
    char32_t NUM_OF_COLS = 0;
    FILE* fp = fopen(test_images_idx3_ubyte_file, "rb");
    if (fp == NULL)
    {
        printf("unable open train-images.idx3-ubyte");
    }
    else
    {
        unsigned char NUM_OF_IMAGES_TMP;
        fread(&MAGIC_NUMBER, 1, sizeof(char32_t), fp);

        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES = NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP;


        fread(&NUM_OF_ROWS, sizeof(char32_t), 1, fp);
        fread(&NUM_OF_COLS, sizeof(char32_t), 1, fp);

        unsigned char tmp;
        for (int i = 0; i < NUM_OF_IMAGES * 28 * 28 ; i++)
        {/// TEST_LIUKE0?
            fread(&tmp, sizeof(char), 1, fp);
            Test_Data.push_back((double)tmp );
        }
    }
    
}

void readImage::read_data_train_label()
{
    char32_t MAGIC_NUMBER = 0;
    char32_t NUM_OF_IMAGES = 0;
    FILE* fp = fopen(train_labels_idx1_ubyte_file, "rb");
    if (fp == NULL)
    {
        printf("unable open train-images.idx3-ubyte");
    }
    else
    {
        unsigned char NUM_OF_IMAGES_TMP;
        fread(&MAGIC_NUMBER, 1, sizeof(char32_t), fp);

        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES = NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP;

        unsigned char tmp;
        for (int i = 0; i < NUM_OF_IMAGES; i++)
        {
            fread(&tmp, sizeof(char), 1, fp);
            for (int i = 0; i < 10; i++)
                if (tmp == i)
                    Train_Label.push_back(1);
                else
                    Train_Label.push_back(0);
        }
    }
   
}

void readImage::read_data_test_label()
{
    char32_t MAGIC_NUMBER = 0;
    char32_t NUM_OF_IMAGES = 0;
    FILE* fp = fopen(test_labels_idx1_ubyte_file, "rb");
    if (fp == NULL)
    {
        printf("unable open test-images.idx3-ubyte");
    }
    else
    {
        unsigned char NUM_OF_IMAGES_TMP;
        fread(&MAGIC_NUMBER, 1, sizeof(char32_t), fp);
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES = NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP * 256;
        fread(&NUM_OF_IMAGES_TMP, sizeof(char), 1, fp);
        NUM_OF_IMAGES += NUM_OF_IMAGES_TMP;

        unsigned char tmp;
        for (int i = 0; i < NUM_OF_IMAGES; i++)
        {
            fread(&tmp, sizeof(char), 1, fp);

            for (int i = 0; i < 10; i++)
            {//采用独热码的方式
                if (i == tmp)
                    Test_Label.push_back(1);
                else
                    Test_Label.push_back(0);
            }
        }
    }
   
}
void readImage::print_readMINIST()
{
    //测试一下读入是否正确：打印第一张图片；
    cout << "trainng picture" << endl;
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {//fabs(x)<=1e-6
            cout << (fabs(Train_Data[i * 28 + j]) <= 1e-5 ? " " : "*");
        }
        cout << endl;
    }
    cout << "traing label:\n";
    for (int i = 0; i < 10; i++) {
        cout << Train_Label[i] << " ";
    }
    cout << endl;

    //测试一下读入是否正确：打印第一张图片；
    cout << "test picture" << endl;
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 28; j++) {//fabs(x)<=1e-6
            cout << (fabs(Test_Data[i * 28 + j]) <= 1e-5 ? " " : "*");
        }
        cout << endl;
    }
    cout << "test\n";
    for (int i = 0; i < 10; i++) {
        cout << Test_Label[i] << " ";
    }
    cout << endl;
}