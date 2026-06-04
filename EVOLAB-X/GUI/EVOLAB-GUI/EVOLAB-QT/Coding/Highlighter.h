#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QtWidgets>

#include <QHash>
#include <QTextCharFormat>

#include <QTextBlockUserData>


#include "Models/PointerLessModels.h"


class QTextDocument;


// Parenthesis
struct ParenthesisInfo
{
    char character;
    int position;
};

class TextBlockData : public QTextBlockUserData
{
public:
    //TextBlockData();
    QVector<ParenthesisInfo *> parentheses() {
        return this->m_parentheses;
    }

    void insert(ParenthesisInfo *info) {
        bool inserted = false;
        for (int i=0; i < this->m_parentheses.size(); i++)
        {
            if (this->m_parentheses.at(i)->position > info->position) {
                this->m_parentheses.insert(i, info);
                inserted = true;
                break;
            }
        }
        if (!inserted)
            this->m_parentheses.push_back(info);
    }

private:
    QVector<ParenthesisInfo *> m_parentheses;
};


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat editLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    // Std "<cmath>"
    QTextCharFormat stdMathFormat;
    QTextCharFormat stdMathFuncFormat;
    // MACROs format
    QTextCharFormat macroFormat;
    QTextCharFormat macroFuncFormat;
    // Custom MACROs format
    QTextCharFormat customMacroFormat;
    QTextCharFormat customMacroFuncFormat;


    // Compare rule by name
    struct compare_rule : public std::unary_function<HighlightingRule, bool>
    {
        explicit compare_rule(const HighlightingRule &r_rule) : r_rule(r_rule) {}
        bool operator() (const HighlightingRule &l_rule)
        {
            return (l_rule.pattern == r_rule.pattern);
        }
        HighlightingRule r_rule;
    };

    bool RemoveMacroHighlightingRule(QString macro_keyword);
    void AddMacroHighlightingRule(QString macro_keyword, PointerLessModels::VarType type);
};

#endif
