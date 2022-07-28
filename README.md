# LoginSystem
基于Linux select并发 mysql数据库服务器  qtGUI设计前台的登录系统
基本实现登录和注册账号的功能

数据传输使用json字符串，增加拓展性


# 启动方法
服务器：
linux 下载server 设置DataBase类内的连接数据库函数为自己的mysql账号
数据库创建user数据库 表头 name password
user中创建user_pwd数据表
输入make  运行

客户端：
qt中编译运行即可

# 第一次独立完成C++网络编程项目，特此纪念！！！
