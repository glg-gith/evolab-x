
#include "EventFilters/MouseAndKeyPressOnlyFilter.h"

bool MouseAndKeyPressOnlyFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || (event->type() >= 2 && event->type() <= 5 )) {
        // Standard event processing
        return QObject::eventFilter(obj, event);
    } else {
        // Filter any other event
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug("Ate key press %d", keyEvent->key());
        return true;
    }
}

