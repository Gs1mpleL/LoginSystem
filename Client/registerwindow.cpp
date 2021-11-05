#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    connect(ui->register_pushButton,&QPushButton::clicked,this,&RegisterWindow::register_check);
    connect(ui->cannel_pushButton,&QPushButton::clicked,this,&RegisterWindow::close);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}


bool RegisterWindow::register_check()
{
    // 编辑发送的json字符串 并发送
    Json::Value _send_text;
    _send_text["command"] = "register";
    _send_text["name"] = ui->user_lineEdit->text().toStdString();
    _send_text["password"] = ui->password_lineEdit->text().toStdString();
    int _len;
    char * _text = MyJson().sendPack("NULL",_send_text,_len);
    client.send_to_server(_text,_len);
    // 接收验证消息
    client.recv_from_server(1024);
    Json::Value _recv_json = MyJson().stoj(client.recv_buff);
    if(_recv_json["data"]["msg"] == "successful!")
    {
        QMessageBox::information(this,"成功","注册成功");
        return true;
    }
    else if(_recv_json["data"]["msg"] == "failed!")
    {
        QMessageBox::information(this,"错误","用户已存在");
        return false;
    }
}
