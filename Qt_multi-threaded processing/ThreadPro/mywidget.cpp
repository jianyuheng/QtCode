#include "mywidget.h"
#include "ui_mywidget.h"
#include <QThread>
#include <QDebug>


MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    //动态分配空间，注意：不能指定父对象
    myT=new MyThread;

    //创建子进程
    thread=new QThread(this);

    //把自定义的线程加入到子线程中
    myT->moveToThread(thread);

    connect(myT,&MyThread::mySignal,this,&MyWidget::dealSignal);

    qDebug()<<"主线程号："<<QThread::currentThread();
    connect(this,&MyWidget::startThread,myT,&MyThread::myTimeout,Qt::QueuedConnection);

    connect(this,&MyWidget::destroyed,this,&MyWidget::dealClose);

    //线程处理函数内部，纯数据处理，不允许操作图形界面

    //connect()第五个参数的作用，连接方式，默认，队列，直接
    //默认的时候使用队列
    //多线程，默认使用队列
    //单线程，默认使用直接
    //队列：槽函数所在的线程和接受者一样
    //直接方式：槽函数和发送者所在的线程一样
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::dealSignal()
{
    static int i=0;
    i++;
    ui->lcdNumber->display(i);
}

void MyWidget::dealClose()
{
    on_buttonStop_clicked();
    delete myT;
}

void MyWidget::on_buttonStart_clicked()
{
    //启动线程，但是没有启动线程处理函数
    if(thread->isRunning()==true)
    {
        return;
    }
    thread->start();
    myT->setFlag(false);

    //不能直接调用线程处理函数，直接调用
    //导致，线程处理函数和主线程是在同一个线程

   // myT->myTimeout();
    //通过signal-slot方式调用
    emit startThread();


}

void MyWidget::on_buttonStop_clicked()
{
    if(thread->isRunning()==false)
    {
        return;
    }
    myT->setFlag(true);
    thread->quit();
    thread->wait();
}
