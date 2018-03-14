/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "popqlistwidget.h"
#include "qtexteditcodeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAbout;
    QAction *actionHelp;
    QAction *actionQuit;
    QAction *actionLoad_Project;
    QAction *actionAbout2;
    QAction *actionBuild;
    QAction *actionCompile_Simulation;
    QAction *actionNew_C_Project;
    QAction *actionNew_Python_Project;
    QAction *actionSave_Project;
    QAction *actionRun_Simulation;
    QAction *actionOutput;
    QAction *actionRun;
    QAction *actionPreferences;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionBuild_Output;
    QAction *actionOuput_Window;
    QAction *actionELPS_Version;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionExtended_Toolbar;
    QAction *actionExport_Source;
    QAction *actionContext_Toolbar;
    QAction *actionSave_Tab;
    QAction *actionLoad_Tab;
    QAction *actionDefault_Tab;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_20;
    QTabWidget *tabWidget_Top;
    QWidget *tab_Populations;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_12;
    QVBoxLayout *verticalLayout_14;
    QFrame *frame_4;
    QVBoxLayout *verticalLayout_23;
    QLabel *label_2;
    QToolButton *toolButton_AddPop;
    QToolButton *toolButton_RemovePop;
    QToolButton *toolButton_PopRowUp;
    QToolButton *toolButton_PopRowDown;
    QFrame *frame_15;
    QToolButton *toolButton_DuplicatePop;
    QSpacerItem *verticalSpacer_9;
    QFrame *frame_12;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_5;
    QToolButton *toolButton_25;
    QToolButton *toolButton_LockEditPopCellItem;
    QFrame *frame_16;
    QToolButton *toolButton_CopyPopCellItem;
    QToolButton *toolButton_CutPopCellItem;
    QToolButton *toolButton_PastePopCellItem;
    QSpacerItem *verticalSpacer_10;
    QTableWidget *tableWidget_Pop;
    QStackedWidget *stackedWidget_Pop;
    QWidget *page_PopCellSettings;
    QVBoxLayout *verticalLayout_32;
    QGroupBox *groupBox_SelectedCell;
    QVBoxLayout *verticalLayout_33;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_13;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_3;
    QFrame *frame_10;
    QVBoxLayout *verticalLayout_24;
    QToolButton *toolButton_Pop_AddItem;
    QToolButton *toolButton_27;
    QToolButton *toolButton_PopCellItemUp;
    QToolButton *toolButton_PopCellItemDown;
    QFrame *frame_17;
    QToolButton *toolButton_28;
    QSpacerItem *verticalSpacer_3;
    QFrame *frame_SelectedCell;
    QVBoxLayout *verticalLayout_27;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QFrame *frame_ItemSettings;
    QVBoxLayout *verticalLayout_29;
    QGridLayout *gridLayout_ItemSettings;
    QLabel *label_8;
    QComboBox *comboBox_3;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *verticalSpacer_11;
    QLabel *label_7;
    PopQListWidget *listWidget_ItemList;
    QVBoxLayout *verticalLayout_25;
    QLabel *label_11;
    QPlainTextEdit *plainTextEdit_ItemDesc;
    QSpacerItem *verticalSpacer_17;
    QWidget *page_PopDescription;
    QVBoxLayout *verticalLayout_26;
    QGroupBox *groupBox_PopDescription;
    QVBoxLayout *verticalLayout_34;
    QVBoxLayout *verticalLayout_28;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_12;
    QLabel *label_PopDescription;
    QPlainTextEdit *plainText_Edit_PopDescription;
    QSpacerItem *verticalSpacer_12;
    QWidget *tab_Network;
    QHBoxLayout *horizontalLayout_14;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout_6;
    QGridLayout *gridLayout;
    QLabel *label_NetworkNbNode;
    QSpinBox *spinBox_NetworkNbNodes;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_NetworkDist;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox_NetworkDist;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QToolButton *toolButton_NetworkResetDist;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_10;
    QFrame *frame_NetworkParams;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_8;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_8;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_6;
    QComboBox *comboBox_NetPopsSpread;
    QHBoxLayout *horizontalLayout_22;
    QTableWidget *tableWidget_NetPopsRepartition;
    QFrame *frame_NetPopTotalInfo;
    QHBoxLayout *horizontalLayout_23;
    QLabel *label_NetPopIcon_2;
    QTextEdit *textEdit_NetPopTotalInfo;
    QFrame *frame_22;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_18;
    QLabel *label_NetPopIcon;
    QTextEdit *textEdit_SpreadTotal;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_16;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_NetGenerate;
    QWidget *tab_Variables;
    QVBoxLayout *verticalLayout_11;
    QHBoxLayout *horizontalLayout_9;
    QHBoxLayout *horizontalLayout_2;
    QFrame *frame_11;
    QVBoxLayout *verticalLayout_36;
    QLabel *label_30;
    QToolButton *toolButton_VarAdd;
    QToolButton *toolButton_VarRemove;
    QToolButton *toolButton_VarRowUp;
    QToolButton *toolButton_VarRowDown;
    QSpacerItem *verticalSpacer_16;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_21;
    QTableWidget *tableWidget_Variables;
    QLabel *label_29;
    QPlainTextEdit *plainTextEdit_VarDesc;
    QGroupBox *groupBox_VarFuncTrashed;
    QHBoxLayout *horizontalLayout_10;
    QFrame *frame_9;
    QVBoxLayout *verticalLayout_22;
    QGridLayout *gridLayout_5;
    QToolButton *toolButton_10;
    QLabel *label_15;
    QComboBox *comboBox_4;
    QToolButton *toolButton_11;
    QComboBox *comboBox_2;
    QLabel *label_16;
    QSpacerItem *verticalSpacer_7;
    QPlainTextEdit *plainTextEdit;
    QWidget *tab_LifeCycle;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_19;
    QVBoxLayout *verticalLayout_35;
    QLabel *label_14;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_31;
    QToolButton *toolButton_32;
    QFrame *frame_20;
    QToolButton *toolButton_33;
    QSpacerItem *verticalSpacer_2;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_30;
    QLabel *label_17;
    QToolButton *toolButton_34;
    QFrame *frame_21;
    QToolButton *toolButton_16;
    QToolButton *toolButton_35;
    QToolButton *toolButton_14;
    QSpacerItem *verticalSpacer_13;
    QVBoxLayout *verticalLayout_37;
    QTableWidget *tableWidget_LifeCycle;
    QFrame *frame_27;
    QLabel *label_21;
    QPlainTextEdit *plainTextEdit_3;
    QGroupBox *groupBox_VarFuncCreator;
    QGridLayout *gridLayout_8;
    QToolBox *toolBox_CodeEditor;
    QWidget *CurrentFormulaEditing;
    QHBoxLayout *horizontalLayout_26;
    QTextEditCodeEditor *textEdit_Formula;
    QWidget *CustomMacroEditing;
    QHBoxLayout *horizontalLayout_27;
    QTextEditCodeEditor *textEdit_CustomMacro;
    QFrame *frame_13;
    QVBoxLayout *verticalLayout_31;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_27;
    QLabel *label_22;
    QFrame *frame_AvailableMacros;
    QHBoxLayout *horizontalLayout_25;
    QGridLayout *gridLayout_AvailableMacros;
    QComboBox *comboBox_ElpBuiltin;
    QToolButton *toolButton_26;
    QLabel *label;
    QComboBox *comboBox_ElpBuiltinKeywords;
    QLabel *label_19;
    QToolButton *toolButton_FormulaInsertKeyword;
    QToolButton *toolButton_2;
    QHBoxLayout *horizontalLayout_24;
    QComboBox *comboBox_StdMath;
    QComboBox *comboBox_CatStdMath;
    QLabel *label_31;
    QLabel *label_20;
    QToolButton *toolButton_CustomMacroInsert;
    QHBoxLayout *horizontalLayout_28;
    QToolButton *toolButton_CustomMacroEdit;
    QComboBox *comboBox_CustomMacros;
    QSpacerItem *verticalSpacer_6;
    QFrame *frame_18;
    QSpacerItem *verticalSpacer_15;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_26;
    QLabel *label_25;
    QFrame *frame_CustomMacro;
    QVBoxLayout *verticalLayout_12;
    QFrame *frame_14;
    QHBoxLayout *horizontalLayout_8;
    QGridLayout *gridLayout_10;
    QHBoxLayout *horizontalLayout_30;
    QComboBox *comboBox_CustomMacroParamsNames;
    QToolButton *toolButton_5;
    QHBoxLayout *horizontalLayout_31;
    QLineEdit *lineEdit_CustomMacroName;
    QToolButton *toolButton_8;
    QHBoxLayout *horizontalLayout_29;
    QLineEdit *lineEdit_CustomMacroParamName;
    QLabel *label_34;
    QVBoxLayout *verticalLayout_41;
    QComboBox *comboBox_CustomMacroStyle;
    QFrame *frame_24;
    QLabel *label_32;
    QVBoxLayout *verticalLayout_39;
    QFrame *frame_8;
    QHBoxLayout *horizontalLayout_6;
    QToolButton *toolButton_CustomMacroAddNew;
    QToolButton *toolButton_CustomMacroDelete;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *toolButton_CustomMacroApply;
    QToolButton *toolButton_CustomMacroDiscard;
    QFrame *frame_23;
    QVBoxLayout *verticalLayout_38;
    QFrame *frame_26;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_24;
    QLabel *label_23;
    QVBoxLayout *verticalLayout_40;
    QLabel *label_33;
    QFrame *frame_25;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_32;
    QToolButton *toolButton_CustomMacroAddNew_2;
    QToolButton *toolButton_CustomMacroDelete_2;
    QComboBox *comboBox_CustomMacroReturns;
    QSpacerItem *verticalSpacer_14;
    QLabel *label_MacroItemName;
    QHBoxLayout *horizontalLayout_33;
    QSpacerItem *horizontalSpacer_8;
    QToolButton *toolButton_EditorCheckSyntax;
    QWidget *tab_Outputs;
    QVBoxLayout *verticalLayout_19;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_18;
    QFrame *frame_6;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer_5;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuBuild;
    QMenu *menuView;
    QMenu *menuToolbars;
    QMenu *menuEdit;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QFrame *frame_5;
    QVBoxLayout *verticalLayout_9;
    QToolButton *toolButton;
    QToolButton *toolButton_15;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_15;
    QTabWidget *tabWidget_Outputs;
    QWidget *tab_5;
    QVBoxLayout *verticalLayout_16;
    QTextEdit *textEdit_2;
    QWidget *tab_6;
    QVBoxLayout *verticalLayout_17;
    QPlainTextEdit *plainTextEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *toolButton_StopProcess;
    QLabel *label_ProcessingInfo;
    QProgressBar *progressBar;
    QLabel *label_9;
    QToolBar *toolBar_Context;
    QToolBar *toolBar_Extended;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1141, 953);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/logo-base-fav.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/about-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon1);
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/help-contents-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon2);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionLoad_Project = new QAction(MainWindow);
        actionLoad_Project->setObjectName(QString::fromUtf8("actionLoad_Project"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/Folder-Open-Blue-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Project->setIcon(icon3);
        actionAbout2 = new QAction(MainWindow);
        actionAbout2->setObjectName(QString::fromUtf8("actionAbout2"));
        actionAbout2->setIcon(icon);
        actionBuild = new QAction(MainWindow);
        actionBuild->setObjectName(QString::fromUtf8("actionBuild"));
        actionBuild->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/hammer-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild->setIcon(icon4);
        actionCompile_Simulation = new QAction(MainWindow);
        actionCompile_Simulation->setObjectName(QString::fromUtf8("actionCompile_Simulation"));
        actionCompile_Simulation->setIcon(icon4);
        actionNew_C_Project = new QAction(MainWindow);
        actionNew_C_Project->setObjectName(QString::fromUtf8("actionNew_C_Project"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/source-cpp-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_C_Project->setIcon(icon5);
        actionNew_Python_Project = new QAction(MainWindow);
        actionNew_Python_Project->setObjectName(QString::fromUtf8("actionNew_Python_Project"));
        actionNew_Python_Project->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/source-py-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Python_Project->setIcon(icon6);
        actionSave_Project = new QAction(MainWindow);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/new/prefix1/Save-icon-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project->setIcon(icon7);
        actionRun_Simulation = new QAction(MainWindow);
        actionRun_Simulation->setObjectName(QString::fromUtf8("actionRun_Simulation"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/new/prefix1/Play-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun_Simulation->setIcon(icon8);
        actionOutput = new QAction(MainWindow);
        actionOutput->setObjectName(QString::fromUtf8("actionOutput"));
        actionOutput->setCheckable(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/new/prefix1/output-run-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOutput->setIcon(icon9);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionRun->setCheckable(true);
        actionRun->setIcon(icon8);
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/new/prefix1/preferences-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreferences->setIcon(icon10);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/new/prefix1/undo-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon11);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/new/prefix1/redo-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon12);
        actionBuild_Output = new QAction(MainWindow);
        actionBuild_Output->setObjectName(QString::fromUtf8("actionBuild_Output"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/new/prefix1/build-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBuild_Output->setIcon(icon13);
        actionOuput_Window = new QAction(MainWindow);
        actionOuput_Window->setObjectName(QString::fromUtf8("actionOuput_Window"));
        actionOuput_Window->setCheckable(true);
        actionOuput_Window->setChecked(true);
        actionELPS_Version = new QAction(MainWindow);
        actionELPS_Version->setObjectName(QString::fromUtf8("actionELPS_Version"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/new/prefix1/elp-shared.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionELPS_Version->setIcon(icon14);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/new/prefix1/copy-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon15);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/new/prefix1/cut-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon16);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/new/prefix1/paste-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon17);
        actionExtended_Toolbar = new QAction(MainWindow);
        actionExtended_Toolbar->setObjectName(QString::fromUtf8("actionExtended_Toolbar"));
        actionExtended_Toolbar->setCheckable(true);
        actionExtended_Toolbar->setChecked(true);
        actionExport_Source = new QAction(MainWindow);
        actionExport_Source->setObjectName(QString::fromUtf8("actionExport_Source"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/new/prefix1/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_Source->setIcon(icon18);
        actionContext_Toolbar = new QAction(MainWindow);
        actionContext_Toolbar->setObjectName(QString::fromUtf8("actionContext_Toolbar"));
        actionContext_Toolbar->setCheckable(true);
        actionContext_Toolbar->setChecked(true);
        actionSave_Tab = new QAction(MainWindow);
        actionSave_Tab->setObjectName(QString::fromUtf8("actionSave_Tab"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/new/prefix1/save-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Tab->setIcon(icon19);
        actionLoad_Tab = new QAction(MainWindow);
        actionLoad_Tab->setObjectName(QString::fromUtf8("actionLoad_Tab"));
        actionLoad_Tab->setIcon(icon3);
        actionDefault_Tab = new QAction(MainWindow);
        actionDefault_Tab->setObjectName(QString::fromUtf8("actionDefault_Tab"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/new/prefix1/Very-Basic-settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDefault_Tab->setIcon(icon20);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, 0, 9);
        verticalLayout_20 = new QVBoxLayout();
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        verticalLayout_20->setContentsMargins(-1, -1, 9, -1);
        tabWidget_Top = new QTabWidget(centralWidget);
        tabWidget_Top->setObjectName(QString::fromUtf8("tabWidget_Top"));
        tab_Populations = new QWidget();
        tab_Populations->setObjectName(QString::fromUtf8("tab_Populations"));
        verticalLayout_13 = new QVBoxLayout(tab_Populations);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(2);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(0, -1, -1, -1);
        frame_4 = new QFrame(tab_Populations);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setMinimumSize(QSize(20, 0));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        verticalLayout_23 = new QVBoxLayout(frame_4);
        verticalLayout_23->setSpacing(3);
        verticalLayout_23->setContentsMargins(3, 3, 3, 3);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        label_2 = new QLabel(frame_4);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        verticalLayout_23->addWidget(label_2);

        toolButton_AddPop = new QToolButton(frame_4);
        toolButton_AddPop->setObjectName(QString::fromUtf8("toolButton_AddPop"));
        toolButton_AddPop->setMaximumSize(QSize(16777215, 30));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/new/prefix1/add-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_AddPop->setIcon(icon21);

        verticalLayout_23->addWidget(toolButton_AddPop);

        toolButton_RemovePop = new QToolButton(frame_4);
        toolButton_RemovePop->setObjectName(QString::fromUtf8("toolButton_RemovePop"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/new/prefix1/remove-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_RemovePop->setIcon(icon22);

        verticalLayout_23->addWidget(toolButton_RemovePop);

        toolButton_PopRowUp = new QToolButton(frame_4);
        toolButton_PopRowUp->setObjectName(QString::fromUtf8("toolButton_PopRowUp"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/new/prefix1/up-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_PopRowUp->setIcon(icon23);

        verticalLayout_23->addWidget(toolButton_PopRowUp);

        toolButton_PopRowDown = new QToolButton(frame_4);
        toolButton_PopRowDown->setObjectName(QString::fromUtf8("toolButton_PopRowDown"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/new/prefix1/down-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_PopRowDown->setIcon(icon24);

        verticalLayout_23->addWidget(toolButton_PopRowDown);

        frame_15 = new QFrame(frame_4);
        frame_15->setObjectName(QString::fromUtf8("frame_15"));
        frame_15->setFrameShape(QFrame::HLine);
        frame_15->setFrameShadow(QFrame::Sunken);

        verticalLayout_23->addWidget(frame_15);

        toolButton_DuplicatePop = new QToolButton(frame_4);
        toolButton_DuplicatePop->setObjectName(QString::fromUtf8("toolButton_DuplicatePop"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/new/prefix1/duplicate-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_DuplicatePop->setIcon(icon25);

        verticalLayout_23->addWidget(toolButton_DuplicatePop);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_23->addItem(verticalSpacer_9);


        verticalLayout_14->addWidget(frame_4);

        frame_12 = new QFrame(tab_Populations);
        frame_12->setObjectName(QString::fromUtf8("frame_12"));
        frame_12->setFrameShape(QFrame::StyledPanel);
        frame_12->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_12);
        verticalLayout_5->setSpacing(3);
        verticalLayout_5->setContentsMargins(3, 3, 3, 3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_5 = new QLabel(frame_12);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        verticalLayout_5->addWidget(label_5);

        toolButton_25 = new QToolButton(frame_12);
        toolButton_25->setObjectName(QString::fromUtf8("toolButton_25"));
        toolButton_25->setEnabled(false);
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/new/prefix1/move-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon26.addFile(QString::fromUtf8(":/new/prefix1/pointer-icon.png"), QSize(), QIcon::Normal, QIcon::On);
        toolButton_25->setIcon(icon26);
        toolButton_25->setCheckable(true);

        verticalLayout_5->addWidget(toolButton_25);

        toolButton_LockEditPopCellItem = new QToolButton(frame_12);
        toolButton_LockEditPopCellItem->setObjectName(QString::fromUtf8("toolButton_LockEditPopCellItem"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/new/prefix1/lock-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon27.addFile(QString::fromUtf8(":/new/prefix1/edit-icon.png"), QSize(), QIcon::Normal, QIcon::On);
        toolButton_LockEditPopCellItem->setIcon(icon27);
        toolButton_LockEditPopCellItem->setCheckable(true);

        verticalLayout_5->addWidget(toolButton_LockEditPopCellItem);

        frame_16 = new QFrame(frame_12);
        frame_16->setObjectName(QString::fromUtf8("frame_16"));
        frame_16->setFrameShape(QFrame::HLine);
        frame_16->setFrameShadow(QFrame::Sunken);

        verticalLayout_5->addWidget(frame_16);

        toolButton_CopyPopCellItem = new QToolButton(frame_12);
        toolButton_CopyPopCellItem->setObjectName(QString::fromUtf8("toolButton_CopyPopCellItem"));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/new/prefix1/copy-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_CopyPopCellItem->setIcon(icon28);

        verticalLayout_5->addWidget(toolButton_CopyPopCellItem);

        toolButton_CutPopCellItem = new QToolButton(frame_12);
        toolButton_CutPopCellItem->setObjectName(QString::fromUtf8("toolButton_CutPopCellItem"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/new/prefix1/cut-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_CutPopCellItem->setIcon(icon29);

        verticalLayout_5->addWidget(toolButton_CutPopCellItem);

        toolButton_PastePopCellItem = new QToolButton(frame_12);
        toolButton_PastePopCellItem->setObjectName(QString::fromUtf8("toolButton_PastePopCellItem"));
        QIcon icon30;
        icon30.addFile(QString::fromUtf8(":/new/prefix1/paste-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_PastePopCellItem->setIcon(icon30);

        verticalLayout_5->addWidget(toolButton_PastePopCellItem);

        verticalSpacer_10 = new QSpacerItem(20, 22, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_10);


        verticalLayout_14->addWidget(frame_12);


        horizontalLayout_12->addLayout(verticalLayout_14);

        tableWidget_Pop = new QTableWidget(tab_Populations);
        if (tableWidget_Pop->columnCount() < 4)
            tableWidget_Pop->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_Pop->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_Pop->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_Pop->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_Pop->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget_Pop->setObjectName(QString::fromUtf8("tableWidget_Pop"));
        tableWidget_Pop->setMinimumSize(QSize(500, 0));
        tableWidget_Pop->setDragEnabled(false);
        tableWidget_Pop->setDragDropOverwriteMode(true);
        tableWidget_Pop->setDragDropMode(QAbstractItemView::NoDragDrop);
        tableWidget_Pop->setDefaultDropAction(Qt::IgnoreAction);
        tableWidget_Pop->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_Pop->horizontalHeader()->setDefaultSectionSize(120);
        tableWidget_Pop->horizontalHeader()->setMinimumSectionSize(120);
        tableWidget_Pop->horizontalHeader()->setStretchLastSection(true);

        horizontalLayout_12->addWidget(tableWidget_Pop);

        stackedWidget_Pop = new QStackedWidget(tab_Populations);
        stackedWidget_Pop->setObjectName(QString::fromUtf8("stackedWidget_Pop"));
        page_PopCellSettings = new QWidget();
        page_PopCellSettings->setObjectName(QString::fromUtf8("page_PopCellSettings"));
        verticalLayout_32 = new QVBoxLayout(page_PopCellSettings);
        verticalLayout_32->setSpacing(3);
        verticalLayout_32->setContentsMargins(3, 3, 3, 3);
        verticalLayout_32->setObjectName(QString::fromUtf8("verticalLayout_32"));
        groupBox_SelectedCell = new QGroupBox(page_PopCellSettings);
        groupBox_SelectedCell->setObjectName(QString::fromUtf8("groupBox_SelectedCell"));
        verticalLayout_33 = new QVBoxLayout(groupBox_SelectedCell);
        verticalLayout_33->setSpacing(6);
        verticalLayout_33->setContentsMargins(11, 11, 11, 11);
        verticalLayout_33->setObjectName(QString::fromUtf8("verticalLayout_33"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        horizontalLayout_13->setContentsMargins(-1, 0, -1, -1);
        label_13 = new QLabel(groupBox_SelectedCell);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMaximumSize(QSize(16, 16777215));
        label_13->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/settings-icon.png")));

        horizontalLayout_13->addWidget(label_13);

        label_10 = new QLabel(groupBox_SelectedCell);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font);

        horizontalLayout_13->addWidget(label_10);


        verticalLayout_33->addLayout(horizontalLayout_13);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        frame_10 = new QFrame(groupBox_SelectedCell);
        frame_10->setObjectName(QString::fromUtf8("frame_10"));
        frame_10->setMaximumSize(QSize(16777215, 16777215));
        frame_10->setFrameShape(QFrame::StyledPanel);
        frame_10->setFrameShadow(QFrame::Raised);
        verticalLayout_24 = new QVBoxLayout(frame_10);
        verticalLayout_24->setSpacing(3);
        verticalLayout_24->setContentsMargins(3, 3, 3, 3);
        verticalLayout_24->setObjectName(QString::fromUtf8("verticalLayout_24"));
        toolButton_Pop_AddItem = new QToolButton(frame_10);
        toolButton_Pop_AddItem->setObjectName(QString::fromUtf8("toolButton_Pop_AddItem"));
        toolButton_Pop_AddItem->setIcon(icon21);

        verticalLayout_24->addWidget(toolButton_Pop_AddItem);

        toolButton_27 = new QToolButton(frame_10);
        toolButton_27->setObjectName(QString::fromUtf8("toolButton_27"));
        toolButton_27->setIcon(icon22);

        verticalLayout_24->addWidget(toolButton_27);

        toolButton_PopCellItemUp = new QToolButton(frame_10);
        toolButton_PopCellItemUp->setObjectName(QString::fromUtf8("toolButton_PopCellItemUp"));
        toolButton_PopCellItemUp->setIcon(icon23);

        verticalLayout_24->addWidget(toolButton_PopCellItemUp);

        toolButton_PopCellItemDown = new QToolButton(frame_10);
        toolButton_PopCellItemDown->setObjectName(QString::fromUtf8("toolButton_PopCellItemDown"));
        toolButton_PopCellItemDown->setIcon(icon24);

        verticalLayout_24->addWidget(toolButton_PopCellItemDown);

        frame_17 = new QFrame(frame_10);
        frame_17->setObjectName(QString::fromUtf8("frame_17"));
        frame_17->setFrameShape(QFrame::HLine);
        frame_17->setFrameShadow(QFrame::Sunken);

        verticalLayout_24->addWidget(frame_17);

        toolButton_28 = new QToolButton(frame_10);
        toolButton_28->setObjectName(QString::fromUtf8("toolButton_28"));
        QIcon icon31;
        icon31.addFile(QString::fromUtf8(":/new/prefix1/delete-clear-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_28->setIcon(icon31);

        verticalLayout_24->addWidget(toolButton_28);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_24->addItem(verticalSpacer_3);


        horizontalLayout_3->addWidget(frame_10);

        frame_SelectedCell = new QFrame(groupBox_SelectedCell);
        frame_SelectedCell->setObjectName(QString::fromUtf8("frame_SelectedCell"));
        frame_SelectedCell->setFrameShape(QFrame::StyledPanel);
        frame_SelectedCell->setFrameShadow(QFrame::Raised);
        verticalLayout_27 = new QVBoxLayout(frame_SelectedCell);
        verticalLayout_27->setSpacing(6);
        verticalLayout_27->setContentsMargins(11, 11, 11, 11);
        verticalLayout_27->setObjectName(QString::fromUtf8("verticalLayout_27"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_6 = new QLabel(frame_SelectedCell);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMaximumSize(QSize(150, 16777215));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        frame_ItemSettings = new QFrame(frame_SelectedCell);
        frame_ItemSettings->setObjectName(QString::fromUtf8("frame_ItemSettings"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_ItemSettings->sizePolicy().hasHeightForWidth());
        frame_ItemSettings->setSizePolicy(sizePolicy1);
        frame_ItemSettings->setFrameShape(QFrame::StyledPanel);
        frame_ItemSettings->setFrameShadow(QFrame::Raised);
        verticalLayout_29 = new QVBoxLayout(frame_ItemSettings);
        verticalLayout_29->setSpacing(6);
        verticalLayout_29->setContentsMargins(11, 11, 11, 11);
        verticalLayout_29->setObjectName(QString::fromUtf8("verticalLayout_29"));
        gridLayout_ItemSettings = new QGridLayout();
        gridLayout_ItemSettings->setSpacing(6);
        gridLayout_ItemSettings->setObjectName(QString::fromUtf8("gridLayout_ItemSettings"));
        gridLayout_ItemSettings->setContentsMargins(-1, 0, -1, -1);
        label_8 = new QLabel(frame_ItemSettings);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_ItemSettings->addWidget(label_8, 0, 0, 1, 1);

        comboBox_3 = new QComboBox(frame_ItemSettings);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        gridLayout_ItemSettings->addWidget(comboBox_3, 0, 1, 1, 1);


        verticalLayout_29->addLayout(gridLayout_ItemSettings);

        horizontalSpacer_7 = new QSpacerItem(40, 5, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_29->addItem(horizontalSpacer_7);

        verticalSpacer_11 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_29->addItem(verticalSpacer_11);


        gridLayout_3->addWidget(frame_ItemSettings, 1, 1, 1, 1);

        label_7 = new QLabel(frame_SelectedCell);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 0, 1, 1, 1);

        listWidget_ItemList = new PopQListWidget(frame_SelectedCell);
        listWidget_ItemList->setObjectName(QString::fromUtf8("listWidget_ItemList"));
        sizePolicy1.setHeightForWidth(listWidget_ItemList->sizePolicy().hasHeightForWidth());
        listWidget_ItemList->setSizePolicy(sizePolicy1);
        listWidget_ItemList->setMaximumSize(QSize(150, 16777215));
        listWidget_ItemList->setMouseTracking(true);
        listWidget_ItemList->setAcceptDrops(true);
        listWidget_ItemList->setDragEnabled(true);
        listWidget_ItemList->setDragDropOverwriteMode(true);
        listWidget_ItemList->setDragDropMode(QAbstractItemView::InternalMove);
        listWidget_ItemList->setDefaultDropAction(Qt::MoveAction);
        listWidget_ItemList->setAlternatingRowColors(true);
        listWidget_ItemList->setSelectionMode(QAbstractItemView::SingleSelection);
        listWidget_ItemList->setMovement(QListView::Snap);

        gridLayout_3->addWidget(listWidget_ItemList, 1, 0, 1, 1);


        verticalLayout_27->addLayout(gridLayout_3);

        verticalLayout_25 = new QVBoxLayout();
        verticalLayout_25->setSpacing(6);
        verticalLayout_25->setObjectName(QString::fromUtf8("verticalLayout_25"));
        verticalLayout_25->setContentsMargins(-1, 3, -1, -1);
        label_11 = new QLabel(frame_SelectedCell);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        QFont font1;
        font1.setBold(false);
        font1.setItalic(true);
        font1.setWeight(50);
        label_11->setFont(font1);

        verticalLayout_25->addWidget(label_11);

        plainTextEdit_ItemDesc = new QPlainTextEdit(frame_SelectedCell);
        plainTextEdit_ItemDesc->setObjectName(QString::fromUtf8("plainTextEdit_ItemDesc"));
        plainTextEdit_ItemDesc->setMaximumSize(QSize(16777215, 50));
        QFont font2;
        font2.setPointSize(10);
        plainTextEdit_ItemDesc->setFont(font2);
        plainTextEdit_ItemDesc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout_25->addWidget(plainTextEdit_ItemDesc);

        verticalSpacer_17 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_25->addItem(verticalSpacer_17);


        verticalLayout_27->addLayout(verticalLayout_25);


        horizontalLayout_3->addWidget(frame_SelectedCell);


        verticalLayout_33->addLayout(horizontalLayout_3);


        verticalLayout_32->addWidget(groupBox_SelectedCell);

        stackedWidget_Pop->addWidget(page_PopCellSettings);
        page_PopDescription = new QWidget();
        page_PopDescription->setObjectName(QString::fromUtf8("page_PopDescription"));
        verticalLayout_26 = new QVBoxLayout(page_PopDescription);
        verticalLayout_26->setSpacing(6);
        verticalLayout_26->setContentsMargins(3, 3, 3, 3);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        groupBox_PopDescription = new QGroupBox(page_PopDescription);
        groupBox_PopDescription->setObjectName(QString::fromUtf8("groupBox_PopDescription"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_PopDescription->sizePolicy().hasHeightForWidth());
        groupBox_PopDescription->setSizePolicy(sizePolicy2);
        groupBox_PopDescription->setMaximumSize(QSize(500, 16777215));
        verticalLayout_34 = new QVBoxLayout(groupBox_PopDescription);
        verticalLayout_34->setSpacing(6);
        verticalLayout_34->setContentsMargins(11, 11, 11, 11);
        verticalLayout_34->setObjectName(QString::fromUtf8("verticalLayout_34"));
        verticalLayout_28 = new QVBoxLayout();
        verticalLayout_28->setSpacing(6);
        verticalLayout_28->setObjectName(QString::fromUtf8("verticalLayout_28"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(-1, 0, -1, -1);
        label_12 = new QLabel(groupBox_PopDescription);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMaximumSize(QSize(16, 16777215));
        label_12->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/description-icon.png")));
        label_12->setScaledContents(false);

        horizontalLayout_11->addWidget(label_12);

        label_PopDescription = new QLabel(groupBox_PopDescription);
        label_PopDescription->setObjectName(QString::fromUtf8("label_PopDescription"));
        label_PopDescription->setFont(font);

        horizontalLayout_11->addWidget(label_PopDescription);


        verticalLayout_28->addLayout(horizontalLayout_11);

        plainText_Edit_PopDescription = new QPlainTextEdit(groupBox_PopDescription);
        plainText_Edit_PopDescription->setObjectName(QString::fromUtf8("plainText_Edit_PopDescription"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(plainText_Edit_PopDescription->sizePolicy().hasHeightForWidth());
        plainText_Edit_PopDescription->setSizePolicy(sizePolicy3);
        plainText_Edit_PopDescription->setMaximumSize(QSize(16777215, 200));

        verticalLayout_28->addWidget(plainText_Edit_PopDescription);


        verticalLayout_34->addLayout(verticalLayout_28);

        verticalSpacer_12 = new QSpacerItem(20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_34->addItem(verticalSpacer_12);


        verticalLayout_26->addWidget(groupBox_PopDescription);

        stackedWidget_Pop->addWidget(page_PopDescription);

        horizontalLayout_12->addWidget(stackedWidget_Pop);


        verticalLayout_13->addLayout(horizontalLayout_12);

        QIcon icon32;
        icon32.addFile(QString::fromUtf8(":/new/prefix1/virus-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Top->addTab(tab_Populations, icon32, QString());
        tab_Network = new QWidget();
        tab_Network->setObjectName(QString::fromUtf8("tab_Network"));
        horizontalLayout_14 = new QHBoxLayout(tab_Network);
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(9, -1, -1, -1);
        groupBox = new QGroupBox(tab_Network);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, -1, -1, -1);
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_NetworkNbNode = new QLabel(frame);
        label_NetworkNbNode->setObjectName(QString::fromUtf8("label_NetworkNbNode"));

        gridLayout->addWidget(label_NetworkNbNode, 3, 0, 1, 1);

        spinBox_NetworkNbNodes = new QSpinBox(frame);
        spinBox_NetworkNbNodes->setObjectName(QString::fromUtf8("spinBox_NetworkNbNodes"));
        QFont font3;
        font3.setStrikeOut(false);
        spinBox_NetworkNbNodes->setFont(font3);
        spinBox_NetworkNbNodes->setMaximum(16777215);
        spinBox_NetworkNbNodes->setSingleStep(100);

        gridLayout->addWidget(spinBox_NetworkNbNodes, 3, 1, 1, 1);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(-1, 5, -1, -1);
        label_NetworkDist = new QLabel(frame);
        label_NetworkDist->setObjectName(QString::fromUtf8("label_NetworkDist"));

        horizontalLayout_15->addWidget(label_NetworkDist);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_15, 1, 0, 1, 1);

        comboBox_NetworkDist = new QComboBox(frame);
        comboBox_NetworkDist->setObjectName(QString::fromUtf8("comboBox_NetworkDist"));

        gridLayout->addWidget(comboBox_NetworkDist, 1, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout);

        frame_3 = new QFrame(frame);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::HLine);
        frame_3->setFrameShadow(QFrame::Raised);

        verticalLayout_6->addWidget(frame_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font4;
        font4.setPointSize(8);
        font4.setBold(true);
        font4.setItalic(true);
        font4.setWeight(75);
        label_3->setFont(font4);
        label_3->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        horizontalLayout->addWidget(label_3);

        toolButton_NetworkResetDist = new QToolButton(frame);
        toolButton_NetworkResetDist->setObjectName(QString::fromUtf8("toolButton_NetworkResetDist"));
        toolButton_NetworkResetDist->setMaximumSize(QSize(24, 24));
        toolButton_NetworkResetDist->setFont(font);
        toolButton_NetworkResetDist->setIcon(icon20);
        toolButton_NetworkResetDist->setPopupMode(QToolButton::InstantPopup);

        horizontalLayout->addWidget(toolButton_NetworkResetDist);


        verticalLayout_6->addLayout(horizontalLayout);

        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_10 = new QVBoxLayout(groupBox_3);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 9, 0, 0);
        frame_NetworkParams = new QFrame(groupBox_3);
        frame_NetworkParams->setObjectName(QString::fromUtf8("frame_NetworkParams"));
        frame_NetworkParams->setFrameShape(QFrame::StyledPanel);
        frame_NetworkParams->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_NetworkParams);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));

        verticalLayout_10->addWidget(frame_NetworkParams);


        verticalLayout_6->addWidget(groupBox_3);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_8);


        verticalLayout_4->addWidget(frame);


        horizontalLayout_14->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_Network);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_8 = new QVBoxLayout(groupBox_2);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, -1, -1, -1);
        frame_2 = new QFrame(groupBox_2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_7 = new QVBoxLayout(frame_2);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(9, -1, -1, -1);
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        horizontalLayout_17->setContentsMargins(-1, 0, -1, -1);
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_17->addWidget(label_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_6);

        comboBox_NetPopsSpread = new QComboBox(frame_2);
        QIcon icon33;
        icon33.addFile(QString::fromUtf8(":/new/prefix1/spray-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        comboBox_NetPopsSpread->addItem(icon33, QString());
        QIcon icon34;
        icon34.addFile(QString::fromUtf8(":/new/prefix1/pin-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        comboBox_NetPopsSpread->addItem(icon34, QString());
        comboBox_NetPopsSpread->setObjectName(QString::fromUtf8("comboBox_NetPopsSpread"));
        comboBox_NetPopsSpread->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_17->addWidget(comboBox_NetPopsSpread);


        verticalLayout_7->addLayout(horizontalLayout_17);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QString::fromUtf8("horizontalLayout_22"));
        tableWidget_NetPopsRepartition = new QTableWidget(frame_2);
        if (tableWidget_NetPopsRepartition->columnCount() < 2)
            tableWidget_NetPopsRepartition->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_NetPopsRepartition->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_NetPopsRepartition->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        tableWidget_NetPopsRepartition->setObjectName(QString::fromUtf8("tableWidget_NetPopsRepartition"));
        tableWidget_NetPopsRepartition->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_NetPopsRepartition->horizontalHeader()->setDefaultSectionSize(150);
        tableWidget_NetPopsRepartition->horizontalHeader()->setMinimumSectionSize(150);

        horizontalLayout_22->addWidget(tableWidget_NetPopsRepartition);


        verticalLayout_7->addLayout(horizontalLayout_22);

        frame_NetPopTotalInfo = new QFrame(frame_2);
        frame_NetPopTotalInfo->setObjectName(QString::fromUtf8("frame_NetPopTotalInfo"));
        frame_NetPopTotalInfo->setFrameShape(QFrame::StyledPanel);
        frame_NetPopTotalInfo->setFrameShadow(QFrame::Raised);
        horizontalLayout_23 = new QHBoxLayout(frame_NetPopTotalInfo);
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_23->setObjectName(QString::fromUtf8("horizontalLayout_23"));
        label_NetPopIcon_2 = new QLabel(frame_NetPopTotalInfo);
        label_NetPopIcon_2->setObjectName(QString::fromUtf8("label_NetPopIcon_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_NetPopIcon_2->sizePolicy().hasHeightForWidth());
        label_NetPopIcon_2->setSizePolicy(sizePolicy4);
        label_NetPopIcon_2->setBaseSize(QSize(24, 24));
        label_NetPopIcon_2->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/warning.png")));

        horizontalLayout_23->addWidget(label_NetPopIcon_2);

        textEdit_NetPopTotalInfo = new QTextEdit(frame_NetPopTotalInfo);
        textEdit_NetPopTotalInfo->setObjectName(QString::fromUtf8("textEdit_NetPopTotalInfo"));
        QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(textEdit_NetPopTotalInfo->sizePolicy().hasHeightForWidth());
        textEdit_NetPopTotalInfo->setSizePolicy(sizePolicy5);
        textEdit_NetPopTotalInfo->setMaximumSize(QSize(16777215, 28));
        textEdit_NetPopTotalInfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        horizontalLayout_23->addWidget(textEdit_NetPopTotalInfo);


        verticalLayout_7->addWidget(frame_NetPopTotalInfo);

        frame_22 = new QFrame(frame_2);
        frame_22->setObjectName(QString::fromUtf8("frame_22"));
        frame_22->setFrameShape(QFrame::StyledPanel);
        frame_22->setFrameShadow(QFrame::Raised);
        horizontalLayout_21 = new QHBoxLayout(frame_22);
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setContentsMargins(3, 3, 3, 3);
        horizontalLayout_21->setObjectName(QString::fromUtf8("horizontalLayout_21"));
        label_18 = new QLabel(frame_22);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy6);
        QFont font5;
        font5.setBold(true);
        font5.setItalic(true);
        font5.setWeight(75);
        label_18->setFont(font5);

        horizontalLayout_21->addWidget(label_18);

        label_NetPopIcon = new QLabel(frame_22);
        label_NetPopIcon->setObjectName(QString::fromUtf8("label_NetPopIcon"));
        sizePolicy4.setHeightForWidth(label_NetPopIcon->sizePolicy().hasHeightForWidth());
        label_NetPopIcon->setSizePolicy(sizePolicy4);
        label_NetPopIcon->setBaseSize(QSize(24, 24));
        label_NetPopIcon->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/warning24.png")));

        horizontalLayout_21->addWidget(label_NetPopIcon);

        textEdit_SpreadTotal = new QTextEdit(frame_22);
        textEdit_SpreadTotal->setObjectName(QString::fromUtf8("textEdit_SpreadTotal"));
        sizePolicy5.setHeightForWidth(textEdit_SpreadTotal->sizePolicy().hasHeightForWidth());
        textEdit_SpreadTotal->setSizePolicy(sizePolicy5);
        textEdit_SpreadTotal->setMaximumSize(QSize(16777215, 28));
        textEdit_SpreadTotal->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit_SpreadTotal->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit_SpreadTotal->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        horizontalLayout_21->addWidget(textEdit_SpreadTotal);


        verticalLayout_7->addWidget(frame_22);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_7->addItem(verticalSpacer);

        horizontalSpacer_5 = new QSpacerItem(40, 4, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout_7->addItem(horizontalSpacer_5);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        horizontalLayout_16->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_3);

        pushButton_NetGenerate = new QPushButton(frame_2);
        pushButton_NetGenerate->setObjectName(QString::fromUtf8("pushButton_NetGenerate"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(pushButton_NetGenerate->sizePolicy().hasHeightForWidth());
        pushButton_NetGenerate->setSizePolicy(sizePolicy7);
        pushButton_NetGenerate->setMaximumSize(QSize(16777215, 30));
        QIcon icon35;
        icon35.addFile(QString::fromUtf8(":/new/prefix1/Net-mesh-icon24.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_NetGenerate->setIcon(icon35);
        pushButton_NetGenerate->setIconSize(QSize(24, 24));

        horizontalLayout_16->addWidget(pushButton_NetGenerate);


        verticalLayout_7->addLayout(horizontalLayout_16);


        verticalLayout_8->addWidget(frame_2);


        horizontalLayout_14->addWidget(groupBox_2);

        QIcon icon36;
        icon36.addFile(QString::fromUtf8(":/new/prefix1/Net-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Top->addTab(tab_Network, icon36, QString());
        tab_Variables = new QWidget();
        tab_Variables->setObjectName(QString::fromUtf8("tab_Variables"));
        verticalLayout_11 = new QVBoxLayout(tab_Variables);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        frame_11 = new QFrame(tab_Variables);
        frame_11->setObjectName(QString::fromUtf8("frame_11"));
        frame_11->setFrameShape(QFrame::StyledPanel);
        frame_11->setFrameShadow(QFrame::Raised);
        verticalLayout_36 = new QVBoxLayout(frame_11);
        verticalLayout_36->setSpacing(3);
        verticalLayout_36->setContentsMargins(3, 3, 3, 3);
        verticalLayout_36->setObjectName(QString::fromUtf8("verticalLayout_36"));
        label_30 = new QLabel(frame_11);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setFont(font);

        verticalLayout_36->addWidget(label_30);

        toolButton_VarAdd = new QToolButton(frame_11);
        toolButton_VarAdd->setObjectName(QString::fromUtf8("toolButton_VarAdd"));
        toolButton_VarAdd->setIcon(icon21);

        verticalLayout_36->addWidget(toolButton_VarAdd);

        toolButton_VarRemove = new QToolButton(frame_11);
        toolButton_VarRemove->setObjectName(QString::fromUtf8("toolButton_VarRemove"));
        toolButton_VarRemove->setIcon(icon22);

        verticalLayout_36->addWidget(toolButton_VarRemove);

        toolButton_VarRowUp = new QToolButton(frame_11);
        toolButton_VarRowUp->setObjectName(QString::fromUtf8("toolButton_VarRowUp"));
        toolButton_VarRowUp->setIcon(icon23);

        verticalLayout_36->addWidget(toolButton_VarRowUp);

        toolButton_VarRowDown = new QToolButton(frame_11);
        toolButton_VarRowDown->setObjectName(QString::fromUtf8("toolButton_VarRowDown"));
        toolButton_VarRowDown->setIcon(icon24);

        verticalLayout_36->addWidget(toolButton_VarRowDown);

        verticalSpacer_16 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_36->addItem(verticalSpacer_16);


        horizontalLayout_2->addWidget(frame_11);

        groupBox_6 = new QGroupBox(tab_Variables);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        verticalLayout_21 = new QVBoxLayout(groupBox_6);
        verticalLayout_21->setSpacing(6);
        verticalLayout_21->setContentsMargins(11, 11, 11, 11);
        verticalLayout_21->setObjectName(QString::fromUtf8("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(0, -1, -1, -1);
        tableWidget_Variables = new QTableWidget(groupBox_6);
        if (tableWidget_Variables->columnCount() < 2)
            tableWidget_Variables->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_Variables->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_Variables->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        tableWidget_Variables->setObjectName(QString::fromUtf8("tableWidget_Variables"));
        tableWidget_Variables->setMinimumSize(QSize(502, 0));
        tableWidget_Variables->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_Variables->horizontalHeader()->setDefaultSectionSize(200);
        tableWidget_Variables->horizontalHeader()->setMinimumSectionSize(200);
        tableWidget_Variables->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_21->addWidget(tableWidget_Variables);

        label_29 = new QLabel(groupBox_6);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        QFont font6;
        font6.setItalic(true);
        label_29->setFont(font6);

        verticalLayout_21->addWidget(label_29);

        plainTextEdit_VarDesc = new QPlainTextEdit(groupBox_6);
        plainTextEdit_VarDesc->setObjectName(QString::fromUtf8("plainTextEdit_VarDesc"));
        plainTextEdit_VarDesc->setMaximumSize(QSize(16777215, 70));
        plainTextEdit_VarDesc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        verticalLayout_21->addWidget(plainTextEdit_VarDesc);


        horizontalLayout_2->addWidget(groupBox_6);


        horizontalLayout_9->addLayout(horizontalLayout_2);

        groupBox_VarFuncTrashed = new QGroupBox(tab_Variables);
        groupBox_VarFuncTrashed->setObjectName(QString::fromUtf8("groupBox_VarFuncTrashed"));
        horizontalLayout_10 = new QHBoxLayout(groupBox_VarFuncTrashed);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, -1, -1, -1);
        frame_9 = new QFrame(groupBox_VarFuncTrashed);
        frame_9->setObjectName(QString::fromUtf8("frame_9"));
        sizePolicy6.setHeightForWidth(frame_9->sizePolicy().hasHeightForWidth());
        frame_9->setSizePolicy(sizePolicy6);
        frame_9->setMaximumSize(QSize(210, 16777215));
        frame_9->setFrameShape(QFrame::StyledPanel);
        frame_9->setFrameShadow(QFrame::Raised);
        verticalLayout_22 = new QVBoxLayout(frame_9);
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setContentsMargins(11, 11, 11, 11);
        verticalLayout_22->setObjectName(QString::fromUtf8("verticalLayout_22"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        toolButton_10 = new QToolButton(frame_9);
        toolButton_10->setObjectName(QString::fromUtf8("toolButton_10"));
        QIcon icon37;
        icon37.addFile(QString::fromUtf8(":/new/prefix1/new-icon3.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_10->setIcon(icon37);
        toolButton_10->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout_5->addWidget(toolButton_10, 0, 2, 1, 1);

        label_15 = new QLabel(frame_9);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(25, 0));
        label_15->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/variable-B-icon.png")));

        gridLayout_5->addWidget(label_15, 0, 0, 1, 1);

        comboBox_4 = new QComboBox(frame_9);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));
        comboBox_4->setMinimumSize(QSize(120, 0));
        comboBox_4->setMaximumSize(QSize(120, 16777215));

        gridLayout_5->addWidget(comboBox_4, 0, 1, 1, 1);

        toolButton_11 = new QToolButton(frame_9);
        toolButton_11->setObjectName(QString::fromUtf8("toolButton_11"));
        toolButton_11->setIcon(icon37);

        gridLayout_5->addWidget(toolButton_11, 1, 2, 1, 1);

        comboBox_2 = new QComboBox(frame_9);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        sizePolicy4.setHeightForWidth(comboBox_2->sizePolicy().hasHeightForWidth());
        comboBox_2->setSizePolicy(sizePolicy4);
        comboBox_2->setMinimumSize(QSize(120, 0));
        comboBox_2->setMaximumSize(QSize(120, 16777215));

        gridLayout_5->addWidget(comboBox_2, 1, 1, 1, 1);

        label_16 = new QLabel(frame_9);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(0, 0));
        label_16->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/function-icon3.png")));

        gridLayout_5->addWidget(label_16, 1, 0, 1, 1);


        verticalLayout_22->addLayout(gridLayout_5);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_22->addItem(verticalSpacer_7);


        horizontalLayout_10->addWidget(frame_9);

        plainTextEdit = new QPlainTextEdit(groupBox_VarFuncTrashed);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        horizontalLayout_10->addWidget(plainTextEdit);


        horizontalLayout_9->addWidget(groupBox_VarFuncTrashed);


        verticalLayout_11->addLayout(horizontalLayout_9);

        QIcon icon38;
        icon38.addFile(QString::fromUtf8(":/new/prefix1/variable-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Top->addTab(tab_Variables, icon38, QString());
        tab_LifeCycle = new QWidget();
        tab_LifeCycle->setObjectName(QString::fromUtf8("tab_LifeCycle"));
        horizontalLayout_7 = new QHBoxLayout(tab_LifeCycle);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_19 = new QFrame(tab_LifeCycle);
        frame_19->setObjectName(QString::fromUtf8("frame_19"));
        frame_19->setMinimumSize(QSize(0, 10));
        frame_19->setFrameShape(QFrame::StyledPanel);
        frame_19->setFrameShadow(QFrame::Raised);
        verticalLayout_35 = new QVBoxLayout(frame_19);
        verticalLayout_35->setSpacing(3);
        verticalLayout_35->setContentsMargins(3, 3, 3, 3);
        verticalLayout_35->setObjectName(QString::fromUtf8("verticalLayout_35"));
        label_14 = new QLabel(frame_19);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);

        verticalLayout_35->addWidget(label_14);

        toolButton_6 = new QToolButton(frame_19);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));
        toolButton_6->setIcon(icon21);

        verticalLayout_35->addWidget(toolButton_6);

        toolButton_7 = new QToolButton(frame_19);
        toolButton_7->setObjectName(QString::fromUtf8("toolButton_7"));
        toolButton_7->setIcon(icon22);

        verticalLayout_35->addWidget(toolButton_7);

        toolButton_31 = new QToolButton(frame_19);
        toolButton_31->setObjectName(QString::fromUtf8("toolButton_31"));
        toolButton_31->setIcon(icon23);

        verticalLayout_35->addWidget(toolButton_31);

        toolButton_32 = new QToolButton(frame_19);
        toolButton_32->setObjectName(QString::fromUtf8("toolButton_32"));
        toolButton_32->setIcon(icon24);

        verticalLayout_35->addWidget(toolButton_32);

        frame_20 = new QFrame(frame_19);
        frame_20->setObjectName(QString::fromUtf8("frame_20"));
        frame_20->setFrameShape(QFrame::HLine);
        frame_20->setFrameShadow(QFrame::Sunken);

        verticalLayout_35->addWidget(frame_20);

        toolButton_33 = new QToolButton(frame_19);
        toolButton_33->setObjectName(QString::fromUtf8("toolButton_33"));
        toolButton_33->setIcon(icon25);

        verticalLayout_35->addWidget(toolButton_33);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_35->addItem(verticalSpacer_2);


        verticalLayout_3->addWidget(frame_19);

        frame_7 = new QFrame(tab_LifeCycle);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        QSizePolicy sizePolicy8(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(frame_7->sizePolicy().hasHeightForWidth());
        frame_7->setSizePolicy(sizePolicy8);
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_30 = new QVBoxLayout(frame_7);
        verticalLayout_30->setSpacing(3);
        verticalLayout_30->setContentsMargins(3, 3, 3, 3);
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        label_17 = new QLabel(frame_7);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setFont(font);

        verticalLayout_30->addWidget(label_17);

        toolButton_34 = new QToolButton(frame_7);
        toolButton_34->setObjectName(QString::fromUtf8("toolButton_34"));
        toolButton_34->setIcon(icon27);
        toolButton_34->setCheckable(true);

        verticalLayout_30->addWidget(toolButton_34);

        frame_21 = new QFrame(frame_7);
        frame_21->setObjectName(QString::fromUtf8("frame_21"));
        frame_21->setFrameShape(QFrame::HLine);
        frame_21->setFrameShadow(QFrame::Sunken);

        verticalLayout_30->addWidget(frame_21);

        toolButton_16 = new QToolButton(frame_7);
        toolButton_16->setObjectName(QString::fromUtf8("toolButton_16"));
        toolButton_16->setIcon(icon28);

        verticalLayout_30->addWidget(toolButton_16);

        toolButton_35 = new QToolButton(frame_7);
        toolButton_35->setObjectName(QString::fromUtf8("toolButton_35"));
        toolButton_35->setIcon(icon29);

        verticalLayout_30->addWidget(toolButton_35);

        toolButton_14 = new QToolButton(frame_7);
        toolButton_14->setObjectName(QString::fromUtf8("toolButton_14"));
        toolButton_14->setIcon(icon30);

        verticalLayout_30->addWidget(toolButton_14);

        verticalSpacer_13 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_30->addItem(verticalSpacer_13);


        verticalLayout_3->addWidget(frame_7);


        horizontalLayout_7->addLayout(verticalLayout_3);

        verticalLayout_37 = new QVBoxLayout();
        verticalLayout_37->setSpacing(6);
        verticalLayout_37->setObjectName(QString::fromUtf8("verticalLayout_37"));
        verticalLayout_37->setContentsMargins(0, -1, -1, -1);
        tableWidget_LifeCycle = new QTableWidget(tab_LifeCycle);
        if (tableWidget_LifeCycle->columnCount() < 5)
            tableWidget_LifeCycle->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_LifeCycle->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_LifeCycle->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_LifeCycle->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_LifeCycle->setHorizontalHeaderItem(3, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_LifeCycle->setHorizontalHeaderItem(4, __qtablewidgetitem12);
        if (tableWidget_LifeCycle->rowCount() < 2)
            tableWidget_LifeCycle->setRowCount(2);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget_LifeCycle->setVerticalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget_LifeCycle->setVerticalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget_LifeCycle->setItem(0, 0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setFont(font5);
        tableWidget_LifeCycle->setItem(0, 1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget_LifeCycle->setItem(0, 2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget_LifeCycle->setItem(0, 3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget_LifeCycle->setItem(0, 4, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget_LifeCycle->setItem(1, 0, __qtablewidgetitem20);
        tableWidget_LifeCycle->setObjectName(QString::fromUtf8("tableWidget_LifeCycle"));
        tableWidget_LifeCycle->setMinimumSize(QSize(500, 0));
        tableWidget_LifeCycle->setMaximumSize(QSize(602, 16777215));
        tableWidget_LifeCycle->setDragEnabled(true);
        tableWidget_LifeCycle->setDragDropMode(QAbstractItemView::InternalMove);
        tableWidget_LifeCycle->setDefaultDropAction(Qt::MoveAction);
        tableWidget_LifeCycle->setAlternatingRowColors(true);
        tableWidget_LifeCycle->setTextElideMode(Qt::ElideNone);
        tableWidget_LifeCycle->horizontalHeader()->setDefaultSectionSize(102);
        tableWidget_LifeCycle->horizontalHeader()->setMinimumSectionSize(102);
        tableWidget_LifeCycle->horizontalHeader()->setStretchLastSection(true);

        verticalLayout_37->addWidget(tableWidget_LifeCycle);

        frame_27 = new QFrame(tab_LifeCycle);
        frame_27->setObjectName(QString::fromUtf8("frame_27"));
        frame_27->setMinimumSize(QSize(0, 50));
        frame_27->setFrameShape(QFrame::StyledPanel);
        frame_27->setFrameShadow(QFrame::Raised);

        verticalLayout_37->addWidget(frame_27);

        label_21 = new QLabel(tab_LifeCycle);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setFont(font6);

        verticalLayout_37->addWidget(label_21);

        plainTextEdit_3 = new QPlainTextEdit(tab_LifeCycle);
        plainTextEdit_3->setObjectName(QString::fromUtf8("plainTextEdit_3"));
        plainTextEdit_3->setMaximumSize(QSize(16777215, 70));

        verticalLayout_37->addWidget(plainTextEdit_3);


        horizontalLayout_7->addLayout(verticalLayout_37);

        groupBox_VarFuncCreator = new QGroupBox(tab_LifeCycle);
        groupBox_VarFuncCreator->setObjectName(QString::fromUtf8("groupBox_VarFuncCreator"));
        gridLayout_8 = new QGridLayout(groupBox_VarFuncCreator);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, -1, 0, 0);
        toolBox_CodeEditor = new QToolBox(groupBox_VarFuncCreator);
        toolBox_CodeEditor->setObjectName(QString::fromUtf8("toolBox_CodeEditor"));
        QFont font7;
        font7.setFamily(QString::fromUtf8("Monospace"));
        font7.setPointSize(10);
        font7.setBold(true);
        font7.setItalic(false);
        font7.setWeight(75);
        toolBox_CodeEditor->setFont(font7);
        CurrentFormulaEditing = new QWidget();
        CurrentFormulaEditing->setObjectName(QString::fromUtf8("CurrentFormulaEditing"));
        CurrentFormulaEditing->setGeometry(QRect(0, 0, 300, 452));
        horizontalLayout_26 = new QHBoxLayout(CurrentFormulaEditing);
        horizontalLayout_26->setSpacing(0);
        horizontalLayout_26->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        textEdit_Formula = new QTextEditCodeEditor(CurrentFormulaEditing);
        textEdit_Formula->setObjectName(QString::fromUtf8("textEdit_Formula"));
        textEdit_Formula->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        horizontalLayout_26->addWidget(textEdit_Formula);

        QIcon icon39;
        icon39.addFile(QString::fromUtf8(":/new/prefix1/puce.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolBox_CodeEditor->addItem(CurrentFormulaEditing, icon39, QString::fromUtf8("Current Formula Editing"));
        CustomMacroEditing = new QWidget();
        CustomMacroEditing->setObjectName(QString::fromUtf8("CustomMacroEditing"));
        CustomMacroEditing->setGeometry(QRect(0, 0, 85, 84));
        horizontalLayout_27 = new QHBoxLayout(CustomMacroEditing);
        horizontalLayout_27->setSpacing(0);
        horizontalLayout_27->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        textEdit_CustomMacro = new QTextEditCodeEditor(CustomMacroEditing);
        textEdit_CustomMacro->setObjectName(QString::fromUtf8("textEdit_CustomMacro"));
        textEdit_CustomMacro->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        horizontalLayout_27->addWidget(textEdit_CustomMacro);

        toolBox_CodeEditor->addItem(CustomMacroEditing, icon39, QString::fromUtf8("Custom MACRO Editing"));

        gridLayout_8->addWidget(toolBox_CodeEditor, 2, 1, 1, 1);

        frame_13 = new QFrame(groupBox_VarFuncCreator);
        frame_13->setObjectName(QString::fromUtf8("frame_13"));
        sizePolicy6.setHeightForWidth(frame_13->sizePolicy().hasHeightForWidth());
        frame_13->setSizePolicy(sizePolicy6);
        frame_13->setMaximumSize(QSize(210, 16777215));
        frame_13->setFrameShape(QFrame::StyledPanel);
        frame_13->setFrameShadow(QFrame::Raised);
        verticalLayout_31 = new QVBoxLayout(frame_13);
        verticalLayout_31->setSpacing(6);
        verticalLayout_31->setContentsMargins(6, 6, 6, 6);
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QString::fromUtf8("horizontalLayout_19"));
        label_27 = new QLabel(frame_13);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setMaximumSize(QSize(16, 16));
        label_27->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/macro-icon.png")));

        horizontalLayout_19->addWidget(label_27);

        label_22 = new QLabel(frame_13);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setFont(font);

        horizontalLayout_19->addWidget(label_22);


        verticalLayout_31->addLayout(horizontalLayout_19);

        frame_AvailableMacros = new QFrame(frame_13);
        frame_AvailableMacros->setObjectName(QString::fromUtf8("frame_AvailableMacros"));
        frame_AvailableMacros->setFrameShape(QFrame::StyledPanel);
        frame_AvailableMacros->setFrameShadow(QFrame::Raised);
        horizontalLayout_25 = new QHBoxLayout(frame_AvailableMacros);
        horizontalLayout_25->setSpacing(0);
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        gridLayout_AvailableMacros = new QGridLayout();
        gridLayout_AvailableMacros->setSpacing(3);
        gridLayout_AvailableMacros->setContentsMargins(3, 3, 3, 3);
        gridLayout_AvailableMacros->setObjectName(QString::fromUtf8("gridLayout_AvailableMacros"));
        comboBox_ElpBuiltin = new QComboBox(frame_AvailableMacros);
        comboBox_ElpBuiltin->setObjectName(QString::fromUtf8("comboBox_ElpBuiltin"));
        sizePolicy4.setHeightForWidth(comboBox_ElpBuiltin->sizePolicy().hasHeightForWidth());
        comboBox_ElpBuiltin->setSizePolicy(sizePolicy4);
        comboBox_ElpBuiltin->setMinimumSize(QSize(130, 0));
        comboBox_ElpBuiltin->setMaximumSize(QSize(200, 26));
        comboBox_ElpBuiltin->setModelColumn(0);

        gridLayout_AvailableMacros->addWidget(comboBox_ElpBuiltin, 5, 1, 1, 1);

        toolButton_26 = new QToolButton(frame_AvailableMacros);
        toolButton_26->setObjectName(QString::fromUtf8("toolButton_26"));
        QIcon icon40;
        icon40.addFile(QString::fromUtf8(":/new/prefix1/edit-cursor-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_26->setIcon(icon40);

        gridLayout_AvailableMacros->addWidget(toolButton_26, 5, 4, 1, 1);

        label = new QLabel(frame_AvailableMacros);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/user-defined.png")));

        gridLayout_AvailableMacros->addWidget(label, 6, 0, 1, 1);

        comboBox_ElpBuiltinKeywords = new QComboBox(frame_AvailableMacros);
        comboBox_ElpBuiltinKeywords->setObjectName(QString::fromUtf8("comboBox_ElpBuiltinKeywords"));
        comboBox_ElpBuiltinKeywords->setMinimumSize(QSize(130, 0));
        comboBox_ElpBuiltinKeywords->setMaximumSize(QSize(200, 26));

        gridLayout_AvailableMacros->addWidget(comboBox_ElpBuiltinKeywords, 4, 1, 1, 1);

        label_19 = new QLabel(frame_AvailableMacros);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setMinimumSize(QSize(25, 0));
        label_19->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/key-word.png")));

        gridLayout_AvailableMacros->addWidget(label_19, 4, 0, 1, 1);

        toolButton_FormulaInsertKeyword = new QToolButton(frame_AvailableMacros);
        toolButton_FormulaInsertKeyword->setObjectName(QString::fromUtf8("toolButton_FormulaInsertKeyword"));
        toolButton_FormulaInsertKeyword->setIcon(icon40);
        toolButton_FormulaInsertKeyword->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout_AvailableMacros->addWidget(toolButton_FormulaInsertKeyword, 4, 4, 1, 1);

        toolButton_2 = new QToolButton(frame_AvailableMacros);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setIcon(icon40);

        gridLayout_AvailableMacros->addWidget(toolButton_2, 3, 4, 1, 1);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(0);
        horizontalLayout_24->setObjectName(QString::fromUtf8("horizontalLayout_24"));
        horizontalLayout_24->setContentsMargins(-1, 0, -1, -1);
        comboBox_StdMath = new QComboBox(frame_AvailableMacros);
        comboBox_StdMath->setObjectName(QString::fromUtf8("comboBox_StdMath"));
        comboBox_StdMath->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_24->addWidget(comboBox_StdMath);

        comboBox_CatStdMath = new QComboBox(frame_AvailableMacros);
        comboBox_CatStdMath->setObjectName(QString::fromUtf8("comboBox_CatStdMath"));
        comboBox_CatStdMath->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_24->addWidget(comboBox_CatStdMath);


        gridLayout_AvailableMacros->addLayout(horizontalLayout_24, 3, 1, 1, 1);

        label_31 = new QLabel(frame_AvailableMacros);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setMinimumSize(QSize(25, 0));
        label_31->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/math-library.png")));

        gridLayout_AvailableMacros->addWidget(label_31, 3, 0, 1, 1);

        label_20 = new QLabel(frame_AvailableMacros);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setMinimumSize(QSize(0, 0));
        label_20->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/function-icon3.png")));

        gridLayout_AvailableMacros->addWidget(label_20, 5, 0, 1, 1);

        toolButton_CustomMacroInsert = new QToolButton(frame_AvailableMacros);
        toolButton_CustomMacroInsert->setObjectName(QString::fromUtf8("toolButton_CustomMacroInsert"));
        toolButton_CustomMacroInsert->setIcon(icon40);

        gridLayout_AvailableMacros->addWidget(toolButton_CustomMacroInsert, 6, 4, 1, 1);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setSpacing(3);
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        horizontalLayout_28->setContentsMargins(-1, -1, 0, -1);
        toolButton_CustomMacroEdit = new QToolButton(frame_AvailableMacros);
        toolButton_CustomMacroEdit->setObjectName(QString::fromUtf8("toolButton_CustomMacroEdit"));
        QIcon icon41;
        icon41.addFile(QString::fromUtf8(":/new/prefix1/edit-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_CustomMacroEdit->setIcon(icon41);

        horizontalLayout_28->addWidget(toolButton_CustomMacroEdit);

        comboBox_CustomMacros = new QComboBox(frame_AvailableMacros);
        comboBox_CustomMacros->addItem(QString());
        comboBox_CustomMacros->addItem(QString());
        comboBox_CustomMacros->addItem(QString());
        comboBox_CustomMacros->addItem(QString());
        QIcon icon42;
        icon42.addFile(QString::fromUtf8(":/new/prefix1/new-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        comboBox_CustomMacros->addItem(icon42, QString());
        comboBox_CustomMacros->setObjectName(QString::fromUtf8("comboBox_CustomMacros"));
        comboBox_CustomMacros->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_28->addWidget(comboBox_CustomMacros);


        gridLayout_AvailableMacros->addLayout(horizontalLayout_28, 6, 1, 1, 1);


        horizontalLayout_25->addLayout(gridLayout_AvailableMacros);


        verticalLayout_31->addWidget(frame_AvailableMacros);

        verticalSpacer_6 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_31->addItem(verticalSpacer_6);

        frame_18 = new QFrame(frame_13);
        frame_18->setObjectName(QString::fromUtf8("frame_18"));
        frame_18->setMaximumSize(QSize(16777215, 3));
        frame_18->setFrameShape(QFrame::HLine);
        frame_18->setFrameShadow(QFrame::Sunken);

        verticalLayout_31->addWidget(frame_18);

        verticalSpacer_15 = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_31->addItem(verticalSpacer_15);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        horizontalLayout_18->setContentsMargins(-1, 0, -1, -1);
        label_26 = new QLabel(frame_13);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setMaximumSize(QSize(16, 16));
        label_26->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/edit-icon.png")));

        horizontalLayout_18->addWidget(label_26);

        label_25 = new QLabel(frame_13);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setFont(font);

        horizontalLayout_18->addWidget(label_25);


        verticalLayout_31->addLayout(horizontalLayout_18);

        frame_CustomMacro = new QFrame(frame_13);
        frame_CustomMacro->setObjectName(QString::fromUtf8("frame_CustomMacro"));
        frame_CustomMacro->setFrameShape(QFrame::NoFrame);
        frame_CustomMacro->setFrameShadow(QFrame::Raised);
        verticalLayout_12 = new QVBoxLayout(frame_CustomMacro);
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setContentsMargins(0, 0, 0, 0);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        frame_14 = new QFrame(frame_CustomMacro);
        frame_14->setObjectName(QString::fromUtf8("frame_14"));
        frame_14->setFrameShape(QFrame::StyledPanel);
        frame_14->setFrameShadow(QFrame::Raised);
        horizontalLayout_8 = new QHBoxLayout(frame_14);
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(3, 3, 3, 3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setHorizontalSpacing(6);
        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setSpacing(1);
        horizontalLayout_30->setObjectName(QString::fromUtf8("horizontalLayout_30"));
        horizontalLayout_30->setContentsMargins(-1, 0, -1, -1);
        comboBox_CustomMacroParamsNames = new QComboBox(frame_14);
        comboBox_CustomMacroParamsNames->setObjectName(QString::fromUtf8("comboBox_CustomMacroParamsNames"));
        comboBox_CustomMacroParamsNames->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_30->addWidget(comboBox_CustomMacroParamsNames);

        toolButton_5 = new QToolButton(frame_14);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));
        QIcon icon43;
        icon43.addFile(QString::fromUtf8(":/new/prefix1/description.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_5->setIcon(icon43);

        horizontalLayout_30->addWidget(toolButton_5);


        gridLayout_10->addLayout(horizontalLayout_30, 7, 4, 1, 1);

        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setSpacing(1);
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        horizontalLayout_31->setContentsMargins(-1, 0, 0, -1);
        lineEdit_CustomMacroName = new QLineEdit(frame_14);
        lineEdit_CustomMacroName->setObjectName(QString::fromUtf8("lineEdit_CustomMacroName"));
        lineEdit_CustomMacroName->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_31->addWidget(lineEdit_CustomMacroName);

        toolButton_8 = new QToolButton(frame_14);
        toolButton_8->setObjectName(QString::fromUtf8("toolButton_8"));
        toolButton_8->setIcon(icon43);

        horizontalLayout_31->addWidget(toolButton_8);


        gridLayout_10->addLayout(horizontalLayout_31, 1, 4, 1, 1);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setSpacing(1);
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        horizontalLayout_29->setContentsMargins(-1, 0, -1, -1);
        lineEdit_CustomMacroParamName = new QLineEdit(frame_14);
        lineEdit_CustomMacroParamName->setObjectName(QString::fromUtf8("lineEdit_CustomMacroParamName"));
        lineEdit_CustomMacroParamName->setMaximumSize(QSize(16777215, 26));

        horizontalLayout_29->addWidget(lineEdit_CustomMacroParamName);


        gridLayout_10->addLayout(horizontalLayout_29, 6, 4, 1, 1);

        label_34 = new QLabel(frame_14);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setMinimumSize(QSize(0, 26));
        QFont font8;
        font8.setPointSize(8);
        label_34->setFont(font8);

        gridLayout_10->addWidget(label_34, 6, 2, 1, 1);

        verticalLayout_41 = new QVBoxLayout();
        verticalLayout_41->setSpacing(6);
        verticalLayout_41->setObjectName(QString::fromUtf8("verticalLayout_41"));
        verticalLayout_41->setContentsMargins(0, -1, -1, -1);
        comboBox_CustomMacroStyle = new QComboBox(frame_14);
        comboBox_CustomMacroStyle->setObjectName(QString::fromUtf8("comboBox_CustomMacroStyle"));
        comboBox_CustomMacroStyle->setMaximumSize(QSize(16777215, 26));

        verticalLayout_41->addWidget(comboBox_CustomMacroStyle);


        gridLayout_10->addLayout(verticalLayout_41, 2, 4, 1, 1);

        frame_24 = new QFrame(frame_14);
        frame_24->setObjectName(QString::fromUtf8("frame_24"));
        frame_24->setMinimumSize(QSize(0, 10));
        frame_24->setFrameShape(QFrame::HLine);
        frame_24->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(frame_24, 9, 4, 1, 1);

        label_32 = new QLabel(frame_14);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setFont(font8);

        gridLayout_10->addWidget(label_32, 3, 2, 1, 1);

        verticalLayout_39 = new QVBoxLayout();
        verticalLayout_39->setSpacing(3);
        verticalLayout_39->setObjectName(QString::fromUtf8("verticalLayout_39"));
        frame_8 = new QFrame(frame_14);
        frame_8->setObjectName(QString::fromUtf8("frame_8"));
        frame_8->setFrameShape(QFrame::Panel);
        frame_8->setFrameShadow(QFrame::Sunken);
        horizontalLayout_6 = new QHBoxLayout(frame_8);
        horizontalLayout_6->setSpacing(2);
        horizontalLayout_6->setContentsMargins(2, 2, 2, 2);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        toolButton_CustomMacroAddNew = new QToolButton(frame_8);
        toolButton_CustomMacroAddNew->setObjectName(QString::fromUtf8("toolButton_CustomMacroAddNew"));
        toolButton_CustomMacroAddNew->setIcon(icon21);
        toolButton_CustomMacroAddNew->setIconSize(QSize(12, 12));

        horizontalLayout_6->addWidget(toolButton_CustomMacroAddNew);

        toolButton_CustomMacroDelete = new QToolButton(frame_8);
        toolButton_CustomMacroDelete->setObjectName(QString::fromUtf8("toolButton_CustomMacroDelete"));
        toolButton_CustomMacroDelete->setIcon(icon22);
        toolButton_CustomMacroDelete->setIconSize(QSize(12, 12));

        horizontalLayout_6->addWidget(toolButton_CustomMacroDelete);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        toolButton_CustomMacroApply = new QToolButton(frame_8);
        toolButton_CustomMacroApply->setObjectName(QString::fromUtf8("toolButton_CustomMacroApply"));
        QIcon icon44;
        icon44.addFile(QString::fromUtf8(":/new/prefix1/checkmark.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_CustomMacroApply->setIcon(icon44);
        toolButton_CustomMacroApply->setIconSize(QSize(12, 12));

        horizontalLayout_6->addWidget(toolButton_CustomMacroApply);

        toolButton_CustomMacroDiscard = new QToolButton(frame_8);
        toolButton_CustomMacroDiscard->setObjectName(QString::fromUtf8("toolButton_CustomMacroDiscard"));
        QIcon icon45;
        icon45.addFile(QString::fromUtf8(":/new/prefix1/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_CustomMacroDiscard->setIcon(icon45);
        toolButton_CustomMacroDiscard->setIconSize(QSize(12, 12));

        horizontalLayout_6->addWidget(toolButton_CustomMacroDiscard);


        verticalLayout_39->addWidget(frame_8);


        gridLayout_10->addLayout(verticalLayout_39, 10, 4, 1, 1);

        frame_23 = new QFrame(frame_14);
        frame_23->setObjectName(QString::fromUtf8("frame_23"));
        frame_23->setMinimumSize(QSize(0, 10));
        frame_23->setFrameShape(QFrame::HLine);
        frame_23->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(frame_23, 5, 4, 1, 1);

        verticalLayout_38 = new QVBoxLayout();
        verticalLayout_38->setSpacing(6);
        verticalLayout_38->setObjectName(QString::fromUtf8("verticalLayout_38"));
        verticalLayout_38->setContentsMargins(0, -1, -1, -1);
        frame_26 = new QFrame(frame_14);
        frame_26->setObjectName(QString::fromUtf8("frame_26"));
        frame_26->setFrameShape(QFrame::Panel);
        frame_26->setFrameShadow(QFrame::Sunken);
        horizontalLayout_20 = new QHBoxLayout(frame_26);
        horizontalLayout_20->setSpacing(0);
        horizontalLayout_20->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_20->setObjectName(QString::fromUtf8("horizontalLayout_20"));
        horizontalLayout_20->setContentsMargins(1, 0, 0, 0);
        label_24 = new QLabel(frame_26);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setMinimumSize(QSize(0, 27));
        label_24->setFont(font);

        horizontalLayout_20->addWidget(label_24);


        verticalLayout_38->addWidget(frame_26);


        gridLayout_10->addLayout(verticalLayout_38, 10, 2, 1, 1);

        label_23 = new QLabel(frame_14);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setFont(font);

        gridLayout_10->addWidget(label_23, 1, 2, 1, 1);

        verticalLayout_40 = new QVBoxLayout();
        verticalLayout_40->setSpacing(0);
        verticalLayout_40->setObjectName(QString::fromUtf8("verticalLayout_40"));
        verticalLayout_40->setContentsMargins(-1, 0, -1, -1);
        label_33 = new QLabel(frame_14);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setFont(font8);

        verticalLayout_40->addWidget(label_33);


        gridLayout_10->addLayout(verticalLayout_40, 2, 2, 1, 1);

        frame_25 = new QFrame(frame_14);
        frame_25->setObjectName(QString::fromUtf8("frame_25"));
        frame_25->setMinimumSize(QSize(0, 10));
        frame_25->setFrameShape(QFrame::HLine);
        frame_25->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(frame_25, 9, 2, 1, 1);

        widget = new QWidget(frame_14);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_32 = new QHBoxLayout(widget);
        horizontalLayout_32->setSpacing(1);
        horizontalLayout_32->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        toolButton_CustomMacroAddNew_2 = new QToolButton(widget);
        toolButton_CustomMacroAddNew_2->setObjectName(QString::fromUtf8("toolButton_CustomMacroAddNew_2"));
        toolButton_CustomMacroAddNew_2->setIcon(icon21);
        toolButton_CustomMacroAddNew_2->setIconSize(QSize(12, 12));

        horizontalLayout_32->addWidget(toolButton_CustomMacroAddNew_2);

        toolButton_CustomMacroDelete_2 = new QToolButton(widget);
        toolButton_CustomMacroDelete_2->setObjectName(QString::fromUtf8("toolButton_CustomMacroDelete_2"));
        toolButton_CustomMacroDelete_2->setIcon(icon22);
        toolButton_CustomMacroDelete_2->setIconSize(QSize(12, 12));

        horizontalLayout_32->addWidget(toolButton_CustomMacroDelete_2);


        gridLayout_10->addWidget(widget, 7, 2, 1, 1);

        comboBox_CustomMacroReturns = new QComboBox(frame_14);
        comboBox_CustomMacroReturns->setObjectName(QString::fromUtf8("comboBox_CustomMacroReturns"));
        comboBox_CustomMacroReturns->setMinimumSize(QSize(130, 0));
        comboBox_CustomMacroReturns->setMaximumSize(QSize(200, 26));

        gridLayout_10->addWidget(comboBox_CustomMacroReturns, 3, 4, 1, 1);


        horizontalLayout_8->addLayout(gridLayout_10);


        verticalLayout_12->addWidget(frame_14);


        verticalLayout_31->addWidget(frame_CustomMacro);

        verticalSpacer_14 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_31->addItem(verticalSpacer_14);


        gridLayout_8->addWidget(frame_13, 2, 0, 1, 1);

        label_MacroItemName = new QLabel(groupBox_VarFuncCreator);
        label_MacroItemName->setObjectName(QString::fromUtf8("label_MacroItemName"));
        label_MacroItemName->setFont(font);

        gridLayout_8->addWidget(label_MacroItemName, 0, 0, 1, 1);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setSpacing(6);
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        horizontalLayout_33->setContentsMargins(-1, 0, -1, -1);
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_33->addItem(horizontalSpacer_8);

        toolButton_EditorCheckSyntax = new QToolButton(groupBox_VarFuncCreator);
        toolButton_EditorCheckSyntax->setObjectName(QString::fromUtf8("toolButton_EditorCheckSyntax"));
        sizePolicy5.setHeightForWidth(toolButton_EditorCheckSyntax->sizePolicy().hasHeightForWidth());
        toolButton_EditorCheckSyntax->setSizePolicy(sizePolicy5);
        toolButton_EditorCheckSyntax->setFont(font1);
        QIcon icon46;
        icon46.addFile(QString::fromUtf8(":/new/prefix1/spell-check-error.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_EditorCheckSyntax->setIcon(icon46);
        toolButton_EditorCheckSyntax->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        horizontalLayout_33->addWidget(toolButton_EditorCheckSyntax);


        gridLayout_8->addLayout(horizontalLayout_33, 0, 1, 1, 1);


        horizontalLayout_7->addWidget(groupBox_VarFuncCreator);

        QIcon icon47;
        icon47.addFile(QString::fromUtf8(":/new/prefix1/cycle-icon-2.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Top->addTab(tab_LifeCycle, icon47, QString());
        tab_Outputs = new QWidget();
        tab_Outputs->setObjectName(QString::fromUtf8("tab_Outputs"));
        verticalLayout_19 = new QVBoxLayout(tab_Outputs);
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        groupBox_5 = new QGroupBox(tab_Outputs);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        verticalLayout_18 = new QVBoxLayout(groupBox_5);
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(0, -1, 0, 0);
        frame_6 = new QFrame(groupBox_5);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(frame_6);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalSpacer_5 = new QSpacerItem(20, 149, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_5, 1, 0, 1, 1);

        textEdit = new QTextEdit(frame_6);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout_4->addWidget(textEdit, 0, 0, 1, 1);


        verticalLayout_18->addWidget(frame_6);


        verticalLayout_19->addWidget(groupBox_5);

        QIcon icon48;
        icon48.addFile(QString::fromUtf8(":/new/prefix1/output-build-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        tabWidget_Top->addTab(tab_Outputs, icon48, QString());

        verticalLayout_20->addWidget(tabWidget_Top);


        verticalLayout->addLayout(verticalLayout_20);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1141, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuBuild = new QMenu(menuBar);
        menuBuild->setObjectName(QString::fromUtf8("menuBuild"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuToolbars = new QMenu(menuView);
        menuToolbars->setObjectName(QString::fromUtf8("menuToolbars"));
        QIcon icon49;
        icon49.addFile(QString::fromUtf8(":/new/prefix1/toolbox.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuToolbars->setIcon(icon49);
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QSizePolicy sizePolicy9(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy9);
        mainToolBar->setFont(font);
        mainToolBar->setIconSize(QSize(16, 24));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(411, 226));
        QFont font9;
        font9.setBold(false);
        font9.setWeight(50);
        dockWidget->setFont(font9);
        dockWidget->setFloating(false);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        frame_5 = new QFrame(dockWidgetContents);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setMinimumSize(QSize(20, 100));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        verticalLayout_9 = new QVBoxLayout(frame_5);
        verticalLayout_9->setSpacing(3);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        toolButton = new QToolButton(frame_5);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon50;
        icon50.addFile(QString::fromUtf8(":/new/prefix1/Sync-icon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon50.addFile(QString::fromUtf8(":/new/prefix1/stop_red.png"), QSize(), QIcon::Normal, QIcon::On);
        toolButton->setIcon(icon50);
        toolButton->setCheckable(true);
        toolButton->setChecked(true);

        verticalLayout_9->addWidget(toolButton);

        toolButton_15 = new QToolButton(frame_5);
        toolButton_15->setObjectName(QString::fromUtf8("toolButton_15"));
        QIcon icon51;
        icon51.addFile(QString::fromUtf8(":/new/prefix1/Clear-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_15->setIcon(icon51);

        verticalLayout_9->addWidget(toolButton_15);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_9->addItem(verticalSpacer_4);


        horizontalLayout_5->addWidget(frame_5);

        verticalLayout_15 = new QVBoxLayout();
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        tabWidget_Outputs = new QTabWidget(dockWidgetContents);
        tabWidget_Outputs->setObjectName(QString::fromUtf8("tabWidget_Outputs"));
        tab_5 = new QWidget();
        tab_5->setObjectName(QString::fromUtf8("tab_5"));
        verticalLayout_16 = new QVBoxLayout(tab_5);
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        textEdit_2 = new QTextEdit(tab_5);
        textEdit_2->setObjectName(QString::fromUtf8("textEdit_2"));
        textEdit_2->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_16->addWidget(textEdit_2);

        tabWidget_Outputs->addTab(tab_5, icon13, QString());
        tab_6 = new QWidget();
        tab_6->setObjectName(QString::fromUtf8("tab_6"));
        verticalLayout_17 = new QVBoxLayout(tab_6);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setContentsMargins(11, 11, 11, 11);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        plainTextEdit_2 = new QPlainTextEdit(tab_6);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout_17->addWidget(plainTextEdit_2);

        tabWidget_Outputs->addTab(tab_6, icon9, QString());

        verticalLayout_15->addWidget(tabWidget_Outputs);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        toolButton_StopProcess = new QToolButton(dockWidgetContents);
        toolButton_StopProcess->setObjectName(QString::fromUtf8("toolButton_StopProcess"));
        QIcon icon52;
        icon52.addFile(QString::fromUtf8(":/new/prefix1/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_StopProcess->setIcon(icon52);

        horizontalLayout_4->addWidget(toolButton_StopProcess);

        label_ProcessingInfo = new QLabel(dockWidgetContents);
        label_ProcessingInfo->setObjectName(QString::fromUtf8("label_ProcessingInfo"));
        label_ProcessingInfo->setFont(font1);

        horizontalLayout_4->addWidget(label_ProcessingInfo);

        progressBar = new QProgressBar(dockWidgetContents);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 7));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);

        horizontalLayout_4->addWidget(progressBar);

        label_9 = new QLabel(dockWidgetContents);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_4->addWidget(label_9);


        verticalLayout_15->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout_15);


        verticalLayout_2->addLayout(horizontalLayout_5);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget);
        toolBar_Context = new QToolBar(MainWindow);
        toolBar_Context->setObjectName(QString::fromUtf8("toolBar_Context"));
        toolBar_Context->setFont(font);
        toolBar_Context->setMovable(true);
        toolBar_Context->setIconSize(QSize(24, 24));
        toolBar_Context->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBar_Context->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_Context);
        toolBar_Extended = new QToolBar(MainWindow);
        toolBar_Extended->setObjectName(QString::fromUtf8("toolBar_Extended"));
        toolBar_Extended->setFont(font);
        toolBar_Extended->setIconSize(QSize(72, 24));
        toolBar_Extended->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolBar_Extended->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_Extended);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuBuild->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew_C_Project);
        menuFile->addAction(actionNew_Python_Project);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad_Project);
        menuFile->addAction(actionSave_Project);
        menuFile->addAction(actionExport_Source);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionHelp);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuBuild->addAction(actionCompile_Simulation);
        menuBuild->addAction(actionRun_Simulation);
        menuView->addAction(actionOuput_Window);
        menuView->addAction(menuToolbars->menuAction());
        menuView->addSeparator();
        menuView->addAction(actionBuild_Output);
        menuView->addAction(actionOutput);
        menuToolbars->addAction(actionContext_Toolbar);
        menuToolbars->addAction(actionExtended_Toolbar);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionPreferences);
        mainToolBar->addAction(actionAbout2);
        mainToolBar->addSeparator();
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionSave_Project);
        mainToolBar->addAction(actionBuild);
        mainToolBar->addAction(actionRun);
        mainToolBar->addSeparator();
        mainToolBar->addSeparator();
        toolBar_Context->addAction(actionSave_Tab);
        toolBar_Context->addAction(actionLoad_Tab);
        toolBar_Context->addAction(actionDefault_Tab);
        toolBar_Context->addSeparator();
        toolBar_Context->addSeparator();
        toolBar_Extended->addAction(actionELPS_Version);

        retranslateUi(MainWindow);
        QObject::connect(listWidget_ItemList, SIGNAL(updateForeignModelSignal(int,PointerLessModels::t_basic_item*)), MainWindow, SLOT(updateCurrentPopCellModelSlot(int,PointerLessModels::t_basic_item*)));
        QObject::connect(textEdit_Formula, SIGNAL(cursorPositionChanged()), textEdit_Formula, SLOT(matchParentheses()));
        QObject::connect(textEdit_CustomMacro, SIGNAL(cursorPositionChanged()), textEdit_CustomMacro, SLOT(matchParentheses()));
        QObject::connect(textEdit_CustomMacro, SIGNAL(cursorPositionChanged()), textEdit_CustomMacro, SLOT(nonEditableLinesToGrey()));

        tabWidget_Top->setCurrentIndex(3);
        stackedWidget_Pop->setCurrentIndex(0);
        toolBox_CodeEditor->setCurrentIndex(0);
        toolBox_CodeEditor->layout()->setSpacing(1);
        comboBox_ElpBuiltin->setCurrentIndex(0);
        comboBox_ElpBuiltinKeywords->setCurrentIndex(0);
        comboBox_CustomMacroStyle->setCurrentIndex(0);
        comboBox_CustomMacroReturns->setCurrentIndex(0);
        tabWidget_Outputs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EVOLAB-GUI (version 0.0.1)", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About...", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        actionHelp->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionLoad_Project->setText(QApplication::translate("MainWindow", "Load Project...", 0, QApplication::UnicodeUTF8));
        actionLoad_Project->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionAbout2->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAbout2->setToolTip(QApplication::translate("MainWindow", "About EVOLAB-GUI", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBuild->setText(QApplication::translate("MainWindow", "Build", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionBuild->setToolTip(QApplication::translate("MainWindow", "Build Project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionBuild->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        actionCompile_Simulation->setText(QApplication::translate("MainWindow", "Build Simulation", 0, QApplication::UnicodeUTF8));
        actionCompile_Simulation->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", 0, QApplication::UnicodeUTF8));
        actionNew_C_Project->setText(QApplication::translate("MainWindow", "New C++ Project...", 0, QApplication::UnicodeUTF8));
        actionNew_Python_Project->setText(QApplication::translate("MainWindow", "New Py Project...", 0, QApplication::UnicodeUTF8));
        actionSave_Project->setText(QApplication::translate("MainWindow", "Save Project...", 0, QApplication::UnicodeUTF8));
        actionSave_Project->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionRun_Simulation->setText(QApplication::translate("MainWindow", "Run Simulation", 0, QApplication::UnicodeUTF8));
        actionRun_Simulation->setShortcut(QApplication::translate("MainWindow", "Ctrl+Return", 0, QApplication::UnicodeUTF8));
        actionOutput->setText(QApplication::translate("MainWindow", "Run Output", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRun->setToolTip(QApplication::translate("MainWindow", "Run Simulation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRun->setShortcut(QApplication::translate("MainWindow", "Ctrl+Return", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences...", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        actionBuild_Output->setText(QApplication::translate("MainWindow", "Build Output", 0, QApplication::UnicodeUTF8));
        actionOuput_Window->setText(QApplication::translate("MainWindow", "Ouput Window", 0, QApplication::UnicodeUTF8));
        actionELPS_Version->setText(QApplication::translate("MainWindow", "ELPS Version", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionExtended_Toolbar->setText(QApplication::translate("MainWindow", "Extended Toolbar", 0, QApplication::UnicodeUTF8));
        actionExport_Source->setText(QApplication::translate("MainWindow", "Export Source...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExport_Source->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Export source files:</span><br/>Exports all source files, as well as auto-generated C++ sources and Makefiles. Everything that is required to run the simulation from outside the GUI (a cluster, for example).</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExport_Source->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+E", 0, QApplication::UnicodeUTF8));
        actionContext_Toolbar->setText(QApplication::translate("MainWindow", "Context Toolbar", 0, QApplication::UnicodeUTF8));
        actionSave_Tab->setText(QApplication::translate("MainWindow", "Save Tab", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSave_Tab->setToolTip(QApplication::translate("MainWindow", "Save Current Tab Settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSave_Tab->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionLoad_Tab->setText(QApplication::translate("MainWindow", "Load Tab", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionLoad_Tab->setToolTip(QApplication::translate("MainWindow", "Load Current Tab Settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionDefault_Tab->setText(QApplication::translate("MainWindow", "Clear Tab", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionDefault_Tab->setToolTip(QApplication::translate("MainWindow", "Restore Current Tab Defaults", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("MainWindow", "Site", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_AddPop->setToolTip(QApplication::translate("MainWindow", " Add Pop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_AddPop->setText(QApplication::translate("MainWindow", "Add Pop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_RemovePop->setToolTip(QApplication::translate("MainWindow", "Remove Pop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_RemovePop->setText(QApplication::translate("MainWindow", "Remove Pop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_PopRowUp->setToolTip(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_PopRowUp->setText(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_PopRowDown->setToolTip(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_PopRowDown->setText(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_DuplicatePop->setToolTip(QApplication::translate("MainWindow", "Duplicate Pop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_DuplicatePop->setText(QApplication::translate("MainWindow", "Duplicate Pop", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_25->setToolTip(QApplication::translate("MainWindow", "Select / Drag", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_25->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_LockEditPopCellItem->setToolTip(QApplication::translate("MainWindow", "Lock / Edit item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_LockEditPopCellItem->setText(QApplication::translate("MainWindow", "Lock / Edit item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CopyPopCellItem->setToolTip(QApplication::translate("MainWindow", "Copy item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CopyPopCellItem->setText(QApplication::translate("MainWindow", "Copy item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CutPopCellItem->setToolTip(QApplication::translate("MainWindow", "Cut / Clear item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CutPopCellItem->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_PastePopCellItem->setToolTip(QApplication::translate("MainWindow", "Paste item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_PastePopCellItem->setText(QApplication::translate("MainWindow", "Paste item", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_Pop->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "Site / Pop Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_Pop->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "Attributes", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_Pop->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "States", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_Pop->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "Events", 0, QApplication::UnicodeUTF8));
        groupBox_SelectedCell->setTitle(QApplication::translate("MainWindow", "Selected Cell Settings", 0, QApplication::UnicodeUTF8));
        label_13->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Pop [attr, state, event]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_Pop_AddItem->setToolTip(QApplication::translate("MainWindow", "Add item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_Pop_AddItem->setText(QApplication::translate("MainWindow", "Add item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_27->setToolTip(QApplication::translate("MainWindow", "Remove item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_27->setText(QApplication::translate("MainWindow", "Remove item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_PopCellItemUp->setToolTip(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_PopCellItemUp->setText(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_PopCellItemDown->setToolTip(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_PopCellItemDown->setText(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_28->setToolTip(QApplication::translate("MainWindow", "Clear all items", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_28->setText(QApplication::translate("MainWindow", "Clear all items", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Items", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "Param1", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Item Settings", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Item Description:", 0, QApplication::UnicodeUTF8));
        plainTextEdit_ItemDesc->setPlainText(QApplication::translate("MainWindow", "try\n"
"yrtyrt", 0, QApplication::UnicodeUTF8));
        groupBox_PopDescription->setTitle(QApplication::translate("MainWindow", "Selected Site / Pop Description ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QString());
        label_PopDescription->setText(QApplication::translate("MainWindow", "( - )", 0, QApplication::UnicodeUTF8));
        plainText_Edit_PopDescription->setPlainText(QApplication::translate("MainWindow", "Describe Population...", 0, QApplication::UnicodeUTF8));
        tabWidget_Top->setTabText(tabWidget_Top->indexOf(tab_Populations), QApplication::translate("MainWindow", "Populations", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Setup", 0, QApplication::UnicodeUTF8));
        label_NetworkNbNode->setText(QApplication::translate("MainWindow", "Number of Nodes", 0, QApplication::UnicodeUTF8));
        label_NetworkDist->setText(QApplication::translate("MainWindow", "Distribution Type", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Default distribution parameters...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_NetworkResetDist->setToolTip(QApplication::translate("MainWindow", "Reset current distribution parameters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_NetworkResetDist->setText(QApplication::translate("MainWindow", "Default Params...", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Distribution Parameters:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Populate", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Populations Spreading Method", 0, QApplication::UnicodeUTF8));
        comboBox_NetPopsSpread->setItemText(0, QApplication::translate("MainWindow", "Stochastic", 0, QApplication::UnicodeUTF8));
        comboBox_NetPopsSpread->setItemText(1, QApplication::translate("MainWindow", "Fixed", 0, QApplication::UnicodeUTF8));

        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_NetPopsRepartition->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "Sites / Populations", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_NetPopsRepartition->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "Distributions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_NetPopIcon_2->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#8b0000;\">Warning...</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_NetPopIcon_2->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_18->setToolTip(QApplication::translate("MainWindow", "Green is OK, Red is not...", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_18->setText(QApplication::translate("MainWindow", "Check Total", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_NetPopIcon->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#8b0000;\">Warning!</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_NetPopIcon->setText(QString());
        textEdit_SpreadTotal->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:12pt; font-weight:600; color:#3cb371;\">1.0</span><span style=\" font-family:'Ubuntu'; font-size:12pt; font-weight:600; color:#000000;\"> / </span><span style=\" font-family:'Ubuntu'; font-size:12pt; font-weight:600; color:#aa0000;\">0.99</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_NetGenerate->setText(QApplication::translate("MainWindow", "   Generate Network  ", 0, QApplication::UnicodeUTF8));
        tabWidget_Top->setTabText(tabWidget_Top->indexOf(tab_Network), QApplication::translate("MainWindow", "Network", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("MainWindow", "Item", 0, QApplication::UnicodeUTF8));
        toolButton_VarAdd->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        toolButton_VarRemove->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_VarRowUp->setToolTip(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_VarRowUp->setText(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_VarRowDown->setToolTip(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_VarRowDown->setText(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "Variables:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_Variables->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "Variable / Function Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_Variables->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "Formula", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("MainWindow", "Variable or Function Description:", 0, QApplication::UnicodeUTF8));
        plainTextEdit_VarDesc->setPlainText(QString());
        groupBox_VarFuncTrashed->setTitle(QApplication::translate("MainWindow", "Formula           (Variable: \"??????\")", 0, QApplication::UnicodeUTF8));
        toolButton_10->setText(QApplication::translate("MainWindow", "New Const...", 0, QApplication::UnicodeUTF8));
        label_15->setText(QString());
        comboBox_4->clear();
        comboBox_4->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "NB_SITES", 0, QApplication::UnicodeUTF8)
        );
        toolButton_11->setText(QApplication::translate("MainWindow", "New Func...", 0, QApplication::UnicodeUTF8));
        comboBox_2->clear();
        comboBox_2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "NB_NEIGH(site id)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "MEAN(attribute name)", 0, QApplication::UnicodeUTF8)
        );
        label_16->setText(QString());
        plainTextEdit->setPlainText(QApplication::translate("MainWindow", "This whole groupbox is defined once and for all in \"tab_LifeCycle\" and grabbed from there !!!", 0, QApplication::UnicodeUTF8));
        tabWidget_Top->setTabText(tabWidget_Top->indexOf(tab_Variables), QApplication::translate("MainWindow", "Variables", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "Row", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("MainWindow", "Add", 0, QApplication::UnicodeUTF8));
        toolButton_7->setText(QApplication::translate("MainWindow", "Remove", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_31->setToolTip(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_31->setText(QApplication::translate("MainWindow", "Move up", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_32->setToolTip(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_32->setText(QApplication::translate("MainWindow", "Move down", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_33->setToolTip(QApplication::translate("MainWindow", "Duplicate Pop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_33->setText(QApplication::translate("MainWindow", "Duplicate Pop", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MainWindow", "Item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_34->setToolTip(QApplication::translate("MainWindow", "Lock / Edit item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_34->setText(QApplication::translate("MainWindow", "Lock / Edit item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_16->setToolTip(QApplication::translate("MainWindow", "Copy item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_16->setText(QApplication::translate("MainWindow", "Copy item", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_35->setToolTip(QApplication::translate("MainWindow", "Cut / Clear item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_35->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_14->setToolTip(QApplication::translate("MainWindow", "Paste item", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_14->setText(QApplication::translate("MainWindow", "Paste item", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_LifeCycle->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_LifeCycle->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "Rate (var or func)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_LifeCycle->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "Transition  (func)", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_LifeCycle->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "State", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_LifeCycle->horizontalHeaderItem(4);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "Space", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget_LifeCycle->verticalHeaderItem(0);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget_LifeCycle->verticalHeaderItem(1);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = tableWidget_LifeCycle->isSortingEnabled();
        tableWidget_LifeCycle->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget_LifeCycle->item(0, 0);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "Infection", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget_LifeCycle->item(0, 1);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "beta", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget_LifeCycle->item(0, 2);
        ___qtablewidgetitem17->setText(QApplication::translate("MainWindow", "userdef_func()...", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget_LifeCycle->item(0, 3);
        ___qtablewidgetitem18->setText(QApplication::translate("MainWindow", "I", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget_LifeCycle->item(0, 4);
        ___qtablewidgetitem19->setText(QApplication::translate("MainWindow", "Local(level)/Global", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget_LifeCycle->item(1, 0);
        ___qtablewidgetitem20->setText(QApplication::translate("MainWindow", "Death", 0, QApplication::UnicodeUTF8));
        tableWidget_LifeCycle->setSortingEnabled(__sortingEnabled);

        label_21->setText(QApplication::translate("MainWindow", "Selected ???? Description:", 0, QApplication::UnicodeUTF8));
        groupBox_VarFuncCreator->setTitle(QApplication::translate("MainWindow", "Rate / Transition Formula", 0, QApplication::UnicodeUTF8));
        textEdit_Formula->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Monospace'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">DOUBLE BIRTH_RATE(SITE)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">BEGIN</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">DOUBLE ret</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">...</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">RETURN"
                        "(ret)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">END</p></body></html>", 0, QApplication::UnicodeUTF8));
        toolBox_CodeEditor->setItemText(toolBox_CodeEditor->indexOf(CurrentFormulaEditing), QApplication::translate("MainWindow", "Current Formula Editing", 0, QApplication::UnicodeUTF8));
        textEdit_CustomMacro->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Monospace'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">DOUBLE BIRTH_RATE(SITE)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">BEGIN</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">DOUBLE ret</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">...</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">RETURN"
                        "(ret)</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">END</p></body></html>", 0, QApplication::UnicodeUTF8));
        toolBox_CodeEditor->setItemText(toolBox_CodeEditor->indexOf(CustomMacroEditing), QApplication::translate("MainWindow", "Custom MACRO Editing", 0, QApplication::UnicodeUTF8));
        label_27->setText(QString());
        label_22->setText(QApplication::translate("MainWindow", "Available MACROs", 0, QApplication::UnicodeUTF8));
        comboBox_ElpBuiltin->clear();
        comboBox_ElpBuiltin->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "NB_NEIGH(site id, level)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "MEAN(attribute name)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "NB_SITES()", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboBox_ElpBuiltin->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Builtin constants, variables and functions:</span><br/>Predefined constants, variables and functions MACROs.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        toolButton_26->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Insert at cursor's position:</span><br/>Insert MACRO to the right sided global formula text area.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_26->setText(QApplication::translate("MainWindow", "New Func...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("MainWindow", "User-defined MACROs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label->setText(QString());
        comboBox_ElpBuiltinKeywords->clear();
        comboBox_ElpBuiltinKeywords->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "RETURN(T)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "FOREACH(T,VECTOR<T>)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "BEGIN", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "END", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DEFINE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "VOID", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "BOOL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "INT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DOUBLE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "INT_PTR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DOUBLE_PTR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "FREE_PTR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RND", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "VECTOR<T>", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "NODE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "CURRENT_NODE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "SITE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "CURRENT_SITE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DEF_FUNC(type, name)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DEF_CONST(type, name, value)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DEF_VAR(type, name, value)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DEF_PTR(type, name, value)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "SET_VAR(name, value)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "SET_PTR(name, value)", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboBox_ElpBuiltinKeywords->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Some builtin keywords:</span><br/>Predefined keyword MACROs</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        label_19->setToolTip(QApplication::translate("MainWindow", "Predefined keyword MACROs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_19->setText(QString());
#ifndef QT_NO_TOOLTIP
        toolButton_FormulaInsertKeyword->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Insert at cursor's position:</span><br/>Insert MACRO to the right sided global formula text area.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_FormulaInsertKeyword->setText(QApplication::translate("MainWindow", "New Const...", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        label_31->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Standard <span style=\" font-weight:600; color:#676700;\">cmath</span> library</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_31->setText(QString());
#ifndef QT_NO_TOOLTIP
        label_20->setToolTip(QApplication::translate("MainWindow", "Predefined constant, variable and function MACROs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_20->setText(QString());
        toolButton_CustomMacroInsert->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        toolButton_CustomMacroEdit->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacros->setItemText(0, QApplication::translate("MainWindow", "BIRTH_RATE", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacros->setItemText(1, QApplication::translate("MainWindow", "MyVar", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacros->setItemText(2, QApplication::translate("MainWindow", "MyFunc", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacros->setItemText(3, QApplication::translate("MainWindow", "---", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacros->setItemText(4, QApplication::translate("MainWindow", "New...", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
        comboBox_CustomMacros->setToolTip(QApplication::translate("MainWindow", "User-defined MACROs", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_26->setText(QString());
        label_25->setText(QApplication::translate("MainWindow", "Custom MACRO", 0, QApplication::UnicodeUTF8));
        toolButton_5->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        lineEdit_CustomMacroName->setText(QApplication::translate("MainWindow", "BIRTH_RATE", 0, QApplication::UnicodeUTF8));
        toolButton_8->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        lineEdit_CustomMacroParamName->setText(QApplication::translate("MainWindow", "NewParam", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("MainWindow", "Params", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacroStyle->clear();
        comboBox_CustomMacroStyle->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "UserConst", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "UserFunc", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "RateCallback", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "EventCallback", 0, QApplication::UnicodeUTF8)
        );
        label_32->setText(QApplication::translate("MainWindow", "Return", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroAddNew->setToolTip(QApplication::translate("MainWindow", "Create a new custom MACRO", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroAddNew->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroDelete->setToolTip(QApplication::translate("MainWindow", "Remove the current custom MACRO", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroDelete->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroApply->setToolTip(QApplication::translate("MainWindow", "Validate changes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroApply->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroDiscard->setToolTip(QApplication::translate("MainWindow", "Discard changes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroDiscard->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("MainWindow", "Tools", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("MainWindow", "Style", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroAddNew_2->setToolTip(QApplication::translate("MainWindow", "Create a new custom MACRO", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroAddNew_2->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton_CustomMacroDelete_2->setToolTip(QApplication::translate("MainWindow", "Remove the current custom MACRO", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton_CustomMacroDelete_2->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        comboBox_CustomMacroReturns->clear();
        comboBox_CustomMacroReturns->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "VOID", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "BOOL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "INT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DOUBLE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "INT_PTR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DOUBLE_PTR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "VECTOR<T>", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "NODE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "SITE", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboBox_CustomMacroReturns->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600; color:#00007f;\">Some builtin keywords:</span><br/>Predefined keyword MACROs</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        label_MacroItemName->setText(QApplication::translate("MainWindow", "[Item Name...]", 0, QApplication::UnicodeUTF8));
        toolButton_EditorCheckSyntax->setText(QApplication::translate("MainWindow", "Check Code Errors", 0, QApplication::UnicodeUTF8));
        tabWidget_Top->setTabText(tabWidget_Top->indexOf(tab_LifeCycle), QApplication::translate("MainWindow", "Life Cycle", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "Output Configuration:", 0, QApplication::UnicodeUTF8));
        textEdit->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Ubuntu'; font-size:11pt;\">Penser qu'on va vouloir certains parametres variables d'un run sur l'autre (au fil des repetitions). Donc </span><span style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:600; color:#ff0000;\">g\303\251rer le nombre de repetitions</span><span style=\" font-family:'Ubuntu'; font-size:11pt;\"> et les </span><span style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:600; color:#ff0000;\">fourchettes de variation (XML??)</span><span style=\" font-family:'Ubuntu'; font-size:11pt;\"> de certians params (qu"
                        "'ils soient globaux, au d'init des attributs, ou que sais-je encore...</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        tabWidget_Top->setTabText(tabWidget_Top->indexOf(tab_Outputs), QApplication::translate("MainWindow", "Outputs", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuBuild->setTitle(QApplication::translate("MainWindow", "Build And Run", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0, QApplication::UnicodeUTF8));
        menuToolbars->setTitle(QApplication::translate("MainWindow", "Toolbars", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindow", "Main Tools", 0, QApplication::UnicodeUTF8));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", "  Simulation Output", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        toolButton->setToolTip(QApplication::translate("MainWindow", "Auto refresh", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        toolButton_15->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        tabWidget_Outputs->setTabText(tabWidget_Outputs->indexOf(tab_5), QApplication::translate("MainWindow", "Build Output", 0, QApplication::UnicodeUTF8));
        plainTextEdit_2->setPlainText(QApplication::translate("MainWindow", "Bleh! Blah...", 0, QApplication::UnicodeUTF8));
        tabWidget_Outputs->setTabText(tabWidget_Outputs->indexOf(tab_6), QApplication::translate("MainWindow", "Run Output", 0, QApplication::UnicodeUTF8));
        toolButton_StopProcess->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        label_ProcessingInfo->setText(QApplication::translate("MainWindow", "Processing...", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "24%", 0, QApplication::UnicodeUTF8));
        toolBar_Context->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar_Extended->setWindowTitle(QApplication::translate("MainWindow", "Context Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
