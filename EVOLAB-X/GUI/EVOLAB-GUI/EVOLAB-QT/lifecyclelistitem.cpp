#include "lifecyclelistitem.h"
#include "ui_lifecyclelistitem.h"

LifeCycleListItem::LifeCycleListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LifeCycleListItem)
{
    ui->setupUi(this);
}

LifeCycleListItem::~LifeCycleListItem()
{
    delete ui;
}
