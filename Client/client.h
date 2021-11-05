#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "Config.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    void send_to_server(char * str, int len);
    void recv_from_server(int len);
    void setSendMsg(char * str,int len);
    char send_buff[2048];
    char recv_buff[2048];
    int sockfd;
    sockaddr_in sockaddr;
signals:

};

#endif // CLIENT_H
