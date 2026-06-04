#include "qtexteditcodeeditor.h"

QTextEditCodeEditor::QTextEditCodeEditor(QWidget *parent) :
    QTextEdit(parent)
{

    this->syntax_highlighter = new Highlighter(this->document());

    // Macro editor stuffs
    this->prevText_CustomMacro = "";
    this->prevCursorPos_CustomMacro = -1;
    this->prevEndBlockText_CustomMacro = "";


    // Line numbers
    lineNumberArea = new LineNumberArea(this);
    ///
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    ///
    updateLineNumberAreaWidth(0);

}

void QTextEditCodeEditor::SetMainWindow(MainWindow *main_win)
{
    this->mainWindow = main_win;
}


void QTextEditCodeEditor::BackupState()
{
    this->prevText_CustomMacro = this->toPlainText();
    this->prevCursorPos_CustomMacro = this->textCursor().position();
}


void QTextEditCodeEditor::mouseReleaseEvent(QMouseEvent *e)
{

    QRegExp begin_regexp = QRegExp("(^|\n)(\\s)*//# BEGIN_EDIT[^\n]*(?=\n|$)");
    QRegExp end_regexp  = QRegExp("(^|\n)(\\s)*//# END_EDIT[^\n]*(?=\n|$)");
    QRegExp end_regexp_0  = QRegExp("//# END_EDIT[^\n]*(?=\n|$)");

    QTextCursor from_1 = QTextCursor(this->document());
    from_1.movePosition(QTextCursor::Start);
    QTextCursor from_2 = QTextCursor(this->document());
    from_2.movePosition(QTextCursor::End);
    QTextCursor cursor_begin = this->document()->find(begin_regexp, from_1);
    QTextCursor cursor_end = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward);
    cursor_begin.movePosition(QTextCursor::EndOfBlock);
    cursor_end.movePosition(QTextCursor::StartOfBlock);
    int begin_pos = cursor_begin.position();
    int end_pos = cursor_end.position();


    QString prev_text = "";
    if (e->button() == Qt::MidButton)
    {
        // Backup the text as it is before middle button click
        prev_text = this->toPlainText();
        // And let the paste operation occure...
        //        e->accept();
        //        return;


        this->disconnect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
        this->disconnect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));

    }


    // !!!!  >> And let the paste operation occure !
    QTextEdit::mouseReleaseEvent(e);
    // !!!!

    if (e->button() == Qt::MidButton) {

        /*
         * Keep track of the editbale ranges (up to you).
         * My way is a single one range inbetween the unique
         * tags "//# BEGIN_EDIT" and "//# END_EDIT"...
         */

        QTextCursor cursor_end_0 = this->document()->find(end_regexp_0, from_2, QTextDocument::FindBackward);
        QString end_block_text = cursor_end_0.block().text();

        bool must_restore = false;
        if (!prev_text.isEmpty() && !(cursor_begin.isNull() || cursor_end.isNull()))
        {
            // Deduce the insertion index by finding the position
            // of the first character that changed between previous
            // text and the current "after-paste" text
            int insert_pos; //, end_insert_pos;
            std::string s_cur = this->toPlainText().toStdString();
            std::string s_prev = prev_text.toStdString();

            int i_max = std::min(s_cur.length(), s_prev.length());
            for (insert_pos=0; insert_pos < i_max; insert_pos++) {
                if (s_cur[insert_pos] != s_prev[insert_pos])
                    break;
            }


            if (this->prevEndBlockText_CustomMacro != end_block_text && !cursor_end_0.isNull())
            {
                cursor_end_0.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor, this->prevEndBlockText_CustomMacro.length());
                this->setTextCursor(cursor_end_0);
                this->insertPlainText("\n");
            }

            from_1.movePosition(QTextCursor::Start);
            from_2.movePosition(QTextCursor::End);
            cursor_begin = this->document()->find(begin_regexp, from_1);
            cursor_end = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward);
            cursor_begin.movePosition(QTextCursor::EndOfBlock);
            cursor_end.movePosition(QTextCursor::StartOfBlock);
            begin_pos = cursor_begin.position();
            end_pos = cursor_end.position();

            // If the insertion point is not in my editable area:
            // just restore the text as it was before the paste occured
            if ((cursor_begin.isNull() || cursor_end.isNull()) || (insert_pos < begin_pos+1 || insert_pos > end_pos))
            {
                must_restore = true;
            }


        }
        else if (prev_text.isEmpty())
        {
            from_1.movePosition(QTextCursor::Start);
            from_2.movePosition(QTextCursor::End);
            cursor_begin = this->document()->find(begin_regexp, from_1);
            cursor_end = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward);
            cursor_begin.movePosition(QTextCursor::EndOfBlock);
            cursor_end.movePosition(QTextCursor::StartOfBlock);
            must_restore = (cursor_begin.isNull() || cursor_end.isNull());

            // Place cursor at beginning of editable area
            QTextCursor curs(cursor_begin);
            curs.movePosition(QTextCursor::NextCharacter);
            this->setTextCursor(curs);
        }

        if (must_restore)
        {
            // Restore text (ghostly) - Assuming, my CustomQTextEdit instance is named "textEdit_CustomMacro"
            // And that the "textChanged()" signal in handled in the top widget (MainWindow)
            this->setText(prev_text);
            this->prevText_CustomMacro = prev_text;
        }
        else
        {
            if (this->prevEndBlockText_CustomMacro == "") { this->prevEndBlockText_CustomMacro = end_block_text; }
            this->BackupState();
        }

        this->connect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
        this->connect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));
    }

}


