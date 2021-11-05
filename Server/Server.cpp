#include"Server.h"

int socketfd;
void SignalFunc(int signo)//自定义信号处理函数
{
	if(signo == SIGINT)
	{
		close(socketfd);
        exit(0);
	}
}

Server::Server(int port)
{	
	signal(SIGINT,SignalFunc);		// 修改终止信号
	memset(&this->recv_buff,0,sizeof(this->recv_buff));
	memset(&this->send_buff,0,sizeof(this->send_buff));
	int ret;
	socketfd = this->sockfd;
	this->sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	int opt = 1;
	// sockfd为需要端口复用的套接字
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
	struct sockaddr_in sockaddr;
	memset(&sockaddr,0,sizeof(sockaddr));
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = INADDR_ANY;
	sockaddr.sin_family = AF_INET;
	
	// bind
	ret = bind(this->sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr));
	if(-1 == ret)
	{
		perror("bind");	
		return;
	}
	
	// listen
	ret = listen(this->sockfd,128);
	if(-1 == ret)
	{
		perror("listen");
		return;
	}
	cout << "服务器启动！"<<endl;	

}


Server::~Server()
{
	close(this->sockfd);
}

int Server::accept_one()
{
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	int connect = accept(this->sockfd,(struct sockaddr*)&clientaddr,&len);
	char  buff[16];
	if(connect == -1)
	{
		perror("connect");
		return -1;
	}
	cout << "new connect!\nid:"<<connect-3<<"\tip:"<<inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,buff,16)<<"\tport:"<<ntohs(clientaddr.sin_port)<<endl;
	return connect;
}
void Server::send_to_client(char * str,int connect,int len)
{
	setSendMsg(str,len);
	int ret = send(connect,this->send_buff,len,0);
	cout << "client <- "<<send_buff <<endl;
	memset(&this->send_buff,0,sizeof(this->send_buff));
}

int Server::recv_from_client(int connect,int len)
{
	memset(&this->recv_buff,0,sizeof(this->recv_buff));
	int ret = recv(connect,this->recv_buff,len,0);
	this->recv_num = recv_buff[0] - '0';	  // 接受到的char转数字
	if(ret <= 0)
	{
		cout<<"client：" << connect-3<< "->disconnect!"<<endl;
		return -1;	
	}
	
	cout <<"client -> " <<recv_buff <<endl;
	return 0;
}


void Server::setSendMsg(char * str,int len)
{
	for(int i = 0;i<len;i++)
	{
		this->send_buff[i] = str[i];
	}	
}


const string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return buf;
}

const string currentTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%H:%M", &tstruct);
    return buf;
}



void Server::select_one()
{
	int maxfd = this->sockfd;	// 最大描述符数量
	FD_ZERO(&oldset);
	FD_ZERO(&rset);
	FD_SET(this->sockfd,&oldset);	//SOCKFD放入监测集合

	int flag = 0;    // 判断是否有用户处于等待状态  1 表示等待
	int	waitfd;		// 等待用户的fd
	int	num = 0;	// 该用户的棋盘号
	while(1)
	{
		
		rset = oldset;
		int n = select(maxfd+1,&rset,NULL,NULL,NULL);	// rset中未发生变化就会被剔除 返回发生变化的数量
		if(n<0)
		{
			perror("select");
			return;
		}
		if(n==0)
		{
			continue;
		}
		else
		{
			if(FD_ISSET(this->sockfd,&rset))	// 检测到连接请求
			{
				int connect = this->accept_one();
				if(-1 == connect)
				{
					return;
				}

				FD_SET(connect,&oldset);
				if(connect > maxfd)
				{
					maxfd = connect;
				}	
			}

			else for(int i = this->sockfd +1;i<=maxfd;i++)
			{
				if(FD_ISSET(i,&rset))
				{
					int ret = this->recv_from_client(i,1024);
					if(-1 == ret)
					{
						close(i);
						FD_CLR(i,&oldset);
						continue;
					}
					else
					{
						// 检测到第i个客户端出现写数据
						processMsg(i);  // 对客户端处理
						// 此处填写客户端发送的信息的处理函数！
					}
				}
			}	
		}
			
	}
}


void Server::processMsg(int connect)
{
	// HrsF>**Pl3,G
    // 初步准备接受数据格式
    // {"user":"xxxx","data":{"command":"xxxx"}}  根据command来区分json字符串
	const string recv_buff_data = this->recv_buff;
	Json::Value json_data = myjson.stoj(recv_buff_data);

	string user = json_data["user"].asString();  // 用户名
	string command = json_data["data"]["command"].asString(); // 命令

	//登录  格式{"user":"xxxx","data":{"command":"login","name":"xxx","password":"xxxx"}}
	if(command == "login")
	{
		string name = json_data["data"]["name"].asString();
		string password = json_data["data"]["password"].asString();
		int ret = db.login(name.data(),password.data());
		if(0 == ret) // 成功
		{
			sendSuccessfulStatus(connect,user,SUCCESSFUL);
		}
		else if(1 == ret) // 密码错误
		{
			sendSuccessfulStatus(connect,user,FAILED);
		}
		else if(-1 == ret) // 不存在
		{
			sendSuccessfulStatus(connect,user,ERROR);
		}
	}

	// 格式{"user":"NULL","data":{"command":"register","name":"xxx","password":"xxxx"}}
	else if(command == "register")
	{
		string name = json_data["data"]["name"].asString();
		string password = json_data["data"]["password"].asString();
		if(db.registered(name.data(),password.data())) // 注册成功
		{
			sendSuccessfulStatus(connect,user,SUCCESSFUL);
		}
		else											// 用户已存在
		{
			sendSuccessfulStatus(connect,user,FAILED);
		}
	}
	// {"user":"xxxx","data":{"command":"append","subject":"xxxx","total":xxxx,"current_process":xxx}}
	else if(command == "append")  // 添加课程 
	{
		const string subject = json_data["data"]["subject"].asString();
		const int total = json_data["data"]["total"].asInt();
		const int current_process = json_data["data"]["current_process"].asInt();

		// cout << subject<<" "<<total<<" "<<current_process<<endl;
		db.append(user.data(),subject.data(),total,current_process);
		sendSuccessfulStatus(connect,user,SUCCESSFUL);
	}

	else if(command == "showall")
	{
		Json::Value all;
		int len;
		all = db.showall(user.data(),len);
		
		if(all)
		{
			cout << "len = "<<len<<endl;
			cout << MyJson().jtos(all,len)<<endl;
			char * send_str = myjson.jtos(all,len);
			send_to_client(send_str,connect,len);
		}
		else
		{
			sendSuccessfulStatus(connect,user,FAILED);
		}
	}
}




// 格式{"user":"XXX","data":{"isSuccessful":"xxx","code":"xxx"}
void Server::sendSuccessfulStatus(int connect,string user,char* flag)
{
	Json::Value data_msg;
	if(flag == SUCCESSFUL)
	{
		data_msg["isSuccessful"] = "true";
		data_msg["msg"] = SUCCESSFUL;
	}
	else
	{
		data_msg["isSuccessful"] = "false";
		if(flag == FAILED)
		{
			data_msg["msg"] = FAILED;
		}
		else
		{
			data_msg["msg"] = ERROR;
		}
	}
	int len ;
	char * send_msg = myjson.sendPack(user,data_msg,len);
	this->send_to_client(send_msg, connect,len);
}