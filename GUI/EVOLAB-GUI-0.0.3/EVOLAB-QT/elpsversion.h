#ifndef ELPSVERSION_H
#define ELPSVERSION_H

#include <QWidget>

namespace Ui {
class ElpsVersion;
}

class ElpsVersion : public QWidget
{
    Q_OBJECT

public:
    explicit ElpsVersion(QWidget *parent = 0);
    ~ElpsVersion();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ElpsVersion *ui;
};

#endif // ELPSVERSION_H
