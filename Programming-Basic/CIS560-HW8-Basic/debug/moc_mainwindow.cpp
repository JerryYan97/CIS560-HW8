/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[36];
    char stringdata0[785];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 27), // "sign_HighLightRelatedJoints"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(4, 57, 1), // "i"
QT_MOC_LITERAL(5, 59, 16), // "slot_extrudeFace"
QT_MOC_LITERAL(6, 76, 24), // "slot_addMeshToListWidget"
QT_MOC_LITERAL(7, 101, 5), // "Mesh&"
QT_MOC_LITERAL(8, 107, 5), // "iMesh"
QT_MOC_LITERAL(9, 113, 25), // "slot_freeVerticesPinboxes"
QT_MOC_LITERAL(10, 139, 22), // "slot_freeFacesPinboxes"
QT_MOC_LITERAL(11, 162, 22), // "slot_slientAllPinboxes"
QT_MOC_LITERAL(12, 185, 25), // "slot_VertexPositionXValue"
QT_MOC_LITERAL(13, 211, 25), // "slot_VertexPositionYValue"
QT_MOC_LITERAL(14, 237, 25), // "slot_VertexPositionZValue"
QT_MOC_LITERAL(15, 263, 22), // "slot_FaceColorRedValue"
QT_MOC_LITERAL(16, 286, 24), // "slot_FaceColorGreenValue"
QT_MOC_LITERAL(17, 311, 23), // "slot_FaceColorBlueValue"
QT_MOC_LITERAL(18, 335, 20), // "slot_highlightVertex"
QT_MOC_LITERAL(19, 356, 18), // "slot_highlightFace"
QT_MOC_LITERAL(20, 375, 22), // "slot_highlightHalfEdge"
QT_MOC_LITERAL(21, 398, 19), // "slot_highlightJoint"
QT_MOC_LITERAL(22, 418, 18), // "slot_splitCubeEdge"
QT_MOC_LITERAL(23, 437, 24), // "slot_triangulateCubeFace"
QT_MOC_LITERAL(24, 462, 15), // "slot_smoothMesh"
QT_MOC_LITERAL(25, 478, 29), // "on_actionLoad_Scene_triggered"
QT_MOC_LITERAL(26, 508, 32), // "on_actionLoad_Skeleton_triggered"
QT_MOC_LITERAL(27, 541, 34), // "on_actionCamera_Controls_trig..."
QT_MOC_LITERAL(28, 576, 28), // "slot_addSkeletonToListWidget"
QT_MOC_LITERAL(29, 605, 9), // "Skeleton&"
QT_MOC_LITERAL(30, 615, 28), // "slot_onSkinningButtonClicked"
QT_MOC_LITERAL(31, 644, 26), // "slot_jointItemOtherActions"
QT_MOC_LITERAL(32, 671, 29), // "slot_onRotationXButtonClicked"
QT_MOC_LITERAL(33, 701, 29), // "slot_onRotationYButtonClicked"
QT_MOC_LITERAL(34, 731, 29), // "slot_onRotationZButtonClicked"
QT_MOC_LITERAL(35, 761, 23) // "on_actionQuit_triggered"

    },
    "MainWindow\0sign_HighLightRelatedJoints\0"
    "\0QListWidgetItem*\0i\0slot_extrudeFace\0"
    "slot_addMeshToListWidget\0Mesh&\0iMesh\0"
    "slot_freeVerticesPinboxes\0"
    "slot_freeFacesPinboxes\0slot_slientAllPinboxes\0"
    "slot_VertexPositionXValue\0"
    "slot_VertexPositionYValue\0"
    "slot_VertexPositionZValue\0"
    "slot_FaceColorRedValue\0slot_FaceColorGreenValue\0"
    "slot_FaceColorBlueValue\0slot_highlightVertex\0"
    "slot_highlightFace\0slot_highlightHalfEdge\0"
    "slot_highlightJoint\0slot_splitCubeEdge\0"
    "slot_triangulateCubeFace\0slot_smoothMesh\0"
    "on_actionLoad_Scene_triggered\0"
    "on_actionLoad_Skeleton_triggered\0"
    "on_actionCamera_Controls_triggered\0"
    "slot_addSkeletonToListWidget\0Skeleton&\0"
    "slot_onSkinningButtonClicked\0"
    "slot_jointItemOtherActions\0"
    "slot_onRotationXButtonClicked\0"
    "slot_onRotationYButtonClicked\0"
    "slot_onRotationZButtonClicked\0"
    "on_actionQuit_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  159,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,  162,    2, 0x0a /* Public */,
       6,    1,  165,    2, 0x0a /* Public */,
       9,    1,  168,    2, 0x0a /* Public */,
      10,    1,  171,    2, 0x0a /* Public */,
      11,    1,  174,    2, 0x0a /* Public */,
      12,    1,  177,    2, 0x0a /* Public */,
      13,    1,  180,    2, 0x0a /* Public */,
      14,    1,  183,    2, 0x0a /* Public */,
      15,    1,  186,    2, 0x0a /* Public */,
      16,    1,  189,    2, 0x0a /* Public */,
      17,    1,  192,    2, 0x0a /* Public */,
      18,    1,  195,    2, 0x0a /* Public */,
      19,    1,  198,    2, 0x0a /* Public */,
      20,    1,  201,    2, 0x0a /* Public */,
      21,    1,  204,    2, 0x0a /* Public */,
      22,    1,  207,    2, 0x0a /* Public */,
      23,    1,  210,    2, 0x0a /* Public */,
      24,    1,  213,    2, 0x0a /* Public */,
      25,    1,  216,    2, 0x0a /* Public */,
      26,    1,  219,    2, 0x0a /* Public */,
      27,    0,  222,    2, 0x0a /* Public */,
      28,    1,  223,    2, 0x0a /* Public */,
      30,    1,  226,    2, 0x0a /* Public */,
      31,    1,  229,    2, 0x0a /* Public */,
      32,    1,  232,    2, 0x0a /* Public */,
      33,    1,  235,    2, 0x0a /* Public */,
      34,    1,  238,    2, 0x0a /* Public */,
      35,    0,  241,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, QMetaType::Double,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sign_HighLightRelatedJoints((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->slot_extrudeFace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->slot_addMeshToListWidget((*reinterpret_cast< Mesh(*)>(_a[1]))); break;
        case 3: _t->slot_freeVerticesPinboxes((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->slot_freeFacesPinboxes((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->slot_slientAllPinboxes((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->slot_VertexPositionXValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->slot_VertexPositionYValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->slot_VertexPositionZValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->slot_FaceColorRedValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->slot_FaceColorGreenValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->slot_FaceColorBlueValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->slot_highlightVertex((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 13: _t->slot_highlightFace((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 14: _t->slot_highlightHalfEdge((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 15: _t->slot_highlightJoint((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 16: _t->slot_splitCubeEdge((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->slot_triangulateCubeFace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: _t->slot_smoothMesh((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: _t->on_actionLoad_Scene_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->on_actionLoad_Skeleton_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->on_actionCamera_Controls_triggered(); break;
        case 22: _t->slot_addSkeletonToListWidget((*reinterpret_cast< Skeleton(*)>(_a[1]))); break;
        case 23: _t->slot_onSkinningButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->slot_jointItemOtherActions((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 25: _t->slot_onRotationXButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->slot_onRotationYButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: _t->slot_onRotationZButtonClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: _t->on_actionQuit_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(QListWidgetItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sign_HighLightRelatedJoints)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 29)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::sign_HighLightRelatedJoints(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
