#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QUdpSocket>//UDP套接字


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
    //槽函数，处理对方发送的数据
    void dealMsg();

private slots:
    void on_buttonSend_clicked();

private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
};

#endif // WIDGET_H
