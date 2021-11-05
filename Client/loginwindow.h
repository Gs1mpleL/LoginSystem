#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
//#include "registerwindow.h"
#include "mainwindow.h"
#include <iostream>
using namespace std;
#include <QWidget>
#include "myclient.h"
#include <QMessageBox>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget ,public MyClient
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    bool login();
    bool registered();

private slots:

private:
    Ui::LoginWindow *ui;

};

#endif // LOGINWINDOW_H
