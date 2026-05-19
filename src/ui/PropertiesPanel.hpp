#pragma once

#include <QWidget>

class QLabel;
class QDoubleSpinBox;

class PropertiesPanel final : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesPanel(QWidget* parent = nullptr);

    void showEmpty();

    void showMarker(
        const QString& name,
        double x,
        double y,
        double z
    );

    void showBox(
        const QString& name,
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

signals:
    void positionChanged(double x, double y, double z);

    void boxChanged(
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

private slots:
    void onPositionEditorChanged();
    void onBoxEditorChanged();

private:
    void setEditorsEnabled(bool positionEnabled, bool dimensionsEnabled);
    void setUpdatingUi(bool updating);

private:
    QLabel* m_nameValue = nullptr;
    QLabel* m_typeValue = nullptr;

    QDoubleSpinBox* m_xEditor = nullptr;
    QDoubleSpinBox* m_yEditor = nullptr;
    QDoubleSpinBox* m_zEditor = nullptr;

    QDoubleSpinBox* m_lengthEditor = nullptr;
    QDoubleSpinBox* m_widthEditor = nullptr;
    QDoubleSpinBox* m_heightEditor = nullptr;

    bool m_isUpdatingUi = false;
};