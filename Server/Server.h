#pragma once
#include<iostream>
using namespace std;
#include<sys/socket.h>
#include<sys/unistd.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/signal.h>
#include<sys/select.h>
#include<errno.h>
#include<string.h>
#include<fstream>
#include "MyJson.h"
#include "DataBase.h"
#define SUCCESSFUL "successful!"
#define FAILED "failed!"
#define ERROR "have error"
class Server
{
public:
	Server(int port);      // 服务器的初始化操作  端口

	~Server();

	int accept_one();    // 接受一个链接的函数

	void send_to_client(char *str,int connect, int len);   // 发送消息

	int recv_from_client( int connect, int len);   // 接收消息 放入buff

	void setSendMsg(char *str,int len ); 	// 设置发送数据

	void select_one();   // select 服务器

    void processMsg(int connect);   // 对connect发来的msg进行处理

	void sendSuccessfulStatus(int connect, string user,char* flag);  // 发送 简单的判断成功和失败的json字符串

	
	int sockfd;
	char send_buff[2048];	// 要发送的消息
	char recv_buff[2048];	// 接收到的信息
    int recv_num;
	fd_set oldset,rset;	// 检测的fd集合

	MyJson myjson;   // 用来处理json字符串
	DataBase db;	// 数据库操作
};

