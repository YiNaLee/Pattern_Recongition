#pragma once
#include <vector>
#include <iostream>
using namespace std;
#define TEST_LIUKE0 10
#define TEST_LIUKE1 1

class readImage {
public:
    vector<double>Train_Data;
    vector<double>Train_Label;
    vector<double>Test_Data;
    vector<double>Test_Label;
    void read_data_train_image();
    void read_data_test_image();
    void read_data_train_label();
    void read_data_test_label();
    void print_readMINIST();
};
