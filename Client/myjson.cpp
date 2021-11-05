#include "myjson.h"

MyJson::MyJson(QObject *parent) : QObject(parent)
{

}

Json::Value MyJson::stoj(const string json_text)
{
    Json::Reader reader;
    Json::Value root;

    // 判断是否成功
    if (!reader.parse(json_text, root))
    {
            cout << "parse fail! " << endl;
            return NULL;
    }
    return root;
}

char * MyJson::jtos(Json::Value data,int & len)
{
    Json::FastWriter writer;
    string json_text = writer.write(data);
    len = json_text.size();
    char *c = new char[len];
    strcpy(c,json_text.c_str());
    return c;
}

char* MyJson::sendPack(string user,Json::Value data_msg,int & len)
{
    Json::Value json_send_to_client;
    json_send_to_client["user"] = Json::Value(user);
    json_send_to_client["data"] = data_msg;
    return jtos(json_send_to_client,len);
}

