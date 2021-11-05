#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>
#include <QDialog>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginWindow login;
    login.show();
    return a.exec();
}
