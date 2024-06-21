#include "deletepage.h"
#include "ui_deletepage.h"

deletepage::deletepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deletepage)
{
    ui->setupUi(this);
    ui->comboBox->addItem("None");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");
        if (qry.exec()) {
            if (qry.next()) {
                QString usename = qry.value(0).toString();
                qry.prepare("SELECT TASK FROM Task where username='"+usename+"'");

                if (qry.exec()) {
                    while (qry.next()) {
                        QString value = qry.value(0).toString();
                        ui->comboBox->addItem(value);
                    }
                } else {
                    qDebug() << "Error executing query: " << qry.lastError().text();
                }
            }
        } else {
            qDebug() << "Error executing query: " << qry.lastError().text();
        }

        db.close();
    } else {
        qDebug() << "Failed to open database: " << db.lastError().text();
    }
}

deletepage::~deletepage()
{
    delete ui;
}

void deletepage::on_pushButton_2_clicked()
{
    setVisible(false);
}


void deletepage::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");
    int result = QMessageBox::question(this, "Confirmation", "Are you sure you want to delete?", QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        if (db.open()) {
            QSqlQuery qry;
            qry.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

            if (qry.exec() && qry.next()) {
                QString username = qry.value(0).toString();
                QString taskName = ui->comboBox->currentText();

                QSqlQuery deleteQuery;
                deleteQuery.prepare("DELETE FROM Task WHERE TASK = :taskName AND USERNAME = :username");
                deleteQuery.bindValue(":taskName", taskName);
                deleteQuery.bindValue(":username", username);

                if (deleteQuery.exec()) {
                    QMessageBox::information(this, "Delete", "Deleted");
                } else {
                    qDebug() << "Error deleting from Task table:" << deleteQuery.lastError().text();
                    QMessageBox::information(this, "Error", "Error Deleting");
                }
            } else {
                qDebug() << "No valid record found in the SELECT query";
            }
            db.close();
        } else {
            qDebug() << "Failed to open database:" << db.lastError().text();
        }
    } else if (result == QMessageBox::No) {
        ui->comboBox->setCurrentIndex(-1);
    }
}

