#pragma once

#include <QJsonObject>
#include <QMainWindow>

#include "cad/CadDocument.hpp"

class QCloseEvent;
class QDoubleSpinBox;
class QLabel;
class QListWidget;
class QListWidgetItem;
class OccView;

enum class SelectedObjectKind
{
    None,
    Marker,
    Box
};

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

    void undo();
    void redo();

    void addAiMarker();
    void addBox();
    void deleteSelectedObject();

    void onBoxRemoved(int boxId);

    void onMarkerAdded(std::shared_ptr<AiMarker> marker);
    void onMarkerUpdated(std::shared_ptr<AiMarker> marker);
    void onMarkerRemoved(int markerId);

    void onBoxAdded(std::shared_ptr<CadBox> box);
    void onBoxUpdated(std::shared_ptr<CadBox> box);

    void onDocumentCleared();

    void onObjectSelected(QListWidgetItem* item);
    void onPositionEditorChanged();
    void onBoxEditorChanged();

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

    void saveUndoSnapshot();
    void restoreDocumentSnapshot(const QJsonObject& snapshot);
    void rebuildObjectPanelFromDocument();

    void showMarkerProperties(const std::shared_ptr<AiMarker>& marker);
    void showBoxProperties(const std::shared_ptr<CadBox>& box);
    void clearPropertiesPanel();

    void updateObjectListItem(const std::shared_ptr<AiMarker>& marker);
    void updateObjectListItem(const std::shared_ptr<CadBox>& box);
    void removeObjectListItem(int objectId);

private:
    CadDocument m_document;

    OccView* m_view = nullptr;

    QListWidget* m_objectList = nullptr;

    QLabel* m_propertyNameValue = nullptr;
    QLabel* m_propertyTypeValue = nullptr;

    QDoubleSpinBox* m_propertyXEditor = nullptr;
    QDoubleSpinBox* m_propertyYEditor = nullptr;
    QDoubleSpinBox* m_propertyZEditor = nullptr;

    QDoubleSpinBox* m_propertyLengthEditor = nullptr;
    QDoubleSpinBox* m_propertyWidthEditor = nullptr;
    QDoubleSpinBox* m_propertyHeightEditor = nullptr;

    QString m_currentFilePath;
    bool m_documentModified = false;

    QVector<QJsonObject> m_undoStack;
    QVector<QJsonObject> m_redoStack;
    bool m_isRestoringSnapshot = false;

    int m_selectedObjectId = 0;
    SelectedObjectKind m_selectedObjectKind = SelectedObjectKind::None;

    bool m_isUpdatingPropertiesUi = false;
};