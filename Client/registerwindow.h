#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include "myjson.h"
#include "myclient.h"
#include <QMessageBox>
namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QWidget,public MyClient
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

    bool register_check();

private:
    Ui::RegisterWindow *ui;
};

#endif // REGISTERWINDOW_H
