#include "mainpage.h"
#include "ui_mainpage.h"
#include "addpage.h"
#include "deletepage.h"
#include "requestpage.h"
#include "response.h"
#include "relogin.h"
Mainpage::Mainpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Mainpage)
{
    ui->setupUi(this);
    ui->add->setVisible(false);
    ui->delete_2->setVisible(false);
    ui->Request->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->clearhistory->setVisible(false);
    QSqlQuery qry;
    QString name;
    qry.prepare("Select NAME from LOGIN where USERNAME IN (Select username from Loginstatus order by DATE DESC ,TIME DESC LIMIT 1 )");
    if(qry.exec() && qry.first()){
        name=qry.value(0).toString().toUpper();
    }
    else{
        qDebug()<<"Failed to retrieve the Data";
    }
    ui->label->setText("HELLO, "+name);
    ui->label_2->setText(name+"'s DASHBOARD");

}

Mainpage::~Mainpage()
{
    delete ui;
}
void Mainpage::on_task_clicked()
{
    ui->listWidget->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QString username;

        QSqlQuery query;
        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

        if (query.exec() && query.next()) {
            // Assign the value to the outer 'username' variable
            username = query.value(0).toString();

            query.prepare("SELECT STIME, ETIME, TASK FROM Task WHERE USERNAME='" + username + "' ORDER BY STIME");

            if (query.exec()) {
                while (query.next()) {
                    QString startTime = query.value(0).toString();
                    QString endTime = query.value(1).toString();
                    QString task = query.value(2).toString();

                    QString taskInfo = startTime + " - " + endTime + " : " + task;

                    // Assuming 'listWidget' is the name of your QListWidget
                    QListWidgetItem *item = new QListWidgetItem(taskInfo);
                    ui->listWidget->addItem(item);
                }
            } else {
                qDebug() << "Error executing the second query: " << query.lastError().text();
            }
        } else {
            qDebug() << "Error executing the first query: " << query.lastError().text();
        }

        db.close();

        // Set font size
        QFont font = ui->listWidget->font();
        font.setPointSize(16); // Adjust the size as needed
        ui->listWidget->setFont(font);

        // Add scrollbar
        ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else {
        qDebug() << "Database could not open";
    }


    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->add->setVisible(true);
    ui->delete_2->setVisible(true);
    ui->Request->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->clearhistory->setVisible(false);
}


void Mainpage::on_latest_clicked()
{
    ui->listWidget->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QString username;

        QSqlQuery outerQuery;
        outerQuery.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

        if (outerQuery.exec() && outerQuery.next()) {
            // Assign the value to the outer 'username' variable
            username = outerQuery.value(0).toString();

            // Get the current time in UTC format
            QDateTime currentDateTimeUtc = QDateTime::currentDateTime();
            QString currentDateTimeStringUtc = currentDateTimeUtc.toString("hh:mm");

            QSqlQuery innerQuery;

            // Query to get the latest upcoming task based on the current time
            innerQuery.prepare("SELECT TASK, STIME, ETIME FROM Task WHERE DATETIME(STIME) > DATETIME(:currentDateTime) AND USERNAME='" + username + "' ORDER BY DATETIME(STIME) ASC LIMIT 1");
            innerQuery.bindValue(":currentDateTime", currentDateTimeStringUtc);

            if (innerQuery.exec()) {
                if (innerQuery.next()) {
                    QString task = innerQuery.value(0).toString();
                    QString startTime = innerQuery.value(1).toString();
                    QString endTime = innerQuery.value(2).toString();

                    QString upcomingTask = "Task: " + task +
                                           "\nStart Time: " + startTime +
                                           "\nEnd Time: " + endTime;

                    // Assuming 'listWidget' is the name of your QListWidget
                    QListWidgetItem *item = new QListWidgetItem(upcomingTask);
                    ui->listWidget->addItem(item);
                } else {
                    // No upcoming task found
                    QListWidgetItem *item = new QListWidgetItem("Nothing left for the day.");
                    ui->listWidget->addItem(item);
                    qDebug() << "Query returned no results.";
                }
            } else {
                // Print detailed error information
                qDebug() << "Error executing query:" << innerQuery.lastError().text();
            }

            db.close();
            QFont font = ui->listWidget->font();
            font.setPointSize(16); // Adjust the size as needed
            ui->listWidget->setFont(font);
        } else {
            qDebug() << "Database could not open:" << db.lastError().text();
        }
    }

    // Hide labels and buttons
    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->add->setVisible(false);
    ui->delete_2->setVisible(false);
    ui->Request->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->clearhistory->setVisible(false);
}


