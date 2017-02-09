#include "widget.h"
#include "ui_widget.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariantList>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //打印Qt支持的数据库驱动
    qDebug()<<QSqlDatabase::drivers();

    //添加MySql数据库
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    //连接数据库
    db.setHostName("127.0.0.1");//数据库服务器IP
    db.setUserName("root");//数据库用户名
    db.setPassword("123456");//密码
    db.setDatabaseName("info");//使用哪个数据库

    //打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,"错误",db.lastError().text());
        return;
    }

    //创建表
//    QSqlQuery query;
//    query.exec("create table student(id int primary key auto increment,name varchar(255),age int,score int)");

    //插入
    QSqlQuery query;
    query.exec("insert into student(id,name,age,score) values(1,'mike',18,90)");

    //批量插入
    //odbc风格
    //预处理语句
    //?相当于占位符
//    query.prepare("insert into student(name,age,score) values(?,?,?)");
    //给字段设置内容 list
//    QVariantList namelist;
//    namelist<<"jack"<<"hahaha"<<"heiheihei";
//    QVariantList agelist;
//    namelist<<11<<22<<33;
//    QVariantList scorelist;
//    namelist<<80<<90<<100;

//    //给字段绑定相应的值
//    query.addBindValue(nameList);
//    query.addBindValue(ageList);
//    query.addBindValue(scoreList);

//    //执行预处理命令
//    query.execBatch();
    //oracle风格
    //占位符 : +自定义的名字
    query.prepare("insert into student(name,age,score) values(:name,:age,:score)");
    //给字段设置内容 list
    QVariantList namelist;
    namelist<<"jack"<<"hahaha"<<"heiheihei";
    QVariantList agelist;
    namelist<<11<<22<<33;
    QVariantList scorelist;
    namelist<<80<<90<<100;
    //给字段绑定相应的值
    query.bindValue(":name",namelist);
    query.bindValue(":score",scorelist);
    query.bindValue(":age",agelist);
    //执行预处理命令
    query.execBatch();

    //数据库的遍历
    query.exec("select * from student");
    while(query.next())//一行一行遍历
    {
        //取出当前行的内容
        qDebug()<<query.value(0).toInt()
               <<query.value(1).toString()
              <<query.value("age").toInt()
             <<query.value("score").toInt();

    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_buttonDel_clicked()
{
    //获取行编辑的内容
    QString name=ui->lineEdit->text();

    QString sql=QString("delete from student where name='%1'").arg(name);
    //开启事务
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.exec(sql);

}

void Widget::on_buttonSure_clicked()
{
    //确定删除
     QSqlDatabase::database().commit();

}

void Widget::on_buttonCancel_clicked()
{
    //回滚事务，撤销
    QSqlDatabase::database().rollback();
}
