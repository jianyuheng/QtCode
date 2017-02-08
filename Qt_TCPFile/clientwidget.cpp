#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    setWindowTitle("客户端");

    //进度条
    ui->progressBar->setValue(0);

    tcpSocket=new QTcpSocket(this);
    isStart=true;
    connect(tcpSocket,&QTcpSocket::readyRead,
            [=]()
    {
        //取出接收的内容
        QByteArray buf = tcpSocket->readAll();

        if(true==isStart)
        {
            //接受头,isStart标记头部信息
            isStart=false;
            //解析头部信息 QString buf = "hello##1024"
// 字符串拆包    QString str = "hello##1024";
//             str.section("##",0,0)

             //初始化
            fileName=QString(buf).section("##",0,0);
            fileSize=QString(buf).section("##",1,1).toInt();

            recvSize=0;

            //打开文件
            file.setFileName(fileName);

            bool isOk=file.open(QIODevice::WriteOnly);
            if(false==isOk)
            {
                qDebug()<<"WriteOnly error 36";
            }


//            //弹出对话框，显示接受文件的信息
//QUESTION?
//            QString str=QString("接收的文件：[%1:%2kb]").arg(fileName).arg(fileSize/1024);
//            QMessageBox::information(this,"文件信息",str);


            //设置进度条
            ui->progressBar->setMinimum(0);
            ui->progressBar->setMaximum(fileSize/1024);
            ui->progressBar->setValue(0);
        }

        else//文件信息
        {
           qint64 len=file.write(buf);
           if(len>0)//接收数据大于0
           {
               recvSize+=len;
               QString str=QString::number(recvSize);

               tcpSocket->write(str.toUtf8().data());
               //qDebug()<<len;
           }

           //更新进度条
           ui->progressBar->setValue(recvSize/1024);


           if(recvSize==fileSize)//文件接受完毕
           {
               //给服务器发送信息表明客户端已成功接收文件
               tcpSocket->write("file done");
               QMessageBox::information(this,"完成","文件接受完成");
               file.close();


               tcpSocket->disconnectFromHost();
               tcpSocket->close();
           }
        }
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

//连接服务器
void ClientWidget::on_pushButton_clicked()
{
    //获取服务器的ip和端口
    QString ip=ui->lineEditIP->text();
    quint16 port=ui->lineEditPort->text().toInt();

    tcpSocket->connectToHost(QHostAddress(ip),port);

}
