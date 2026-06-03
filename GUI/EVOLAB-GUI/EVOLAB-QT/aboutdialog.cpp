#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->label_EvolabVersion->setText(QString("V ") + QUOTEME(EVOLAB_GUI_VERSION));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
