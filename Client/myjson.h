#ifndef MYJSON_H
#define MYJSON_H
#include"json.h"
#include<iostream>
using namespace std;
#include <QObject>

class MyJson : public QObject
{
    Q_OBJECT
public:
    explicit MyJson(QObject *parent = nullptr);

    // 从string 到 Json
    Json::Value stoj(const string json_text);

    char * jtos(Json::Value data,int & len);

    void processRecv(char * recv_msg);

    // 封装发送的json字符串
    char* sendPack(string user,Json::Value data_msg,int & len);   // len函数是为了得到这个包装好的字符串的大小 不然很麻烦
signals:

};

#endif // MYJSON_H
