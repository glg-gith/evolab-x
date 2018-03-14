#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen); //red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen); //red);

    //---
    editLineCommentFormat.setForeground(Qt::darkRed); //red);
    rule.pattern = QRegExp("//#[^\n]*");
    rule.format = editLineCommentFormat;
    highlightingRules.append(rule);
    //---

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");


    // Builtin MACROS
    macroFormat.setForeground(QColor(207, 117, 0));
    macroFormat.setFontWeight(QFont::Bold);
    macroFuncFormat.setFontItalic(true);
    macroFuncFormat.setForeground(QColor(242, 138, 0));

//    QStringList macroPatterns;
//    macroPatterns << "\\bRETURN\\b";
//    foreach (const QString &pattern, macroPatterns) {
//        // Macro
//        rule.pattern = QRegExp(pattern);
//        rule.format = macroFormat;
//        highlightingRules.append(rule);
//        // Function Macro
//        rule.pattern = QRegExp(pattern + "+(?=\\()");
//        rule.format = macroFuncFormat;
//        highlightingRules.append(rule);
//    }

    // Std math "<cmath>"
    stdMathFormat.setForeground(QColor(100, 100, 0));
    stdMathFormat.setFontWeight(QFont::Bold);
    stdMathFuncFormat.setFontItalic(true);
    stdMathFuncFormat.setForeground(QColor(110, 110, 0));

//    QStringList cmathPatterns;
//    cmathPatterns << "\\bPI\\b" << "\\bsin\\b";
//    foreach (const QString &pattern, cmathPatterns) {
//        // Constant
//        rule.pattern = QRegExp(pattern);
//        rule.format = stdMathFormat;
//        highlightingRules.append(rule);
//        // Function
//        rule.pattern = QRegExp(pattern + "+(?=\\()");
//        rule.format = stdMathFuncFormat;
//        highlightingRules.append(rule);
//    }

    // Builtin MACROS
    customMacroFormat.setForeground(QColor(60, 100, 0));
    customMacroFormat.setFontWeight(QFont::Bold);
    customMacroFuncFormat.setFontItalic(true);
    customMacroFuncFormat.setForeground(QColor(60, 75, 0));

//    QStringList customPatterns;
//    customPatterns << "\\bNB_NEIGH\\b";
//    foreach (const QString &pattern, customPatterns) {
//        // Constant
//        rule.pattern = QRegExp(pattern);
//        rule.format = customMacroFormat;
//        highlightingRules.append(rule);
//        // Function
//        rule.pattern = QRegExp(pattern + "+(?=\\()");
//        rule.format = customMacroFuncFormat;
//        highlightingRules.append(rule);
//    }

}

void Highlighter::highlightBlock(const QString &text)
{
    // Parenthesis
    TextBlockData *data = new TextBlockData;
    int leftPos = text.indexOf('(');
    while (leftPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = '(';
        info->position = leftPos;
        data->insert(info);
        leftPos = text.indexOf('(', leftPos + 1);
    }
    int rightPos = text.indexOf(')');
    while (rightPos != -1) {
        ParenthesisInfo *info = new ParenthesisInfo;
        info->character = ')';
        info->position = rightPos;
        data->insert(info);
        rightPos = text.indexOf(')', rightPos + 1);
    }
    setCurrentBlockUserData(data);

    // Highlighting
    //--------------
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
    //--------------
}

bool Highlighter::RemoveMacroHighlightingRule(QString macro_keyword)
{
    bool removed = false;
    HighlightingRule rule;

    rule.pattern = QRegExp(macro_keyword + "+(?=\\()");         // Function
    QVector<HighlightingRule>::Iterator it = std::find_if(highlightingRules.begin(), highlightingRules.end(), compare_rule(rule));
    if (it != highlightingRules.end()) {
        highlightingRules.erase(it);
        removed = true;
    }
    else
    {
        rule.pattern = QRegExp(macro_keyword);                  // Constant
        QVector<HighlightingRule>::Iterator it = std::find_if(highlightingRules.begin(), highlightingRules.end(), compare_rule(rule));
        if (it != highlightingRules.end()) {
            highlightingRules.erase(it);
            removed = true;
        }
    }
    return removed;
}

void Highlighter::AddMacroHighlightingRule(QString macro_keyword, PointerLessModels::VarType type)
{
    QString pattern = QString("\\b") + macro_keyword + QString("\\b");
    HighlightingRule rule;

    if (type == PointerLessModels::CMathBuiltin || type == PointerLessModels::ElpBuiltin) {
        // Constant
//        macroFormat.setForeground(QColor(207, 117, 0));
//        macroFormat.setFontWeight(QFont::Bold);
        rule.pattern = QRegExp(pattern);
        rule.format = macroFormat;
        highlightingRules.append(rule);
        // Function
//        macroFuncFormat.setFontItalic(true);
//        macroFuncFormat.setForeground(QColor(242, 138, 0));
        rule.pattern = QRegExp(pattern + "+(?=\\()");
        rule.format = macroFuncFormat;
        highlightingRules.append(rule);
    } else {
        // Constant
//        customMacroFormat.setForeground(QColor(60, 100, 0));
//        customMacroFormat.setFontWeight(QFont::Bold);
        rule.pattern = QRegExp(pattern);
        rule.format = customMacroFormat;
        highlightingRules.append(rule);
        // Function
//        customMacroFuncFormat.setFontItalic(true);
//        customMacroFuncFormat.setForeground(QColor(60, 75, 0));
        rule.pattern = QRegExp(pattern + "+(?=\\()");
        rule.format = customMacroFuncFormat;
        highlightingRules.append(rule);
    }
}





