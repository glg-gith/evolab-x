#ifndef LIFECYCLELISTITEM_H
#define LIFECYCLELISTITEM_H

#include <QWidget>

namespace Ui {
class LifeCycleListItem;
}

class LifeCycleListItem : public QWidget
{
    Q_OBJECT

public:
    explicit LifeCycleListItem(QWidget *parent = 0);
    ~LifeCycleListItem();

private:
    Ui::LifeCycleListItem *ui;
};

#endif // LIFECYCLELISTITEM_H
