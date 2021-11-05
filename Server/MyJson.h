#include"Json/json.h"
#include<string.h>
#include<iostream>
#include<cstring>
#pragma once
using namespace std;
class MyJson
{
public:
    Json::Value stoj(const string json_text);

    char * jtos(Json::Value json_data,int &len);

    // 封装发送的json字符串
    char* sendPack(string user,Json::Value data_msg,int & len);
};