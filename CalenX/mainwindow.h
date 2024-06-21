#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include<QFileInfo>
#include<QMessageBox>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
signals:
    void takeusename(const QString& user);
    void giveusename(const QString& use);
};
#endif // MAINWINDOW_H
