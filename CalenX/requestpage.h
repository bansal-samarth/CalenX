#ifndef REQUESTPAGE_H
#define REQUESTPAGE_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>

namespace Ui {
class requestpage;
}

class requestpage : public QDialog
{
    Q_OBJECT

public:
    explicit requestpage(QWidget *parent = nullptr);
    ~requestpage();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::requestpage *ui;
};

#endif // REQUESTPAGE_H
