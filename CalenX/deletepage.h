#ifndef DELETEPAGE_H
#define DELETEPAGE_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>

namespace Ui {
class deletepage;
}

class deletepage : public QDialog
{
    Q_OBJECT

public:
    explicit deletepage(QWidget *parent = nullptr);
    ~deletepage();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::deletepage *ui;
};

#endif // DELETEPAGE_H
