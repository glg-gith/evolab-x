#ifndef MOUSEANDKEYPRESSONLYFILTER_H
#define MOUSEANDKEYPRESSONLYFILTER_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>

class MouseAndKeyPressOnlyFilter : public QObject
{
    Q_OBJECT

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
};


#endif // MOUSEANDKEYPRESSONLYFILTER_H
