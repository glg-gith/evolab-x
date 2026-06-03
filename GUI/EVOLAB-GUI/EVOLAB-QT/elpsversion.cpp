#include "elpsversion.h"
#include "ui_elpsversion.h"

ElpsVersion::ElpsVersion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElpsVersion)
{
    ui->setupUi(this);
}

ElpsVersion::~ElpsVersion()
{
    delete ui;
}

void ElpsVersion::on_pushButton_clicked()
{
    this->close();
}
