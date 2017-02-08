#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTimer>

#include <mythread.h>//线程头文件


namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    void dealTimeout();//定义槽函数
    void dealDone();//定义线程结束槽函数
    void stopThread();//关闭线程

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyWidget *ui;

    QTimer *myTimer;
    MyThread  *thread;//线程对象
};

#endif // MYWIDGET_H