void QTextEditCodeEditor::mousePressEvent(QMouseEvent *e)
{
    //clog << "Saved cursor !!!!!!!!!!!!!!!!!!!!" << endl;
    //    ((MainWindow *)this->topLevelWidget())->prevCursorPos_CustomMacro = this->textCursor().position();
    QTextEdit::mousePressEvent(e);
}

void QTextEditCodeEditor::keyPressEvent(QKeyEvent *e)
{
    QTextCursor curs = QTextCursor(this->textCursor());

    if (curs.hasSelection() && (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab))
    {

        if (e->key() == Qt::Key_Tab)
        {
            this->disconnect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
            this->disconnect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));

            int spos = curs.anchor();
            int epos = curs.position();

            if (spos > epos) { std::swap(spos, epos); }

            curs.setPosition(spos, QTextCursor::MoveAnchor);
//            int sblock = curs.block().blockNumber();

            curs.setPosition(epos, QTextCursor::MoveAnchor);

            QStringList lines = this->textCursor().selectedText().split(QChar::ParagraphSeparator/*, QString::SkipEmptyParts*/);
            int nb_blocks = lines.count();

//            for (int i=0; i < lines.count(); i++) {
//                cout << i << lines.at(i).toStdString() << endl;
//            }

            // Do indentation
            curs.setPosition(spos, QTextCursor::MoveAnchor);

            curs.beginEditBlock();


            // Indent
            for(int i = 0; i < nb_blocks; ++i)
            {
                curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
                if (lines.at(i) != "") { curs.insertText("\t"); }
                curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
            }

            curs.endEditBlock();

            // Clean Selection
            curs.setPosition(spos, QTextCursor::MoveAnchor);
            curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
            for (int i=0; i < nb_blocks-1; i++) {
                curs.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
            }
            if (lines.at(lines.length()-1) != "")
                curs.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            else
                curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
            this->setTextCursor(curs);

            this->connect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
            this->connect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));

        }
        else if (e->key() == Qt::Key_Backtab)
        {

            this->disconnect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
            this->disconnect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));


            int spos = curs.anchor();
            int epos = curs.position();

            if (spos > epos) { std::swap(spos, epos); }

            curs.setPosition(spos, QTextCursor::MoveAnchor);
            int sblock = curs.block().blockNumber();

            curs.setPosition(epos, QTextCursor::MoveAnchor);
            int eblock = curs.block().blockNumber();

            QStringList lines = this->textCursor().selectedText().split(QChar::ParagraphSeparator/*, QString::SkipEmptyParts*/);
            int nb_blocks = lines.count();

            // Do de-indentation
            curs.setPosition(spos, QTextCursor::MoveAnchor);

            curs.beginEditBlock();

            curs.setPosition(spos, QTextCursor::MoveAnchor);
            //const int blocks_diff = eblock - sblock;
            for(int i = 0; i < nb_blocks; ++i)
            {
                // Select first tabulation
                curs.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
                curs.setPosition(curs.position()+1, QTextCursor::KeepAnchor);
                if (lines.at(i) != "" && curs.selectedText() == "\t")
                {
                    curs.removeSelectedText();
                }
                curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
            }

            curs.endEditBlock();

            this->connect(this, SIGNAL(textChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_textChanged()));
            this->connect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));
        }

        this->BackupState();

        e->accept();
        return;
    }
    else if ((e->modifiers() & Qt::ControlModifier) && (e->key() == Qt::Key_E))     // CTRL-E
    {
        QRegExp begin_regexp = QRegExp("(^|\n)(\\s)*//# BEGIN_EDIT[^\n]*(?=\n|$)");
        QRegExp end_regexp  = QRegExp("//# END_EDIT[^\n]*(?=\n|$)");

        QTextCursor from_1 = QTextCursor(this->document());
        from_1.movePosition(QTextCursor::Start);
        QTextCursor from_2 = QTextCursor(this->document());
        from_2.movePosition(QTextCursor::End);
        QTextCursor cursor_begin = this->document()->find(begin_regexp, from_1);
        QTextCursor cursor_end = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward);
        cursor_begin.movePosition(QTextCursor::EndOfBlock);
        cursor_end.movePosition(QTextCursor::StartOfBlock);

        cout << from_2.isNull() << " | " << from_2.position() << " | " << cursor_end.isNull() << endl;
        cout << cursor_begin.position() << " | " << cursor_end.position() << endl;

        curs.setPosition(cursor_begin.position()+1, QTextCursor::MoveAnchor);
        curs.setPosition(cursor_end.position(), QTextCursor::KeepAnchor);
        this->disconnect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));
        this->setTextCursor(curs);
        this->connect(this, SIGNAL(cursorPositionChanged()), this->mainWindow, SLOT(on_textEdit_CustomMacro_cursorPositionChanged()));

        e->accept();
        return;
    }
    else
        QTextEdit::keyPressEvent(e);
}



