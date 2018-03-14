#include "Coding/LineNumberArea.h"

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const {
    return QSize(((QTextEditCodeEditor *)codeEditor)->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    ((QTextEditCodeEditor *)codeEditor)->lineNumberAreaPaintEvent(event);
}

