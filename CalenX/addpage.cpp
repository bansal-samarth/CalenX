#include "addpage.h"
#include "qmessagebox.h"
#include "ui_addpage.h"

Addpage::Addpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Addpage)
{
    ui->setupUi(this);
}

Addpage::~Addpage()
{
    delete ui;
}

void Addpage::on_pushButton_clicked()
{
    int result = QMessageBox::question(this, "Confirmation", "Are you sure you want to save?", QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/VISHNU/calenx.db"); // Set the path to your database file

        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

            if (qry.exec()) {
                if (qry.next()) {
                    QString rest = qry.value(0).toString();
                    QString nm = ui->TaskName->text();
                    QString stime = ui->stimeEdit->text();
                    QString etime = ui->etimeEdit->text();

                    qry.prepare("INSERT INTO Task (TASK, STIME, ETIME, USERNAME) VALUES (:name, :start_time, :end_time, :username)");
                    qry.bindValue(":name", nm);
                    qry.bindValue(":start_time", stime);
                    qry.bindValue(":end_time", etime);
                    qry.bindValue(":username", rest);

                    if (qry.exec()) {
                        QMessageBox::information(this, "Saved", "Saved");
                    } else {
                        qDebug() << "Error inserting into Task table:" << qry.lastError().text();
                        QMessageBox::information(this, "Error", "Error inserting into Task table");
                    }
                } else {
                    qDebug() << "No valid record found in the SELECT query";
                }
            } else {
                qDebug() << "Error executing the SELECT query:" << qry.lastError().text();
            }
            db.close();
        } else {
            qDebug() << "Failed to open database:" << db.lastError().text();
        }
    } else if (result == QMessageBox::No) {
        ui->TaskName->clear();
        ui->stimeEdit->clear();
        ui->etimeEdit->clear();
    }
}
void Addpage::on_pushButton_2_clicked()
{
    setVisible(false);
}