void QTextEditCodeEditor::matchParentheses()
{
    //std::clog << "matchParentheses()" << endl;

    QList<QTextEdit::ExtraSelection> selections;
    this->setExtraSelections(selections);

    TextBlockData *data = static_cast<TextBlockData *>(textCursor().block().userData());

    if (data) {
        QVector<ParenthesisInfo *> infos = data->parentheses();

        int pos = textCursor().block().position();
        for (int i = 0; i < infos.size(); ++i) {
            ParenthesisInfo *info = infos.at(i);

            int curPos = textCursor().position() - textCursor().block().position();
            if (info->position == curPos - 1 && info->character == '(') {
                if (matchLeftParenthesis(textCursor().block(), i + 1, 0/*, false*/))
                {
                    createParenthesisSelection(pos + info->position);
                    //return;
                }
            }
            else if (info->position == curPos - 1 && info->character == ')') {
                if (matchRightParenthesis(textCursor().block(), i - 1, 0, true))
                    createParenthesisSelection(pos + info->position);
            }
        }
    }
}

bool QTextEditCodeEditor::matchRightParenthesis(QTextBlock currentBlock, int i, int numRightParentheses, bool firstTime)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> parentheses = data->parentheses();

    if(!firstTime) //Set i to the end of the parentheses list.
        i = parentheses.size()-1;
    int docPos = currentBlock.position();
    for (; i > -1 && parentheses.size() > 0; --i) {
        ParenthesisInfo *info = parentheses.at(i);
        if (info->character == ')') {
            ++numRightParentheses;
            continue;
        }
        if (info->character == '(' && numRightParentheses == 0) {
            createParenthesisSelection(docPos + info->position);
            return true;
        } else
            --numRightParentheses;
    }

    currentBlock = currentBlock.previous();
    if (currentBlock.isValid())
        return matchRightParenthesis(currentBlock, 0, numRightParentheses);

    return false;
}

bool QTextEditCodeEditor::matchLeftParenthesis(QTextBlock currentBlock, int i, int numLeftParentheses, bool firstTime)
{
    TextBlockData *data = static_cast<TextBlockData *>(currentBlock.userData());
    QVector<ParenthesisInfo *> parentheses = data->parentheses();

    int docPos = currentBlock.position();
    for (; i < parentheses.size() && parentheses.size() > 0; i++) {
        ParenthesisInfo *info = parentheses.at(i);
        if (info->character == '(') {
            ++numLeftParentheses;
            continue;
        }
        if (info->character == ')' && numLeftParentheses == 0) {
            createParenthesisSelection(docPos + info->position);
            return true;
        } else
            --numLeftParentheses;
    }

    currentBlock = currentBlock.next();
    if (currentBlock.isValid())
        return matchLeftParenthesis(currentBlock, 0, numLeftParentheses);

    return false;
}


