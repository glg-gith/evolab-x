/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_EvolabVersion;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QStringLiteral("AboutDialog"));
        AboutDialog->resize(345, 253);
        AboutDialog->setWindowOpacity(0.7);
        label = new QLabel(AboutDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 10, 101, 91));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/logo-base.png")));
        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 10, 130, 31));
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(14);
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_EvolabVersion = new QLabel(AboutDialog);
        label_EvolabVersion->setObjectName(QStringLiteral("label_EvolabVersion"));
        label_EvolabVersion->setGeometry(QRect(70, 40, 90, 30));
        QFont font1;
        font1.setFamily(QStringLiteral("Mitra Mono"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setItalic(true);
        font1.setWeight(75);
        label_EvolabVersion->setFont(font1);
        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 120, 251, 17));
        QFont font2;
        font2.setFamily(QStringLiteral("Monospace"));
        font2.setPointSize(10);
        font2.setItalic(false);
        label_3->setFont(font2);
        label_5 = new QLabel(AboutDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(100, 150, 231, 17));
        label_5->setFont(font2);
        label_5->setOpenExternalLinks(true);
        label_6 = new QLabel(AboutDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 190, 111, 17));
        QFont font3;
        font3.setFamily(QStringLiteral("Monospace"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        label_6->setFont(font3);
        label_7 = new QLabel(AboutDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(160, 200, 161, 41));
        QFont font4;
        font4.setFamily(QStringLiteral("Monospace"));
        font4.setPointSize(10);
        label_7->setFont(font4);

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "About...", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QApplication::translate("AboutDialog", "EVOLAB-GUI", Q_NULLPTR));
        label_EvolabVersion->setText(QApplication::translate("AboutDialog", "V 0.0.1", Q_NULLPTR));
        label_3->setText(QApplication::translate("AboutDialog", "<html><head/><body><p>Designed By <span style=\" font-weight:600;\">Gauthier Boaglio</span></p></body></html>", Q_NULLPTR));
        label_5->setText(QApplication::translate("AboutDialog", "<html><head/><body><p><a href=\"http://www.evolepid.org/\"><span style=\" font-weight:600; text-decoration: underline; color:#00007f;\">EEE</span></a>  -  <a href=\"http://www.cefe.cnrs.fr/en\"><span style=\" font-weight:600; text-decoration: underline; color:#00007f;\">CEFE</span></a> / CNRS - UMR 5175</p></body></html>", Q_NULLPTR));
        label_6->setText(QApplication::translate("AboutDialog", "<html><head/><body><p><span style=\" font-weight:400; font-style:italic; color:#006336;\">Supervisors :</span></p></body></html>", Q_NULLPTR));
        label_7->setText(QApplication::translate("AboutDialog", "<html><head/><body><p><span style=\" font-style:italic; color:#006336;\">PhD - Sylvain Gandon<br/>PhD - Sebastien Lion</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
