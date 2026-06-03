#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCursor>

#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QIcon>

#include <QTimer>

#include <QClipboard>


#include <LabConsts.h>
#include "aboutdialog.h"
#include "varlistitem.h"
#include "elpsversion.h"

#include "Consts.h"
#include "XMLUtils.h"
#include "Models/PointerLessModels.h"
#include "EventFilters/KeyPressOnlyFilter.h"
#include "EventFilters/MouseAndKeyPressOnlyFilter.h"
#include "Models/Macros.h"
#include "qtexteditcodeeditor.h"

#include <vector>
#include <tr1/functional>
#include <memory>


#include <QVariant>



#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef __WIN32__
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif




// TODO: replace this shitty type by PointerLessModels::t_basic_param
//Q_DECLARE_METATYPE(XMLUtils::param_struct *)
Q_DECLARE_METATYPE(PointerLessModels::t_basic_param*)
Q_DECLARE_METATYPE(PointerLessModels::t_basic_item*)
//Q_DECLARE_METATYPE(PopCellListItem *)

Q_DECLARE_METATYPE(PointerLessModels::t_basic_param)
Q_DECLARE_METATYPE(PointerLessModels::t_cell_items_list)

//Q_DECLARE_METATYPE(std::shared_ptr<PointerLessModels::t_basic_item>)


using namespace CTypes;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



    // Misc utils...
    int printCWD();
    void clearLayout(QLayout* layout, bool deleteWidgets);

    struct pop_compare : public std::unary_function<PointerLessModels::t_pop, bool>
    {
        explicit pop_compare(const PointerLessModels::t_pop &r_pop) : r_pop(r_pop) {}
        bool operator() (const PointerLessModels::t_pop &l_pop)
        {
            return (l_pop.name == r_pop.name);
        }
        PointerLessModels::t_pop r_pop;
    };



    // HTML
    QString beginDoc = "<html><body>";
    QString endDoc = "</html></body>";
    QString errorHtml = "<b><font color=\"Crimson\">Error: ";
    QString criticalHtml = "<b><font color=\"DarkRed \">Critical: "; //"<b><font color=\"DarkMagenta \">";
    QString infoHtml = "<b><font color=\"DarkSlateGray\">Info: ";

    QString redHtml = "<b><font color=\"DarkRed \">";
    QString greenHtml = "<b><font color=\"MediumSeaGreen \">";

    QString endHtmlCR = "</font><br/></b>";
    QString endHtml = "</font></b>";

    // Logging
    QTextCursor output_cursor;
    //std::stringstream cerr_buffer;
    //std::stringstream cout_buffer;
    void StartOutputCapture();
    void EndOutputCapture();
    QString GetCapturedOutput();

    // Misc Qt
    void RemoveTableSelectedRows(QTableWidget *qtable);

    // Network
    void UpdateNetworkParams(int index, bool skip_general_params = false);
    void UpdateNetPopRepartition(int index = 0);
    void UpdateNetPopRepartitionTotal();


    // Populations
    void AddPop(PointerLessModels::t_cells_group *p_cg = NULL, int index = -1);
    std::vector<PointerLessModels::t_pop> *GetPopsList();
    void UpdatePopCell(int row, int col, PointerLessModels::t_cell_items_list *model);
    void UpdatePopCellSettings(int row, int col, PointerLessModels::t_cell_items_list *model);
    void UpdatePopCellItemSettings(int index, PointerLessModels::t_basic_item *item);
    void MoveCurrentPopCellItem(int step);
    void MoveCurrentPop(int step);
    bool IsLockedPopCellItem(QTableWidgetItem *item);
    void SetLockedPopCellItem(QTableWidgetItem *item, bool locked);

    // Variables
    void UpdateCurrentVarSettings(int row);
    void UpdateCustomMacroEditor(PointerLessModels::t_variable& var);



