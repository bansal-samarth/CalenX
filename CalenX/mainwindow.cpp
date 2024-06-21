#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "mainpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(!Connopen()){
        qDebug()<<"Failed Connection";
    }
    else{
        qDebug()<<"Connection Successful";
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    Dialog dl;
    dl.setModal(true);
    dl.exec();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString username, password, date, time;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    username = ui->username->text();
    password = ui->password->text();
    date = currentDateTime.date().toString();
    time = currentDateTime.time().toString();

    if (!Connopen()) {
        qDebug() << "Failed to connect";
    }

    Connopen();

    QSqlQuery qry;
    qry.prepare("SELECT * FROM Login WHERE USERNAME = :username AND password = :password");
    qry.bindValue(":username", username);
    qry.bindValue(":password", password);

    if (qry.exec()) {
        int count = 0;
        while (qry.next()) {
            count = count + 1;
        }
        if (count == 1) {
            QSqlQuery qry1;
            qry1.prepare("INSERT INTO Loginstatus (username, DATE, TIME) VALUES (:username, :date, :time)");
            qry1.bindValue(":username", username);
            qry1.bindValue(":date", date);
            qry1.bindValue(":time", time);

            if (qry1.exec()) {
                Mainpage mp;
                mp.setModal(true);
                setVisible(false);
                mp.exec();
            } else {
                qDebug()<<"Insertion Failed";
            }

            Connclose();
        }
        if (count < 1) {
            QMessageBox::critical(this, "Incorrect", "Incorrect Username or Password");
        }
    }
}

