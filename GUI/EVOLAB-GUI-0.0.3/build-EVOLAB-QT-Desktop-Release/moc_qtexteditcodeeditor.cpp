/****************************************************************************
** Meta object code from reading C++ file 'qtexteditcodeeditor.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../EVOLAB-QT/qtexteditcodeeditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtexteditcodeeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTextEditCodeEditor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   21,   20,   20, 0x0a,
      55,   21,   20,   20, 0x0a,
      85,   21,   20,   20, 0x0a,
     111,   21,   20,   20, 0x0a,
     138,   20,   20,   20, 0x08,
     157,   20,   20,   20, 0x08,
     196,  182,   20,   20, 0x08,
     227,   20,   20,   20, 0x08,
     256,   20,   20,   20, 0x08,
     282,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QTextEditCodeEditor[] = {
    "QTextEditCodeEditor\0\0e\0"
    "mouseReleaseEvent(QMouseEvent*)\0"
    "mousePressEvent(QMouseEvent*)\0"
    "keyPressEvent(QKeyEvent*)\0"
    "resizeEvent(QResizeEvent*)\0"
    "matchParentheses()\0nonEditableLinesToGrey()\0"
    "newBlockCount\0updateLineNumberAreaWidth(int)\0"
    "updateLineNumberArea(QRectF)\0"
    "updateLineNumberArea(int)\0"
    "updateLineNumberArea()\0"
};

void QTextEditCodeEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
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

const QMetaObjectExtraData QTextEditCodeEditor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTextEditCodeEditor::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_QTextEditCodeEditor,
      qt_meta_data_QTextEditCodeEditor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTextEditCodeEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTextEditCodeEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTextEditCodeEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTextEditCodeEditor))
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
    }
    return _id;
}
QT_END_MOC_NAMESPACE
