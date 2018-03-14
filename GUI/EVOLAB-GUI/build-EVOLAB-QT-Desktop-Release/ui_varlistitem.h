/********************************************************************************
** Form generated from reading UI file 'varlistitem.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARLISTITEM_H
#define UI_VARLISTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VarListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QWidget *VarListItem)
    {
        if (VarListItem->objectName().isEmpty())
            VarListItem->setObjectName(QStringLiteral("VarListItem"));
        VarListItem->resize(280, 27);
        horizontalLayout = new QHBoxLayout(VarListItem);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEdit = new QLineEdit(VarListItem);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(VarListItem);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout->addWidget(lineEdit_2);


        retranslateUi(VarListItem);

        QMetaObject::connectSlotsByName(VarListItem);
    } // setupUi

    void retranslateUi(QWidget *VarListItem)
    {
        VarListItem->setWindowTitle(QApplication::translate("VarListItem", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class VarListItem: public Ui_VarListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARLISTITEM_H
