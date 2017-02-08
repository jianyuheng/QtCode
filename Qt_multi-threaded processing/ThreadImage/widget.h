#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include "mythread.h"
#include <QImage>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    //重写绘图事件
    void paintEvent(QPaintEvent *event);
    void getImage(QImage temp);
    void dealClose();//关闭线程

private:
    Ui::Widget *ui;
    QImage image;
    MyThread *myT;//自定义线程对象
    QThread *thread;//子进程
};

#endif // WIDGET_H
