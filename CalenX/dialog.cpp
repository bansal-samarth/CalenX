#include "dialog.h"
#include "ui_dialog.h"
#include "relogin.h"
#include "mainwindow.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    if(!conn.Connopen()){
        qDebug()<<"Failed Connection";
    }
    else{
        qDebug()<<"Connection Successful";
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    Relogin rl;
    rl.setModal(true);
    setVisible(false);
    rl.exec();
}


void Dialog::on_pushButton_2_clicked()
{
    QString username,name,email,password,confirm,comp,fro,to;
    QStringList user;
    QStringList emailcol;
    name=ui->Name->text().toLower();
    username=ui->username->text().toLower();
    email=ui->email->text().toLower();
    password=ui->password->text();
    confirm=ui->confirm_password->text();
    comp=ui->company->text().toLower();
    fro=ui->from->text();
    to=ui->to->text();
    if(password==confirm){

        if(!conn.Connopen()){
            qDebug()<<"Failed To Connect";
        }
        conn.Connopen();
        QSqlQuery qry;
        if(qry.exec("select USERNAME from LOGIN ")){
            while(qry.next()){
                user.append(qry.value(0).toString());
            }
        }
        else{
            qDebug() << "Error executing query: " << qry.lastError().text();
        }
        if(user.contains(username)){
            QMessageBox::critical(this,"Incorrect","Username already exists");
        }
        else{
            if (ui->company->isEnabled()) {
                QSqlQuery qry;
                qry.prepare("INSERT INTO Login (NAME, USERNAME, EMAIL, PASSWORD, STATUS, COMPANY, FROMTIME, TOTIME, POSITION) "
                            "VALUES (:name, :username, :email, :password, 'Work', :comp, :fro, :to, :role)");
                qry.bindValue(":name", name); // Replace with actual values
                qry.bindValue(":username", username); // Replace with actual values
                qry.bindValue(":email", email); // Replace with actual values
                qry.bindValue(":password", password); // Replace with actual values
                qry.bindValue(":comp", comp); // Replace with actual values
                qry.bindValue(":fro", fro); // Replace with actual values
                qry.bindValue(":to", to); // Replace with actual values

                if (ui->employee->isChecked()) {
                    qry.bindValue(":role", "Employee");
                } else if (ui->Manager->isChecked()) {
                    qry.bindValue(":role", "Manager");
                }

                if (qry.exec()) {
                    QMessageBox::information(this, "Saved", "Saved");
                    conn.Connclose();
                } else {
                    QMessageBox::information(this, "Error", "Error: " + qry.lastError().text());
                }
            } else {
                QSqlQuery qry;
                qry.prepare("INSERT INTO Login (NAME, USERNAME, EMAIL, PASSWORD, STATUS, COMPANY, FROMTIME, TOTIME, POSITION) "
                            "VALUES (:name, :username, :email, :password, 'Personal', 'NIL', '00:00', '00:00', 'NIL')");
                qry.bindValue(":name", name); // Replace with actual values
                qry.bindValue(":username", username); // Replace with actual values
                qry.bindValue(":email", email); // Replace with actual values
                qry.bindValue(":password", password); // Replace with actual values

                if (qry.exec()) {
                    QMessageBox::information(this, "Saved", "Saved");
                    conn.Connclose();
                } else {
                    QMessageBox::information(this, "Error", "Error: " + qry.lastError().text());
                }
            }

            }
        }
    else{
            QMessageBox::critical(this,"Incorrect","confirmed password and password are not matching");
    }
        conn.close();
}


void Dialog::on_pushButton_3_clicked()
{
        ui->company->setEnabled(true);
        ui->from->setEnabled(true);
        ui->to->setEnabled(true);
        ui->Manager->setEnabled(true);
        ui->employee->setEnabled(true);
}


void Dialog::on_pushButton_4_clicked()
{
    ui->company->setEnabled(false);
    ui->from->setEnabled(false);
    ui->to->setEnabled(false);
    ui->Manager->setEnabled(false);
    ui->employee->setEnabled(false);
}

