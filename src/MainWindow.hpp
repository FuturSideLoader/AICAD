#pragma once

#include <QMainWindow>

#include "cad/CadDocument.hpp"

class QCloseEvent;
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

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();

    void addAiMarker();
    void deleteSelectedMarker();

    void onMarkerAdded(std::shared_ptr<AiMarker> marker);
    void onMarkerUpdated(std::shared_ptr<AiMarker> marker);
    void onMarkerRemoved(int markerId);
    void onDocumentCleared();

    void onObjectSelected(QListWidgetItem* item);
    void onPositionEditorChanged();
    void selectMarkerById(int markerId);

private:
    void createMenus();
    void createObjectPanel();
    void createPropertiesPanel();

    bool saveDocumentToFile(const QString& filePath);
    bool loadDocumentFromFile(const QString& filePath);

    bool maybeSaveBeforeDestructiveAction();
    void setDocumentModified(bool modified);
    void updateWindowTitle();

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

    QString m_currentFilePath;
    bool m_documentModified = false;

    int m_selectedMarkerId = 0;
    bool m_isUpdatingPropertiesUi = false;
};