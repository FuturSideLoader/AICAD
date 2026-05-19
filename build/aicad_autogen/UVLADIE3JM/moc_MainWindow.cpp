/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/MainWindow.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "newDocument",
        "",
        "openDocument",
        "saveDocument",
        "saveDocumentAs",
        "undo",
        "redo",
        "addAiMarker",
        "addBox",
        "deleteSelectedObject",
        "onBoxRemoved",
        "boxId",
        "onMarkerAdded",
        "std::shared_ptr<AiMarker>",
        "marker",
        "onMarkerUpdated",
        "onMarkerRemoved",
        "markerId",
        "onBoxAdded",
        "std::shared_ptr<CadBox>",
        "box",
        "onBoxUpdated",
        "onDocumentCleared",
        "onObjectSelected",
        "objectId",
        "onPositionChanged",
        "x",
        "y",
        "z",
        "onBoxChanged",
        "length",
        "width",
        "height",
        "selectMarkerById",
        "selectObjectById",
        "PickedObjectKind",
        "kind"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'newDocument'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openDocument'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveDocument'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveDocumentAs'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'undo'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'redo'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'addAiMarker'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'addBox'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deleteSelectedObject'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBoxRemoved'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 12 },
        }}),
        // Slot 'onMarkerAdded'
        QtMocHelpers::SlotData<void(std::shared_ptr<AiMarker>)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'onMarkerUpdated'
        QtMocHelpers::SlotData<void(std::shared_ptr<AiMarker>)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'onMarkerRemoved'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'onBoxAdded'
        QtMocHelpers::SlotData<void(std::shared_ptr<CadBox>)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onBoxUpdated'
        QtMocHelpers::SlotData<void(std::shared_ptr<CadBox>)>(22, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Slot 'onDocumentCleared'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onObjectSelected'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
        // Slot 'onPositionChanged'
        QtMocHelpers::SlotData<void(double, double, double)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 27 }, { QMetaType::Double, 28 }, { QMetaType::Double, 29 },
        }}),
        // Slot 'onBoxChanged'
        QtMocHelpers::SlotData<void(double, double, double, double, double, double)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 27 }, { QMetaType::Double, 28 }, { QMetaType::Double, 29 }, { QMetaType::Double, 31 },
            { QMetaType::Double, 32 }, { QMetaType::Double, 33 },
        }}),
        // Slot 'selectMarkerById'
        QtMocHelpers::SlotData<void(int)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'selectObjectById'
        QtMocHelpers::SlotData<void(int, PickedObjectKind)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 25 }, { 0x80000000 | 36, 37 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newDocument(); break;
        case 1: _t->openDocument(); break;
        case 2: _t->saveDocument(); break;
        case 3: _t->saveDocumentAs(); break;
        case 4: _t->undo(); break;
        case 5: _t->redo(); break;
        case 6: _t->addAiMarker(); break;
        case 7: _t->addBox(); break;
        case 8: _t->deleteSelectedObject(); break;
        case 9: _t->onBoxRemoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 10: _t->onMarkerAdded((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<AiMarker>>>(_a[1]))); break;
        case 11: _t->onMarkerUpdated((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<AiMarker>>>(_a[1]))); break;
        case 12: _t->onMarkerRemoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->onBoxAdded((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<CadBox>>>(_a[1]))); break;
        case 14: _t->onBoxUpdated((*reinterpret_cast<std::add_pointer_t<std::shared_ptr<CadBox>>>(_a[1]))); break;
        case 15: _t->onDocumentCleared(); break;
        case 16: _t->onObjectSelected((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 17: _t->onPositionChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 18: _t->onBoxChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[6]))); break;
        case 19: _t->selectMarkerById((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->selectObjectById((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<PickedObjectKind>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
