#include "requestpage.h"
#include "qmessagebox.h"
#include "ui_requestpage.h"
requestpage::requestpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::requestpage)
{
    ui->setupUi(this);
    ui->comboBox->addItem("None");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QString username;
        QSqlQuery query;

        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");
        if (query.exec() && query.next()) {
            username = query.value(0).toString();

            query.prepare("SELECT STATUS, COMPANY, POSITION FROM Login WHERE USERNAME='" + username + "'");
            if (query.exec() && query.next()) {
                QString status = query.value(0).toString();
                QString company = query.value(1).toString();
                QString position = query.value(2).toString();

                if (status.toLower() == "personal") {
                    query.prepare("SELECT USERNAME FROM Login WHERE STATUS='" + status + "' AND USERNAME NOT IN ('" + username + "')");
                    if (query.exec()) {
                        while (query.next()) {
                            QString name = query.value(0).toString();
                            ui->comboBox->addItem(name);
                        }
                    } else {
                        qDebug() << "Error executing innermost query: " << query.lastError().text();
                    }
                } else if (status.toLower() == "work") {
                    query.prepare("SELECT NAME, POSITION FROM Login WHERE STATUS='" + status + "' AND COMPANY='" + company + "' AND USERNAME NOT IN ('" + username + "')");
                    if (query.exec()) {
                        while (query.next()) {
                            QString name = query.value(0).toString();
                            QString position = query.value(1).toString();
                            QString combine = name + '-' + position;
                            ui->comboBox->addItem(combine);
                        }
                    } else {
                        qDebug() << "Error executing innermost query: " << query.lastError().text();
                    }
                }
            } else {
                qDebug() << "Error executing second query: " << query.lastError().text();
            }
        } else {
            qDebug() << "Error executing first query: " << query.lastError().text();
        }

        db.close();
    }
}

requestpage::~requestpage()
{
    delete ui;
}

void requestpage::on_pushButton_2_clicked()
{
    setVisible(false);
}

void requestpage::on_pushButton_3_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");
    if (db.open()) {
        QString username;

        QSqlQuery outerQuery;
        outerQuery.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

        if (outerQuery.exec() && outerQuery.next()) {
            // Assign the value to the outer 'username' variable
            username = outerQuery.value(0).toString();
            QSqlQuery qry;

            qry.prepare("SELECT STATUS FROM Login WHERE USERNAME = :username");
            qry.bindValue(":username", username);

            if(qry.exec() && qry.next()){
                QString status=qry.value(0).toString();
                QString selectedName = ui->comboBox->currentText();

                qry.prepare("SELECT STIME, ETIME, TASK FROM Task WHERE USERNAME = :username");
                qry.bindValue(":username", selectedName);

                if(qry.exec()) {
                    ui->listWidget->clear(); // Clear previous items in the listWidget

                    while (qry.next()) {
                        QString stime=qry.value(0).toString();
                        QString etime=qry.value(1).toString();
                        QString task=qry.value(2).toString();
                        QString info=stime+"-"+etime+": "+task;
                        ui->listWidget->addItem(info);
                    }
                }
            }
        }
        db.close();
        QFont font = ui->listWidget->font();
        font.setPointSize(16); // Adjust the size as needed
        ui->listWidget->setFont(font);
    }
    else {
        qDebug() << "Database could not open:" << db.lastError().text();
    }
}


void requestpage::on_pushButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QString username;
        QSqlQuery query;

        // Selecting username from Loginstatus
        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");
        if (query.exec()) {
            if (query.next()) {
                username = query.value(0).toString();
                QString usedname = ui->comboBox->currentText();
                QString Task = ui->lineEdit->text();
                QString stime = ui->timeEdit->text();
                QString etime = ui->timeEdit_2->text();

                // Inserting data into Notification table
                query.prepare("INSERT INTO Notification(fromwhom, towhom, stime, etime, Task) VALUES(:fromwhom, :towhom, :stime, :etime, :task)");
                query.bindValue(":fromwhom", username);
                query.bindValue(":towhom", usedname);
                query.bindValue(":stime", stime);
                query.bindValue(":etime", etime);
                query.bindValue(":task", Task);

                if (query.exec()) {
                    QMessageBox::information(this, "Saved", "Saved");
                } else {
                    qDebug() << "Error inserting into Notification table:" << query.lastError().text();
                    QMessageBox::information(this, "Error", "Error inserting into Notification table");
                }

            } else {
                qDebug() << "No records found in Loginstatus for the given condition.";
                QMessageBox::information(this, "Error", "No records found in Loginstatus for the given condition.");
            }

            db.close();
        } else {
            qDebug() << "Error executing SELECT query:" << query.lastError().text();
            QMessageBox::information(this, "Error", "Error executing SELECT query");
        }
    } else {
        qDebug() << "Unable to Open Database:" << db.lastError().text();
        QMessageBox::information(this, "Error", "Unable to open database");
    }
}

