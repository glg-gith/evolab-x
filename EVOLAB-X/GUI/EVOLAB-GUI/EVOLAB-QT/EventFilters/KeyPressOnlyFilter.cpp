
#include "EventFilters/KeyPressOnlyFilter.h"

bool KeyPressOnlyFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        // Standard event processing
        return QObject::eventFilter(obj, event);
    } else {
        // Filter any other event
//        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        qDebug("Ate key press %d", keyEvent->key());
        return true;
    }
}

