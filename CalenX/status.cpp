#include "status.h"
#include "ui_status.h"

status::status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::status)
{
    ui->setupUi(this);
}

status::~status()
{
    delete ui;
}
