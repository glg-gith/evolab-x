#ifndef QTEXTEDITHIGHLIGHTER_H
#define QTEXTEDITHIGHLIGHTER_H

#include <QTextEdit>
#include <string>

#include <iostream>
#include <vector>

#include "mainwindow.h"
#include "Coding/Highlighter.h"
#include "Coding/LineNumberArea.h"


class MainWindow;



class QTextEditCodeEditor : public QTextEdit
{
    Q_OBJECT

public:

    explicit QTextEditCodeEditor(QWidget *parent = 0);

    void SetMainWindow(MainWindow *main_win);
    void BackupState();

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

signals:


public slots:

    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

    void resizeEvent(QResizeEvent *e);


private slots:

    void matchParentheses();
    void nonEditableLinesToGrey();

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();



private:

    MainWindow *mainWindow;

    Highlighter *syntax_highlighter;
    std::vector<int> non_editable_lines;

    QWidget *lineNumberArea;

    ///
    bool matchLeftParenthesis(QTextBlock currentBlock, int index, int numRightParentheses, bool firstTime = false);
    bool matchRightParenthesis(QTextBlock currentBlock, int index, int numLeftParentheses, bool firstTime = false);
    void createParenthesisSelection(int pos);


public:

    QString prevText_CustomMacro;
    int prevCursorPos_CustomMacro;
    QString prevEndBlockText_CustomMacro;

};

#endif // QTEXTEDITHIGHLIGHTER_H
