#ifndef VARLISTITEM_H
#define VARLISTITEM_H

#include <QWidget>

namespace Ui {
class VarListItem;
}

class VarListItem : public QWidget
{
    Q_OBJECT

public:
    explicit VarListItem(QWidget *parent = 0);
    ~VarListItem();

private:
    Ui::VarListItem *ui;
};

#endif // VARLISTITEM_H
