#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(400,300);
    this->setWindowTitle("登录");
    connect(ui->login_button,&QPushButton::clicked,this,&LoginWindow::login);
    connect(ui->register_button,&QPushButton::clicked,this,&LoginWindow::registered);
    ui->password_text->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


bool LoginWindow::login()
{
    // 编辑发送的json字符串 并发送
    Json::Value send_text;
    send_text["command"] = "login";
    send_text["name"] = ui->user_text->text().toStdString();
    send_text["password"] = ui->password_text->text().toStdString();
    int len;
    char * text = MyJson().sendPack("NULL",send_text,len);
    client.send_to_server(text,len);
    // 接收验证消息
    client.recv_from_server(1024);
    Json::Value recv_json = MyJson().stoj(client.recv_buff);
    if(recv_json["data"]["msg"] == "successful!")
    {
        MainWindow * mainwindow = new MainWindow;
        mainwindow->show();
        close();
        return true;
    }
    else if(recv_json["data"]["msg"] == "failed!")  // 密码错误
    {
        QMessageBox::information(this,"错误","密码错误");
        return false;
    }

    else if(recv_json["data"]["msg"] == "have error")  // 用户不存在
    {
        QMessageBox::information(this,"错误","用户不存在");
        QMessageBox::StandardButton reply = QMessageBox::question(this,"注册","是否进行注册?",QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            registered();
        }

        return false;
    }

}

bool LoginWindow::registered()
{
    // 编辑发送的json字符串 并发送
    Json::Value send_text;
    send_text["command"] = "register";
    send_text["name"] = ui->user_text->text().toStdString();
    send_text["password"] = ui->password_text->text().toStdString();
    // 判断是否为空不起作用  为解决@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    if(ui->user_text->text().isEmpty() || ui->password_text->text().isEmpty())
    {
        cout << "??";
        QMessageBox::warning(this,"提醒","请输入账号和密码!");
        return false;
    }
    int len;
    char * text = MyJson().sendPack("NULL",send_text,len);
    client.send_to_server(text,len);
    // 接收验证消息
    client.recv_from_server(1024);
    Json::Value recv_json = MyJson().stoj(client.recv_buff);
    if(recv_json["data"]["msg"] == "successful!")
    {
        QMessageBox::information(this,"注册","注册成功!");
        return true;
    }
    if(recv_json["data"]["msg"] == "failed!")
    {
        QMessageBox::warning(this,"注册","注册失败!用户已存在");
        return false;
    }
}
