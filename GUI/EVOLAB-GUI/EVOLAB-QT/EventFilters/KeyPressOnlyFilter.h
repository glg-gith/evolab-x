#ifndef KEYPRESSONLYFILTER_H
#define KEYPRESSONLYFILTER_H

#include <QObject>
#include <QEvent>

class KeyPressOnlyFilter : public QObject
{
    Q_OBJECT

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
};


#endif // KEYPRESSONLYFILTER_H