void QTextEditCodeEditor::createParenthesisSelection(int pos)
{
    QList<QTextEdit::ExtraSelection> selections = this->extraSelections();
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = this->textCursor();
    selection.format.setBackground(Qt::green);
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    selection.cursor = cursor;
    selections.append(selection);
    this->setExtraSelections(selections);
}



void QTextEditCodeEditor::nonEditableLinesToGrey()
{
    QRegExp begin_regexp = QRegExp("(^|\n)(\\s)*//# BEGIN_EDIT[^\n]*(?=\n|$)");
    QRegExp end_regexp  = QRegExp("(^|\n)(\\s)*//# END_EDIT[^\n]*(?=\n|$)");

    if (this->document()->find(begin_regexp).isNull() ||
            this->document()->find(end_regexp).isNull())
        return;

    // Change block format (will set the khaki background)

    QColor lightkaki = QColor(220, 220, 170);
    QTextCursor c = QTextCursor(this->document());


    QTextCursor from_1 = QTextCursor(this->document());
    from_1.movePosition(QTextCursor::Start);
    QTextCursor from_2 = QTextCursor(this->document());
    from_2.movePosition(QTextCursor::End);
    int begin_edit = this->document()->find(begin_regexp, from_1).blockNumber() + 1;
    int end_edit = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward).blockNumber();

    QList<QTextEdit::ExtraSelection> selections = this->extraSelections();


    // Disabled lines to grey
    this->non_editable_lines.clear();

    int nb_lines = this->document()->blockCount();
    c.movePosition(QTextCursor::Start, QTextCursor::KeepAnchor);
    for (int i=0; i < begin_edit; ++i) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lightkaki);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        c.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
        //create one selection per line ...
        selection.cursor = c;
        selections.append(selection);

        this->non_editable_lines.push_back(i);
    }

    c = this->document()->find(end_regexp, from_2, QTextDocument::FindBackward);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    for (int i=nb_lines-1; i > end_edit; --i) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lightkaki);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        //create one selection per line ...
        selection.cursor = c;
        selections.append(selection);
        c.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);

        this->non_editable_lines.push_back(i);
    }
    this->setExtraSelections(selections);
}


int QTextEditCodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 +  fontMetrics().width(QLatin1Char('9')) * (digits);

    return space;
}

void QTextEditCodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void QTextEditCodeEditor::updateLineNumberArea(QRectF /*rect_f*/)
{
    QTextEditCodeEditor::updateLineNumberArea();
}
void QTextEditCodeEditor::updateLineNumberArea(int /*slider_pos*/)
{
    QTextEditCodeEditor::updateLineNumberArea();
}
void QTextEditCodeEditor::updateLineNumberArea()
{
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    // Adjust the slider's position to alway see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());

//    // Snap to first line (TODO...)
//    if (first_block_id > 0)
//    {
//        int slider_pos = this->verticalScrollBar()->sliderPosition();
//        int prev_block_height = (int) this->document()->documentLayout()->blockBoundingRect(this->document()->findBlockByNumber(first_block_id-1)).height();
//        if (dy <= this->document()->documentMargin() + prev_block_height)
//            this->verticalScrollBar()->setSliderPosition(slider_pos - (this->document()->documentMargin() + prev_block_height));
//    }

//    QTextCursor curs = QTextCursor(this->document());
//    curs.movePosition(QTextCursor::Start);
//    int new_count = this->document()->blockCount();
//    for (int i=0; i < new_count; ++i)
//    {
//        if (curs.block().text().isEmpty()) curs.insertText(QString::fromStdString(std::to_string(i+1)));
//        curs.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
//    }

}


void QTextEditCodeEditor::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}


int QTextEditCodeEditor::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinates - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exist for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void QTextEditCodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
//        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
//                .translated(0, translate_y).intersect(this->viewport()->geometry()).height();
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    // Shift the starting point
    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    QColor col_1(90, 255, 30);      // Current line (custom green)
    QColor col_0(120, 120, 120);    // Other lines  (custom darkgrey)
    QColor col_3(Qt::darkRed);    // Non-editable lines

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            if (std::find(this->non_editable_lines.begin(), this->non_editable_lines.end(), blockNumber) != this->non_editable_lines.end()) {
                painter.setPen(col_3);
                if (blockNumber == this->document()->blockCount()-1 && block.text().isEmpty())
                    painter.setPen(Qt::lightGray);
            }
            painter.drawText(-5, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }

}



