#include "mywidget.h"
#include "ui_mywidget.h"
#include <QThread>
#include <QDebug>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    myTimer=new QTimer(this);

    connect(myTimer,&QTimer::timeout,this,&MyWidget::dealTimeout);

    //分配线程空间
    thread=new MyThread(this);

    connect(thread,&MyThread::isDone,this,&MyWidget::dealDone);

    //窗口右上角关闭按钮会触发&MyWidget::destroyed
    //关闭线程
    connect(this,&MyWidget::destroyed,this,&MyWidget::stopThread);

}

void MyWidget::stopThread()
{
    //停止线程
    thread->quit();
    //等待线程处理完所有手头工作
    thread->wait();
}

void MyWidget::dealDone()
{
    qDebug()<<"it is over!";
    myTimer->stop();
}

void MyWidget::dealTimeout()
{
    static int i=0;
    i++;
    //设置lcd的值
    ui->lcdNumber->display(i);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    //如果定时器没有工作
    if(myTimer->isActive()==false)
    {
        myTimer->start(100);
    }

//    //非常复杂的数据处理，耗时较长
//    QThread::sleep(5);

//    //处理完数据后，关闭定时器
//    //myTimer->stop();
//    qDebug()<<"over";

    //启动线程，处理数据
    thread->start();




}
