#pragma once

#include <QMainWindow>

#include "cad/CadDocument.hpp"

class QDoubleSpinBox;
class QLabel;
class QListWidget;
class QListWidgetItem;
class OccView;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void addAiMarker();
    void deleteSelectedMarker();

    void onMarkerAdded(std::shared_ptr<AiMarker> marker);
    void onMarkerUpdated(std::shared_ptr<AiMarker> marker);
    void onMarkerRemoved(int markerId);

    void onObjectSelected(QListWidgetItem* item);
    void onPositionEditorChanged();
    void selectMarkerById(int markerId);

private:
    void createMenus();
    void createObjectPanel();
    void createPropertiesPanel();

    void showMarkerProperties(const std::shared_ptr<AiMarker>& marker);
    void clearPropertiesPanel();
    void updateObjectListItem(const std::shared_ptr<AiMarker>& marker);
    void removeObjectListItem(int markerId);

private:
    CadDocument m_document;

    OccView* m_view = nullptr;

    QListWidget* m_objectList = nullptr;

    QLabel* m_propertyNameValue = nullptr;
    QLabel* m_propertyTypeValue = nullptr;

    QDoubleSpinBox* m_propertyXEditor = nullptr;
    QDoubleSpinBox* m_propertyYEditor = nullptr;
    QDoubleSpinBox* m_propertyZEditor = nullptr;

    int m_selectedMarkerId = 0;
    bool m_isUpdatingPropertiesUi = false;
};