#include "varlistitem.h"
#include "ui_varlistitem.h"

VarListItem::VarListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VarListItem)
{
    ui->setupUi(this);
}

VarListItem::~VarListItem()
{
    delete ui;
}
