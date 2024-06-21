#include "response.h"
#include "ui_response.h"

response::response(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::response)
{
    ui->setupUi(this);
    ui->comboBox->addItem("None");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QSqlQuery query;
        QString username;
        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");
        if (query.exec() && query.next()) {
            username = query.value(0).toString();
            query.prepare("SELECT Task,fromwhom FROM Notification where towhom='"+username+"' and accrej is NULL");
            if(query.exec()){
                while(query.next()){
                    QString task=query.value(0).toString();
                    QString frome=query.value(1).toString();
                    QString combo=task+" by "+frome;
                    ui->comboBox->addItem(combo);
                }
            }
        }
    }
    else{
        qDebug()<<"Failed to open database";
    }
}

response::~response()
{
    delete ui;
}

void response::on_pushButton_3_clicked()
{
    QStringList elements;
    QString m = ui->comboBox->currentText();
    elements = m.split(" by ");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/VISHNU/calenx.db");

    if (db.open()) {
        QSqlQuery query;
        QString username;

        query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

        if (query.exec() && query.next()) {
            username = query.value(0).toString();

            query.prepare("SELECT stime, etime FROM Notification WHERE Task = :task AND fromwhom = :fromwhom AND towhom = :username");

            query.bindValue(":task", elements[0]);    // Use elements[0] instead of m[0]
            query.bindValue(":fromwhom", elements[1]); // Use elements[1] instead of m[1]);
            query.bindValue(":username", username);

            if(query.exec() && query.next()){
                QString stime=query.value(0).toString();
                QString etime=query.value(1).toString();

                QTime time1 = QTime::fromString(stime, "hh:mm");
                QTime time2 = QTime::fromString(etime, "hh:mm");

                if (time1.isValid() && time2.isValid()) {
                    ui->timeEdit->setTime(time1);
                    ui->timeEdit_2->setTime(time2);
                } else {
                    qDebug() << "Invalid time string format";
                }
            } else {
                qDebug() << "Query execution error: " << query.lastError().text();
            }
        } else {
            qDebug() << "Query execution error: " << query.lastError().text();
        }

        // Close the database connection
        db.close();
    } else {
        qDebug() << "Database not opened. Error: " << db.lastError().text();
    }
}


void response::on_pushButton_clicked()
{
    int result = QMessageBox::question(this, "Confirmation", "Are you sure you want to Accept?", QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/VISHNU/calenx.db");

        if (db.open()) {
            QSqlQuery query;
            QString username;

            query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

            if (query.exec() && query.next()) {
                username = query.value(0).toString();
                QString task = ui->comboBox->currentText();
                QStringList m = task.split(" by ");
                QString stime = ui->timeEdit->text();
                QString etime = ui->timeEdit_2->text();

                // Corrected the INSERT statement
                query.prepare("INSERT INTO Task (TASK, STIME, ETIME, USERNAME) VALUES (:task, :stime, :etime, :username)");
                query.bindValue(":task", m[0]);
                query.bindValue(":stime", stime);
                query.bindValue(":etime", etime);
                query.bindValue(":username", username);

                if (query.exec()) {
                    // Corrected the UPDATE statement
                    query.prepare("UPDATE Notification SET accrej='A' WHERE task=:task AND stime=:stime AND etime=:etime AND fromwhom=:fromwhom AND towhom=:username AND accrej IS NULL");
                    query.bindValue(":task", m[0]);
                    query.bindValue(":stime", stime);
                    query.bindValue(":etime", etime);
                    query.bindValue(":fromwhom", m[1]);
                    query.bindValue(":username", username);

                    if (query.exec()) {
                        QMessageBox::information(this, "Saved", "Saved");
                    } else {
                        qDebug() << "Error Occurred in UPDATE statement: " << query.lastError().text();
                    }
                } else {
                    qDebug() << "Error Occurred in INSERT statement: " << query.lastError().text();
                }
            } else {
                qDebug() << "Not Working";
            }

            db.close();
        } else {
            qDebug() << "The database did not connect";
        }
    }
    else{
        setVisible(false);
    }
}

void response::on_pushButton_2_clicked()
{
    int result = QMessageBox::question(this, "Confirmation", "Are you sure you want to Reject?", QMessageBox::Yes | QMessageBox::No);
    if(result==QMessageBox::Yes){
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/Users/VISHNU/calenx.db");

        if (db.open()) {
            QSqlQuery query;
            QString username;

            query.prepare("SELECT USERNAME FROM LOGIN WHERE USERNAME IN (SELECT username FROM Loginstatus ORDER BY DATE DESC, TIME DESC LIMIT 1)");

            if (query.exec() && query.next()) {
                username = query.value(0).toString();
                QString task = ui->comboBox->currentText();
                QStringList m = task.split(" by ");
                QString stime = ui->timeEdit->text();
                QString etime = ui->timeEdit_2->text();
                query.prepare("UPDATE Notification SET accrej='R' WHERE task=:task AND stime=:stime AND etime=:etime AND fromwhom=:fromwhom AND towhom=:username AND accrej IS NULL");
                query.bindValue(":task", m[0]);
                query.bindValue(":stime", stime);
                query.bindValue(":etime", etime);
                query.bindValue(":fromwhom", m[1]);
                query.bindValue(":username", username);
                if (query.exec()) {
                    QMessageBox::information(this, "Saved", "Saved");
                } else {
                    qDebug() << "Error Occurred in UPDATE statement: " << query.lastError().text();
                }
            }
        }
        else{
            qDebug()<<"Database did not Open";
        }
    }
    else if(QMessageBox::No){
        setVisible(false);
    }
}
