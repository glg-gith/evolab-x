/********************************************************************************
** Form generated from reading UI file 'elpsversion.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ELPSVERSION_H
#define UI_ELPSVERSION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ElpsVersion
{
public:
    QFormLayout *formLayout;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ElpsVersion)
    {
        if (ElpsVersion->objectName().isEmpty())
            ElpsVersion->setObjectName(QStringLiteral("ElpsVersion"));
        ElpsVersion->setWindowModality(Qt::NonModal);
        ElpsVersion->resize(200, 36);
        QFont font;
        font.setPointSize(12);
        ElpsVersion->setFont(font);
        ElpsVersion->setMouseTracking(true);
        ElpsVersion->setFocusPolicy(Qt::StrongFocus);
        ElpsVersion->setAcceptDrops(true);
        ElpsVersion->setAutoFillBackground(true);
        formLayout = new QFormLayout(ElpsVersion);
        formLayout->setContentsMargins(2, 2, 2, 2);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        frame = new QFrame(ElpsVersion);
        frame->setObjectName(QStringLiteral("frame"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        frame->setFont(font1);
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::Panel);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(2);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(9, 0, 0, 0);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamily(QStringLiteral("Monospace"));
        font2.setPointSize(10);
        font2.setItalic(true);
        label->setFont(font2);

        horizontalLayout_2->addWidget(label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 5, -1, -1);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(16, 16));
        QIcon icon;
        icon.addFile(QStringLiteral(":/new/prefix1/close-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setFlat(true);

        verticalLayout->addWidget(pushButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        formLayout->setWidget(0, QFormLayout::SpanningRole, frame);


        retranslateUi(ElpsVersion);

        QMetaObject::connectSlotsByName(ElpsVersion);
    } // setupUi

    void retranslateUi(QWidget *ElpsVersion)
    {
        label->setText(QApplication::translate("ElpsVersion", "TextLabel", 0));
        pushButton->setText(QString());
        Q_UNUSED(ElpsVersion);
    } // retranslateUi

};

namespace Ui {
    class ElpsVersion: public Ui_ElpsVersion {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ELPSVERSION_H
