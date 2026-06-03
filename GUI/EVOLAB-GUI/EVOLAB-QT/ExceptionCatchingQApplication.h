#ifndef EXCEPTIONCATCHINGQAPPLICATION_H
#define EXCEPTIONCATCHINGQAPPLICATION_H

#include <QApplication>

#include <iostream>


class ExceptionCatchingQApplication : public QApplication
{
public:
    ExceptionCatchingQApplication(int & argc, char ** argv);

    virtual bool notify(QObject * receiver, QEvent * event);
};

#endif // EXCEPTIONCATCHINGQAPPLICATION_H
