#include"MyJson.h"

Json::Value MyJson::stoj(const string json_text)
{
    Json::Reader reader;
    Json::Value root;

    // 把json_text 存入root
    if (!reader.parse(json_text, root)) {
		cerr << "parse fail! " << endl;
        return root;
	}

    return root;
}


char * MyJson::jtos(Json::Value json_data,int &len)
{
    Json::FastWriter writer;
    string json_text = writer.write(json_data);
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