#include "ui/PropertiesPanel.hpp"

#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLabel>

PropertiesPanel::PropertiesPanel(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QFormLayout(this);

    m_nameValue = new QLabel(this);
    m_typeValue = new QLabel(this);

    m_xEditor = new QDoubleSpinBox(this);
    m_yEditor = new QDoubleSpinBox(this);
    m_zEditor = new QDoubleSpinBox(this);

    m_lengthEditor = new QDoubleSpinBox(this);
    m_widthEditor = new QDoubleSpinBox(this);
    m_heightEditor = new QDoubleSpinBox(this);

    for (QDoubleSpinBox* editor : {
             m_xEditor,
             m_yEditor,
             m_zEditor,
             m_lengthEditor,
             m_widthEditor,
             m_heightEditor
         }) {
        editor->setRange(-1000000.0, 1000000.0);
        editor->setDecimals(2);
        editor->setSingleStep(1.0);
        editor->setSuffix(" mm");
    }

    m_lengthEditor->setMinimum(0.01);
    m_widthEditor->setMinimum(0.01);
    m_heightEditor->setMinimum(0.01);

    layout->addRow("Name:", m_nameValue);
    layout->addRow("Type:", m_typeValue);
    layout->addRow("X:", m_xEditor);
    layout->addRow("Y:", m_yEditor);
    layout->addRow("Z:", m_zEditor);
    layout->addRow("Length:", m_lengthEditor);
    layout->addRow("Width:", m_widthEditor);
    layout->addRow("Height:", m_heightEditor);

    setLayout(layout);

    connect(
        m_xEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onPositionEditorChanged
    );

    connect(
        m_yEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onPositionEditorChanged
    );

    connect(
        m_zEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onPositionEditorChanged
    );

    connect(
        m_lengthEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onBoxEditorChanged
    );

    connect(
        m_widthEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onBoxEditorChanged
    );

    connect(
        m_heightEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &PropertiesPanel::onBoxEditorChanged
    );

    showEmpty();
}

void PropertiesPanel::showEmpty()
{
    setUpdatingUi(true);

    m_nameValue->setText("-");
    m_typeValue->setText("-");

    m_xEditor->setValue(0.0);
    m_yEditor->setValue(0.0);
    m_zEditor->setValue(0.0);

    m_lengthEditor->setValue(0.01);
    m_widthEditor->setValue(0.01);
    m_heightEditor->setValue(0.01);

    setEditorsEnabled(false, false);

    setUpdatingUi(false);
}

void PropertiesPanel::showMarker(
    const QString& name,
    double x,
    double y,
    double z
)
{
    setUpdatingUi(true);

    m_nameValue->setText(name);
    m_typeValue->setText("AI Marker");

    m_xEditor->setValue(x);
    m_yEditor->setValue(y);
    m_zEditor->setValue(z);

    m_lengthEditor->setValue(0.01);
    m_widthEditor->setValue(0.01);
    m_heightEditor->setValue(0.01);

    setEditorsEnabled(true, false);

    setUpdatingUi(false);
}

void PropertiesPanel::showBox(
    const QString& name,
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
{
    setUpdatingUi(true);

    m_nameValue->setText(name);
    m_typeValue->setText("Box");

    m_xEditor->setValue(x);
    m_yEditor->setValue(y);
    m_zEditor->setValue(z);

    m_lengthEditor->setValue(length);
    m_widthEditor->setValue(width);
    m_heightEditor->setValue(height);

    setEditorsEnabled(true, true);

    setUpdatingUi(false);
}

void PropertiesPanel::onPositionEditorChanged()
{
    if (m_isUpdatingUi) {
        return;
    }

    emit positionChanged(
        m_xEditor->value(),
        m_yEditor->value(),
        m_zEditor->value()
    );
}

void PropertiesPanel::onBoxEditorChanged()
{
    if (m_isUpdatingUi) {
        return;
    }

    emit boxChanged(
        m_xEditor->value(),
        m_yEditor->value(),
        m_zEditor->value(),
        m_lengthEditor->value(),
        m_widthEditor->value(),
        m_heightEditor->value()
    );
}

void PropertiesPanel::setEditorsEnabled(
    bool positionEnabled,
    bool dimensionsEnabled
)
{
    m_xEditor->setEnabled(positionEnabled);
    m_yEditor->setEnabled(positionEnabled);
    m_zEditor->setEnabled(positionEnabled);

    m_lengthEditor->setEnabled(dimensionsEnabled);
    m_widthEditor->setEnabled(dimensionsEnabled);
    m_heightEditor->setEnabled(dimensionsEnabled);
}

void PropertiesPanel::setUpdatingUi(bool updating)
{
    m_isUpdatingUi = updating;
}