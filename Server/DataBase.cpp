#include "DataBase.h"

DataBase::DataBase()
{
    // 初始化数据库指针
    conn_ptr = mysql_init(NULL);

    // 连接数据库
    conn_ptr = mysql_real_connect(conn_ptr, "localhost", "root", "Liuzhuohao123.","user", 0, NULL, 0);
    if (conn_ptr) {
       cout << "Connection successful!"<<endl;
    } else {
        cout << "Connection failed!"<<endl;
    }
}

DataBase::~DataBase()
{
    mysql_close(conn_ptr);
}

// 用户是否存在 
MYSQL_ROW DataBase::isExist(const char * name)
{
    char query_str[100];
    sprintf(query_str, "SELECT * from user_pwd WHERE name='%s'", name);    // c++格式化操作
    mysql_query(conn_ptr,query_str);
    // 先用store存储结果，用fetch来获取，若为真则存在
    MYSQL_RES* res_ptr = mysql_store_result(conn_ptr); 
    MYSQL_ROW row = mysql_fetch_row(res_ptr);
    mysql_free_result(res_ptr);
    return row;
}


// 注册操作
bool DataBase::registered(const char * name,const char * password)
{
    // 用户存在
    if(isExist(name))
    {
        cout <<"用户存在，注册失败"<<endl;
        return false;
    }
    // insert 数据
    else
    {
        char  registered_str[100];
        sprintf(registered_str,  "INSERT INTO user_pwd "
                    "(name,password) "
                    "VALUES"
                    "('%s','%s')",name,password);
        mysql_query(conn_ptr,registered_str);
        return true;
    }
}


// 登录账户
int DataBase::login(const char * name,const char * password)
{
    MYSQL_ROW row = isExist(name);
    if(row)
    {
        char * password_in_database = row[1];
        if(0 == strcmp(password,password_in_database))    // char* 比较 若相等返回0
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
}



bool DataBase::append(const char * name,const char * subject,const int total,const int current_process)  // 添加课程
{
    char qurey_str[100];
    sprintf(qurey_str,"create table if not exists %s  like user_model",name);   // 如果不存在表 就创建
    //cout << qurey_str<<endl;
    mysql_query(conn_ptr,qurey_str);

    // 插入数据
    char append_str[120];
    
    sprintf(append_str,"INSERT INTO %s (user_name,subject,total,current_process) VALUE ('%s','%s',%d,%d)",name,name,subject,total,current_process);
    
    //cout << append_str<<endl;
    mysql_query(conn_ptr,append_str);
    return true;
}


Json::Value DataBase::showall(const char * name,int & lens)
{
    Json::Value  all;
    if(isExist(name))
    {
        char query_str[100];
        sprintf(query_str, "SELECT * from %s", name);    // c++格式化操作
        mysql_query(conn_ptr,query_str);
        // 先用store存储结果，用fetch来获取，若为真则存在
        MYSQL_RES* res_ptr = mysql_store_result(conn_ptr); 
        MYSQL_ROW row;
        int row_count = mysql_num_rows(res_ptr);
        // 每次调用fetch_row 按顺序取下一个数组

        // 总json
        Json::Value all;
        int all_len = 0;
        while(row = mysql_fetch_row(res_ptr))
        {
            Json::Value one;
            one["subject"] = row[1];
            one["total"] = row[2];
            one["current_process"] = row[3];
            int len;
            one = MyJson().stoj(MyJson().sendPack(name,one,len));
            all_len += len;
            all.append(one);
        }
        
        mysql_free_result(res_ptr);
        lens = all_len;
        cout << "函数内部"<<MyJson().jtos(all,all_len)<<endl;
        return all;
    }
    else
    {
        return NULL;
    }
}