void Mainpage::on_search_2_clicked()
{
    ui->listWidget->clear();
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
                    query.prepare("SELECT NAME,USERNAME FROM Login WHERE STATUS='" + status + "' AND USERNAME NOT IN ('" + username + "')");
                    if (query.exec()) {
                        while (query.next()) {
                            QString name = query.value(0).toString()+"-"+query.value(1).toString();
                            QListWidgetItem *item = new QListWidgetItem(name);
                            ui->listWidget->addItem(item);
                        }
                    } else {
                        qDebug() << "Error executing innermost query: " << query.lastError().text();
                    }
                }
                else if(status=="Work"){
                    query.prepare("SELECT NAME, POSITION FROM Login WHERE STATUS='" + status + "' AND COMPANY='" + company + "' AND USERNAME NOT IN ('" + username + "')");
                    if (query.exec()) {
                        while (query.next()) {
                            QString name = query.value(0).toString();
                            QString position = query.value(1).toString();
                            QString combine=name+'-'+position;
                            QListWidgetItem *item = new QListWidgetItem(combine);
                            ui->listWidget->addItem(item);
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
        QFont font = ui->listWidget->font();
        font.setPointSize(16); // Adjust the size as needed
        ui->listWidget->setFont(font);
    } else {
        qDebug() << "Database could not open";
    }

    // Hide labels and buttons
    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->add->setVisible(false);
    ui->delete_2->setVisible(false);
    ui->Request->setVisible(true);
    ui->pushButton->setVisible(false);
    ui->clearhistory->setVisible(false);
}


void Mainpage::on_notification_clicked()
{
    ui->listWidget->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        qDebug() << "Database opened successfully";

        QSqlQuery qry;
        QString username;

        qry.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

        if (qry.exec() && qry.next()) {
            username = qry.value(0).toString();

            // First Query
            qry.prepare("SELECT stime,etime,Task,fromwhom FROM notification where towhom=:username and accrej is NULL");
            qry.bindValue(":username", username);

            if (qry.exec()) {
                while (qry.next()) {
                    QString stime = qry.value(0).toString();
                    QString etime = qry.value(1).toString();
                    QString task = qry.value(2).toString();
                    QString sender = qry.value(3).toString();

                    QString combination = stime + "-" + etime + ": " + task + " by " + sender;
                    ui->listWidget->addItem(combination);
                }
            } else {
                qDebug() << "Error executing the first query: " << qry.lastError().text();
            }

            // Second Query
            qry.prepare("SELECT stime,etime,Task,towhom,accrej FROM notification where fromwhom=:username and accrej is not NULL");
            qry.bindValue(":username", username);

            if (qry.exec()) {
                while (qry.next()) {
                    QString stime2 = qry.value(0).toString();
                    QString etime2 = qry.value(1).toString();
                    QString task2 = qry.value(2).toString();
                    QString recipient = qry.value(3).toString();
                    QString what = qry.value(4).toString();

                    if (what == "A") {
                        QString combination2 = stime2 + "-" + etime2 + ": " + task2 + " has been accepted by " + recipient;

                        // Insert into Task Table
                        QSqlQuery insertQuery;
                        insertQuery.prepare("INSERT INTO Task (TASK, STIME, ETIME, USERNAME) VALUES (:task, :stime, :etime, :username)");
                        insertQuery.bindValue(":task", task2);
                        insertQuery.bindValue(":stime", stime2);
                        insertQuery.bindValue(":etime", etime2);
                        insertQuery.bindValue(":username", username);

                        if (insertQuery.exec()) {
                            qDebug() << "Task inserted successfully";
                            ui->listWidget->addItem(combination2);
                        } else {
                            qDebug() << "Error inserting task: " << insertQuery.lastError().text();
                        }
                    } else if (what == "R") {
                        QString combination2 = stime2 + "-" + etime2 + ": " + task2 + " has been rejected by " + recipient;
                        ui->listWidget->addItem(combination2);
                    } else if (what == "") {
                        QString combination2 = stime2 + "-" + etime2 + ": " + task2 + " no responses yet from " + recipient;
                        ui->listWidget->addItem(combination2);
                    }
                }
            } else {
                qDebug() << "Error executing the second query: " << qry.lastError().text();
            }
        } else {
            qDebug() << "Unable to execute the first query: " << qry.lastError().text();
        }

        // Close the database connection
        db.close();
        QFont font = ui->listWidget->font();
        font.setPointSize(16); // Adjust the size as needed
        ui->listWidget->setFont(font);
    } else {
        qDebug() << "Database not opened. Error: " << db.lastError().text();
    }

    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->add->setVisible(false);
    ui->delete_2->setVisible(false);
    ui->Request->setVisible(false);
    ui->pushButton->setVisible(true);
    ui->clearhistory->setVisible(true);
}


