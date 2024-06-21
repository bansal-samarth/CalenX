#ifndef RESPONSE_H
#define RESPONSE_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class response;
}

class response : public QDialog
{
    Q_OBJECT

public:
    explicit response(QWidget *parent = nullptr);
    ~response();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::response *ui;
};

#endif // RESPONSE_H
