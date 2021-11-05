#include <mysql/mysql.h>
#include<iostream>
#include<string.h>
#include "MyJson.h"
using namespace std;

class DataBase
{
public:
    DataBase();  // 数据库登录
    ~DataBase(); // 关闭链接

    MYSQL_ROW isExist(const char * name); // 是否存在 
    
    bool registered(const char * name,const char * password); // 注册账户
    
    int login(const char * name,const char * password);  // 登录账户 0 成功 1 密码错误 -1 用户不存在

    bool append(const char * name,const char * subject,const int total,const int current_process);   // 添加课程

    Json::Value showall(const char * name,int & lens);   // 发送用户所有数据

    
private:
    MYSQL *conn_ptr;
};