void Mainpage::on_profile_clicked()
{
    ui->listWidget->clear();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");
    if (db.open()) {
        QString username;
        QSqlQuery query;
        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");
        if (query.exec() && query.next()) {
            // Assign the value to the outer 'username' variable
            username = query.value(0).toString();
            query.prepare("SELECT NAME, EMAIL, STATUS, COMPANY, FROMTIME, TOTIME, POSITION FROM LOGIN where USERNAME='" +username+"'");

            if (query.exec()) {
                while (query.next()) {
                    QString name = query.value(0).toString();
                    QString email = query.value(1).toString();
                    QString status = query.value(2).toString();
                    QString company = query.value(3).toString();
                    QString fromTime = query.value(4).toString();
                    QString toTime = query.value(5).toString();
                    QString position = query.value(6).toString();

                    QString employeeInfo = "Name: " + name +
                                           "\nEmail: " + email +
                                           "\nStatus: " + status +
                                           "\nCompany: " + company +
                                           "\nFrom Time: " + fromTime +
                                           "\nTo Time: " + toTime +
                                           "\nPosition: " + position;

                    // Assuming 'listWidget' is the name of your QListWidget
                    QListWidgetItem *item = new QListWidgetItem(employeeInfo);
                    ui->listWidget->addItem(item);
                }
                db.close();

                // Set font size
                QFont font = ui->listWidget->font();
                font.setPointSize(16);
                ui->listWidget->setFont(font);
                ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
                ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
            } else {
                qDebug() << "Error executing the query: " << query.lastError().text();
            }
        }
    }
    else {
        qDebug() << "Database could not open";
    }
    ui->label->setVisible(false);
    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->add->setVisible(false);
    ui->delete_2->setVisible(false);
    ui->Request->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->clearhistory->setVisible(false);
}


void Mainpage::on_add_clicked()
{
    Addpage ap;
    ap.setModal(true);
    ap.exec();
}


void Mainpage::on_delete_2_clicked()
{
    deletepage dp;
    dp.setModal(true);
    dp.exec();
}


void Mainpage::on_Request_clicked()
{
    requestpage rp;
    rp.setModal(true);
    rp.exec();
}

void Mainpage::on_pushButton_clicked()
{
    response r;
    r.setModal(true);
    r.exec();
}

void Mainpage::on_pushButton_2_clicked()
{
    setVisible(false);
    Relogin mw;
    mw.setVisible(true);
    mw.exec();
}


void Mainpage::on_clearhistory_clicked()
{
    int result = QMessageBox::warning(this, "Confirmation", "Are you sure? All your notification data will be lost", QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/VISHNU/calenx.db");

        if (db.open()) {
            QSqlQuery query;
            QString username;

            query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

            if (query.exec() && query.next()) {
                username = query.value(0).toString();

                query.prepare("DELETE FROM Notification WHERE towhom=:username");
                query.bindValue(":username", username);

                if (query.exec()) {
                    QMessageBox::information(this, "Success", "Request History Cleared");
                    ui->listWidget->clear();
                } else {
                    qDebug() << "Deletion did not work. Error: " << query.lastError().text();
                }
            } else {
                qDebug() << "Select did not work. Error: " << query.lastError().text();
            }

            db.close();
        } else {
            qDebug() << "Database did not open. Error: " << db.lastError().text();
        }
    } else {
        setVisible(true);
    }
}
