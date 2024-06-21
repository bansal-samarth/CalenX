#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include<QFileInfo>
#include<QMessageBox>
#include <QCheckBox>
namespace Ui {
class Mainpage;
}

class Mainpage : public QDialog
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
    explicit Mainpage(QWidget *parent = nullptr);
    ~Mainpage();

private slots:
    void on_task_clicked();

    void on_latest_clicked();

    void on_search_2_clicked();

    void on_notification_clicked();

    void on_profile_clicked();

    void on_add_clicked();

    void on_delete_2_clicked();

    void on_Request_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_clearhistory_clicked();

private:
    Ui::Mainpage *ui;
};

#endif // MAINPAGE_H
