/****************************************************************************
** Meta object code from reading C++ file 'qtexteditcodeeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../EVOLAB-QT/qtexteditcodeeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtexteditcodeeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QTextEditCodeEditor_t {
    QByteArrayData data[15];
    char stringdata[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QTextEditCodeEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QTextEditCodeEditor_t qt_meta_stringdata_QTextEditCodeEditor = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 17),
QT_MOC_LITERAL(2, 38, 0),
QT_MOC_LITERAL(3, 39, 12),
QT_MOC_LITERAL(4, 52, 1),
QT_MOC_LITERAL(5, 54, 15),
QT_MOC_LITERAL(6, 70, 13),
QT_MOC_LITERAL(7, 84, 10),
QT_MOC_LITERAL(8, 95, 11),
QT_MOC_LITERAL(9, 107, 13),
QT_MOC_LITERAL(10, 121, 16),
QT_MOC_LITERAL(11, 138, 22),
QT_MOC_LITERAL(12, 161, 25),
QT_MOC_LITERAL(13, 187, 13),
QT_MOC_LITERAL(14, 201, 20)
    },
    "QTextEditCodeEditor\0mouseReleaseEvent\0"
    "\0QMouseEvent*\0e\0mousePressEvent\0"
    "keyPressEvent\0QKeyEvent*\0resizeEvent\0"
    "QResizeEvent*\0matchParentheses\0"
    "nonEditableLinesToGrey\0updateLineNumberAreaWidth\0"
    "newBlockCount\0updateLineNumberArea"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QTextEditCodeEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       6,    1,   70,    2, 0x0a /* Public */,
       8,    1,   73,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x08 /* Private */,
      11,    0,   77,    2, 0x08 /* Private */,
      12,    1,   78,    2, 0x08 /* Private */,
      14,    1,   81,    2, 0x08 /* Private */,
      14,    1,   84,    2, 0x08 /* Private */,
      14,    0,   87,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    4,
    QMetaType::Void, 0x80000000 | 9,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QRectF,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void QTextEditCodeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QTextEditCodeEditor *_t = static_cast<QTextEditCodeEditor *>(_o);
        switch (_id) {
        case 0: _t->mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 1: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->keyPressEvent((*reinterpret_cast< QKeyEvent*(*)>(_a[1]))); break;
        case 3: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 4: _t->matchParentheses(); break;
        case 5: _t->nonEditableLinesToGrey(); break;
        case 6: _t->updateLineNumberAreaWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->updateLineNumberArea((*reinterpret_cast< QRectF(*)>(_a[1]))); break;
        case 8: _t->updateLineNumberArea((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->updateLineNumberArea(); break;
        default: ;
        }
    }
}

const QMetaObject QTextEditCodeEditor::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_QTextEditCodeEditor.data,
      qt_meta_data_QTextEditCodeEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *QTextEditCodeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QTextEditCodeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTextEditCodeEditor.stringdata))
        return static_cast<void*>(const_cast< QTextEditCodeEditor*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int QTextEditCodeEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