private slots:

    // Custom slots:
    void refreshWidget(QWidget *w);
    void outputTimeoutSlot();
    //    void aboutSlot();
    //    void tabWidgetSlot();
    //    void elpsVersionSlot();
    void network_param_valueChanged();
    void spinbox_NetPopsRepartition_valueChanged(double value);
    //    void toggleExtendedToolbarSlot(bool checked);
    //
    void updateCurrentPopCellModelSlot(int index, PointerLessModels::t_basic_item *p_item);
    void popCellItemParam_valueChanged();
    void checkBox_PopParamIndexed_valueChanged();
    void spinBox_PopParamNbBeans_valueChanged();
    void checkBox_PopParamIsDefault_valueChanged();

    //    void on_pushButton_clicked();

    void on_toolButton_VarAdd_clicked();

    void on_toolButton_VarRemove_clicked();

    //    void on_tabWidget_currentChanged(int index);

    //    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_NetworkDist_currentIndexChanged(int index);

    //    void on_toolButton_clicked();

    void on_toolButton_15_clicked();

    //    void on_toolButton_16_clicked();
    //    void on_toolButton_17_clicked();
    //    void on_toolButton_18_clicked();

    //    void on_spinBox_valueChanged(int val);

    void on_spinBox_NetworkNbNodes_valueChanged(int val);


    void on_toolButton_25_toggled(bool checked);

    void on_toolButton_AddPop_clicked();

    void on_tableWidget_Pop_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    //    void on_tableWidget_Pop_itemChanged(QTableWidgetItem *item);

    void on_toolButton_RemovePop_clicked();

    void on_toolButton_Pop_AddItem_clicked();

    //    void on_listWidget_3_itemAddedSignal(int , int , const QString &);


    void on_listWidget_ItemList_itemSelectionChanged();


    void on_toolButton_toggled(bool checked);

    void on_listWidget_ItemList_itemChanged(QListWidgetItem *item);

    void on_plainTextEdit_ItemDesc_textChanged();

    //    void on_listWidget_ItemList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    //    void on_listWidget_ItemList_indexesMoved(const QModelIndexList &indexes);

    void on_toolButton_PopCellItemUp_clicked();

    void on_toolButton_PopCellItemDown_clicked();



    void on_actionSave_Tab_triggered();

    void on_actionLoad_Tab_triggered();

    void on_actionDefault_Tab_triggered();

    void on_actionExtended_Toolbar_triggered(bool checked);

    void on_actionContext_Toolbar_triggered(bool checked);

    void on_actionAbout_triggered();

    void on_actionAbout2_triggered();

    void on_actionELPS_Version_triggered();

    void on_actionQuit_triggered();

    void on_actionSave_Project_triggered();


    ////////
    void on_tabWidget_Top_currentChanged(int index);
    ////////

    void on_plainText_Edit_PopDescription_textChanged();

    void on_toolButton_PopRowUp_clicked();

    void on_toolButton_PopRowDown_clicked();

    void on_tableWidget_Pop_itemSelectionChanged();

    void on_toolButton_DuplicatePop_clicked();

    void on_toolButton_CopyPopCellItem_clicked();

    void on_toolButton_PastePopCellItem_clicked();

    void on_toolButton_CutPopCellItem_clicked();

    void on_toolButton_LockEditPopCellItem_clicked();




    void on_toolButton_NetworkResetDist_clicked();



    void on_actionCopy_triggered();

    void on_comboBox_NetPopsSpread_currentIndexChanged(int index);

    void on_pushButton_NetGenerate_clicked();

    void on_tableWidget_Pop_cellChanged(int row, int column);

    void on_toolButton_VarRowUp_clicked();

    void on_toolButton_VarRowDown_clicked();

    void on_tableWidget_Variables_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableWidget_Variables_cellChanged(int row, int column);

    void on_textEdit_Formula_textChanged();

    void on_plainTextEdit_VarDesc_textChanged();

//    void on_textEdit_Formula_cursorPositionChanged();

    void on_toolButton_StopProcess_clicked();

    void on_comboBox_CustomMacros_currentIndexChanged(int index);

    void on_toolButton_CustomMacroDiscard_clicked();

    void on_textEdit_Formula_cursorPositionChanged();

    void on_toolButton_CustomMacroInsert_clicked();

    void on_textEdit_CustomMacro_cursorPositionChanged();

    void on_textEdit_CustomMacro_textChanged();

    void on_toolButton_FormulaInsertKeyword_clicked();

    void on_tableWidget_Variables_cellClicked(int row, int column);

    void on_lineEdit_VarDesc_textChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
    void displayElpsVersion();

    XMLUtils *xmlUtils;


    // Capture outputs
    std::stringstream cout_buffer;
    std::stringstream cerr_buffer;
    std::stringstream clog_buffer;

    AboutDialog *aboutDialog;
    ElpsVersion *elpsDialog;

    QString projectDirectory;

    // Network
    PointerLessModels::t_network_items_list *p_net_params;
    std::string currentNetworkFile;
    bool totalOK;

    // Pop
    PointerLessModels::t_cell_items_list popCellClipboard;
    std::vector<QTableWidgetItem *> lockedPopCells;

    // Meta Model
    int currentTabIndex, previousTabIndex;
    PointerLessModels::t_meta_model *p_meta_model;


    QTimer *outputTimer;

    // Icons
    const QIcon CHK_ICON = QIcon(":/new/prefix1/checked-small-red.png");
    const QIcon EDIT_ICON = QIcon(":/new/prefix1/edit-icon.png");
    const QIcon LOCK_ICON = QIcon(":/new/prefix1/lock-icon.png");
    const QIcon POINTER_ICON = QIcon(":/new/prefix1/pointer-icon.png");

    const QIcon OK_ICON = QIcon(":/new/prefix1/checkmark24.png");
    const QIcon KO_ICON = QIcon(":/new/prefix1/checkmark24-off.png");


    //int
    KeyPressOnlyFilter *filterKeyPressOnly;
    MouseAndKeyPressOnlyFilter *filterMouseAndKeyPressOnly;


    //
    void setupEditor();
    vector<int> lockedLines;
    int prevCursorPos_Formula;
    bool customMacro_hasBeginEndBlocks;
    PointerLessModels::t_vars *p_custom_macro_formula;

    //QTextEdit *formula_editor;
    // Done in the QTextEditCodeEditor constructor now...
//    Highlighter *syntax_highlighter_formula;
//    Highlighter *syntax_highlighter_macro;

public:



};

#endif // MAINWINDOW_H
