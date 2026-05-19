#pragma once

#include <QJsonObject>
#include <QMainWindow>

#include "cad/CadDocument.hpp"

#include "OccView.hpp"
#include "ui/PropertiesPanel.hpp"




class QCloseEvent;
class OccView;
class QAction;
class PropertiesPanel;
class ObjectTreePanel;

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

    void onObjectSelected(int objectId);
    void onPositionChanged(double x, double y, double z);

    void onBoxChanged(
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

    void selectMarkerById(int markerId);

    void selectObjectById(int objectId, PickedObjectKind kind);

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


    void createToolbar();

private:
    CadDocument m_document;

    OccView* m_view = nullptr;

    ObjectTreePanel* m_objectTreePanel = nullptr;

    PropertiesPanel* m_propertiesPanel = nullptr;

    QString m_currentFilePath;
    bool m_documentModified = false;

    QVector<QJsonObject> m_undoStack;
    QVector<QJsonObject> m_redoStack;
    bool m_isRestoringSnapshot = false;

    int m_selectedObjectId = 0;
    SelectedObjectKind m_selectedObjectKind = SelectedObjectKind::None;


    QAction* m_newAction = nullptr;
    QAction* m_openAction = nullptr;
    QAction* m_saveAction = nullptr;
    QAction* m_saveAsAction = nullptr;

    QAction* m_undoAction = nullptr;
    QAction* m_redoAction = nullptr;
    QAction* m_deleteAction = nullptr;

    QAction* m_fitAllAction = nullptr;

    QAction* m_addMarkerAction = nullptr;
    QAction* m_createBoxAction = nullptr;
};
