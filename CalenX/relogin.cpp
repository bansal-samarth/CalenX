#include "relogin.h"
#include "ui_relogin.h"
#include "mainpage.h"
Relogin::Relogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Relogin)
{
    ui->setupUi(this);
}

Relogin::~Relogin()
{
    delete ui;
}

void Relogin::on_pushButton_4_clicked()
{
    QString username,password;
    username=ui->username1->text();
    password=ui->password1->text();
    if(!Connopen()){
        qDebug()<<"Failed to connect";
    }
    Connopen();
    QSqlQuery qry;
    qry.prepare("select * from Login where USERNAME='"+username+"' and password='"+password+"'");
    if(qry.exec()){
        int count=0;
        while(qry.next()){
            count=count+1;
        }
        if(count==1){
            Mainpage mp;
            mp.setModal(true);
            setVisible(false);
            mp.exec();
            Connclose();
        }
        if(count<1){
            QMessageBox::critical(this,"Incorrect","Incorrect Username or Password");
        }
    }
}

