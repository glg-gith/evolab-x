#include "ExceptionCatchingQApplication.h"

ExceptionCatchingQApplication::ExceptionCatchingQApplication(int & argc, char ** argv):
    QApplication(argc, argv)
{
}

bool ExceptionCatchingQApplication::notify(QObject * receiver, QEvent * event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch(std::exception& e)
    {
        std::clog << "Exception thrown: " << e.what() << std::endl;
    }
}

