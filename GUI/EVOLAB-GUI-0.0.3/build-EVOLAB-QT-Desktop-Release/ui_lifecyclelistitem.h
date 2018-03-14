/********************************************************************************
** Form generated from reading UI file 'lifecyclelistitem.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIFECYCLELISTITEM_H
#define UI_LIFECYCLELISTITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LifeCycleListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QComboBox *comboBox;

    void setupUi(QWidget *LifeCycleListItem)
    {
        if (LifeCycleListItem->objectName().isEmpty())
            LifeCycleListItem->setObjectName(QString::fromUtf8("LifeCycleListItem"));
        LifeCycleListItem->resize(477, 27);
        horizontalLayout = new QHBoxLayout(LifeCycleListItem);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit = new QLineEdit(LifeCycleListItem);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setCursorPosition(0);

        horizontalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(LifeCycleListItem);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setCursorPosition(0);

        horizontalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(LifeCycleListItem);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setCursorPosition(0);

        horizontalLayout->addWidget(lineEdit_3);

        comboBox = new QComboBox(LifeCycleListItem);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);


        retranslateUi(LifeCycleListItem);

        QMetaObject::connectSlotsByName(LifeCycleListItem);
    } // setupUi

    void retranslateUi(QWidget *LifeCycleListItem)
    {
        LifeCycleListItem->setWindowTitle(QApplication::translate("LifeCycleListItem", "Form", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("LifeCycleListItem", "Name", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("LifeCycleListItem", "Rate (var or func)", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setText(QApplication::translate("LifeCycleListItem", "State", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("LifeCycleListItem", "Local", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LifeCycleListItem", "Global", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class LifeCycleListItem: public Ui_LifeCycleListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIFECYCLELISTITEM_H
