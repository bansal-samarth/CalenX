#ifndef RELOGIN_H
#define RELOGIN_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include<QFileInfo>
#include<QMessageBox>
namespace Ui {
class Relogin;
}

class Relogin : public QDialog
{
    Q_OBJECT
public:
    QSqlDatabase db;
    void Connclose(){
        db.close();
        db.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool Connopen(){
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/VISHNU/calenx.db");
        if(db.open()){
            qDebug()<<"Connected";
            return true;
        }
        else{
            qDebug()<<"Not Connected";
            return false;
        }
    }

public:
    explicit Relogin(QWidget *parent = nullptr);
    ~Relogin();

private slots:
    void on_pushButton_4_clicked();

private:
    Ui::Relogin *ui;
};

#endif // RELOGIN_H
