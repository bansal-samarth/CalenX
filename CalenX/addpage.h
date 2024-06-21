#ifndef ADDPAGE_H
#define ADDPAGE_H

#include <QDialog>
#include <QMessageBox>
#include <QtSql>
#include <QSqlDatabase>

namespace Ui {
class Addpage;
}

class Addpage : public QDialog
{
    Q_OBJECT
public:
    explicit Addpage(QWidget *parent = nullptr);
    ~Addpage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Addpage *ui;
};

#endif // ADDPAGE_H
