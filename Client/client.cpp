#include"client.h"
// 重写connect函数
int Connect(int sockfd,struct sockaddr_in sockaddr)
{
    int ret = connect(sockfd,(struct sockaddr *)&sockaddr,sizeof(sockaddr));
    if(-1 == ret)
    {
        cout << "connect failed!"<<endl;
        return -1;
    }
    return 0;
}
Client::Client(QObject *parent) : QObject(parent)
{
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        cout << "error"<<endl;
        return;
    }


    this->sockfd = socket(AF_INET,SOCK_STREAM,0);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(SERVER_PORT);
    sockaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    memset(&send_buff,0,sizeof(send_buff));
    memset(&recv_buff,0,sizeof(recv_buff));
    if(Connect(this->sockfd,this->sockaddr) != -1)
    {
        cout << "connect server!"<<endl;
    }
}
Client::~Client()
{
    closesocket(sockfd);
}



void Client::setSendMsg(char * str,int len)
{
    for(int i = 0;i<len;i++)
    {
        this->send_buff[i] = str[i];
    }
}
void Client::send_to_server(char * str,int len)
{
    setSendMsg(str,len);
    send(this->sockfd,send_buff,len,0);
    memset(&send_buff,0,sizeof(send_buff));
}

void Client::recv_from_server(int len)
{
    memset(&recv_buff,0,sizeof(recv_buff));
    recv(this->sockfd,recv_buff,len,0);
}
