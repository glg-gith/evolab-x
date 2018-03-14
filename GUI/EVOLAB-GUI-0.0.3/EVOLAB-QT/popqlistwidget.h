#ifndef POPQLISTWIDGET_H
#define POPQLISTWIDGET_H

#include <QListWidget>

#include "Models/PointerLessModels.h"

class PopQListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit PopQListWidget(QWidget *parent = 0);

    void EmitUpdateForeignModelSignal(int index, PointerLessModels::t_basic_item *p_item);

    //void SetCurrentSelected(int index);

signals:
    void updateForeignModelSignal(int index, PointerLessModels::t_basic_item *p_item);

public slots:

protected:
    virtual void dropEvent(QDropEvent *event);

private:
    int currentSelected;

};

#endif // POPQLISTWIDGET_H
