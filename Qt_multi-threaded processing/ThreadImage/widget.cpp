#include "widget.h"
#include "ui_widget.h"
#include <QThread>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    myT=new MyThread;

    //创建子进程
    thread =new QThread(this);

    //把自定义模块添加到子线程
    myT->moveToThread(thread);

    //启动子线程
    thread->start();

    //线程处理函数必须通过signal-slot来调用
    connect(ui->pushButton,&QPushButton::pressed,myT,&MyThread::drawImage);
    connect(myT,&MyThread::updateImage,this,&Widget::getImage);
    connect(this,&Widget::destroyed,this,&Widget::dealClose);
}

void Widget::getImage(QImage temp)
{
    image=temp;
    update();//更新窗口，自动调用paintEvent
}

void Widget::dealClose()
{
    //退出子线程
    thread->quit();

    //回收资源
    thread->wait();

    delete myT;
    myT=NULL;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);//指定画图窗口
    p.drawImage(50,50,image);
}
