#include "MainWindow.hpp"
#include "OccView.hpp"

#include <QAction>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QWidget>
#include <QCloseEvent>
#include <QFileInfo>
#include <QToolBar>
#include <QIcon>
#include "ui/PropertiesPanel.hpp"
#include "ui/ObjectTreePanel.hpp"
#include "commands/AddMarkerCommand.hpp"
#include <memory>
#include "commands/AddBoxCommand.hpp"
#include "commands/DeleteObjectCommand.hpp"
#include "commands/UpdateMarkerCommand.hpp"
#include "commands/UpdateBoxCommand.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_document(this),
      m_commandStack(this),
      m_view(new OccView(this))
{
    updateWindowTitle();

    setCentralWidget(m_view);

    createMenus();
    createToolbar();
    createObjectPanel();
    createPropertiesPanel();

    connect(
        &m_document,
        &CadDocument::markerAdded,
        m_view,
        &OccView::displayMarker
    );

    connect(
        &m_document,
        &CadDocument::markerAdded,
        this,
        &MainWindow::onMarkerAdded
    );

    connect(
        &m_document,
        &CadDocument::markerUpdated,
        this,
        &MainWindow::onMarkerUpdated
    );

    connect(
        &m_document,
        &CadDocument::markerUpdated,
        m_view,
        &OccView::updateMarkerDisplay
    );

    connect(
        &m_document,
        &CadDocument::markerRemoved,
        this,
        &MainWindow::onMarkerRemoved
    );

    connect(
        &m_document,
        &CadDocument::markerRemoved,
        m_view,
        &OccView::removeMarkerDisplay
    );

    connect(
        &m_document,
        &CadDocument::documentCleared,
        this,
        &MainWindow::onDocumentCleared
    );

    connect(
        &m_document,
        &CadDocument::documentCleared,
        m_view,
        &OccView::clearSceneObjects
    );

    connect(
        m_view,
        &OccView::markerPicked,
        m_view,
        &OccView::selectMarkerVisual
    );

    connect(
        m_objectTreePanel,
        &ObjectTreePanel::objectSelected,
        this,
        &MainWindow::onObjectSelected
    );

    connect(
        &m_document,
        &CadDocument::boxAdded,
        m_view,
        &OccView::displayBox
    );

    connect(
        &m_document,
        &CadDocument::boxAdded,
        this,
        &MainWindow::onBoxAdded
    );

    connect(
        &m_document,
        &CadDocument::boxUpdated,
        m_view,
        &OccView::updateBoxDisplay
    );

    connect(
        &m_document,
        &CadDocument::boxUpdated,
        this,
        &MainWindow::onBoxUpdated
    );

    connect(
        m_view,
        &OccView::objectPicked,
        this,
        &MainWindow::selectObjectById
    );

    connect(
        &m_commandStack,
        &CommandStack::changed,
        this,
        [this]() {
            if (m_undoAction != nullptr) {
                m_undoAction->setText(m_commandStack.undoText());
                m_undoAction->setEnabled(m_commandStack.canUndo());
            }

            if (m_redoAction != nullptr) {
                m_redoAction->setText(m_commandStack.redoText());
                m_redoAction->setEnabled(m_commandStack.canRedo());
            }
        }
    );

    clearPropertiesPanel();

    m_commandStack.clear();

    statusBar()->showMessage("AICAD V0.11 - Document save/load ready");
}

void MainWindow::createMenus()
{
    m_newAction = new QAction(QIcon(":/icons/resources/icons/new.svg"), "New", this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newDocument);

    m_openAction = new QAction(QIcon(":/icons/resources/icons/open.svg"), "Open...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openDocument);

    m_saveAction = new QAction(QIcon(":/icons/resources/icons/save.svg"), "Save", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveDocument);

    m_saveAsAction = new QAction(QIcon(":/icons/resources/icons/save.svg"), "Save As...", this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveDocumentAs);

    m_undoAction = new QAction(QIcon(":/icons/resources/icons/undo.svg"), "Undo", this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::undo);

    m_redoAction = new QAction(QIcon(":/icons/resources/icons/redo.svg"), "Redo", this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::redo);

    m_deleteAction = new QAction(QIcon(":/icons/resources/icons/delete.svg"), "Delete Selected Object", this);
    m_deleteAction->setShortcut(QKeySequence::Delete);
    connect(m_deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedObject);

    m_fitAllAction = new QAction(QIcon(":/icons/resources/icons/fit.svg"), "Fit All", this);
    connect(m_fitAllAction, &QAction::triggered, m_view, &OccView::fitAll);

    m_addMarkerAction = new QAction(QIcon(":/icons/resources/icons/marker.svg"), "Add Marker", this);
    connect(m_addMarkerAction, &QAction::triggered, this, &MainWindow::addAiMarker);

    m_createBoxAction = new QAction(QIcon(":/icons/resources/icons/box.svg"), "Box", this);
    connect(m_createBoxAction, &QAction::triggered, this, &MainWindow::addBox);

    auto* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    auto* editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_deleteAction);

    auto* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction(m_fitAllAction);

    auto* createMenu = menuBar()->addMenu("&Create");
    createMenu->addAction(m_createBoxAction);

    auto* aiMenu = menuBar()->addMenu("&AI");
    aiMenu->addAction(m_addMarkerAction);
}

void MainWindow::createToolbar()
{
    auto* mainToolBar = addToolBar("Main Toolbar");
    mainToolBar->setMovable(true);
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mainToolBar->addAction(m_newAction);
    mainToolBar->addAction(m_openAction);
    mainToolBar->addAction(m_saveAction);

    mainToolBar->addSeparator();

    mainToolBar->addAction(m_undoAction);
    mainToolBar->addAction(m_redoAction);

    mainToolBar->addSeparator();

    mainToolBar->addAction(m_addMarkerAction);
    mainToolBar->addAction(m_createBoxAction);

    mainToolBar->addSeparator();

    mainToolBar->addAction(m_deleteAction);

    mainToolBar->addSeparator();

    mainToolBar->addAction(m_fitAllAction);
}

void MainWindow::createObjectPanel()
{
    auto* dock = new QDockWidget("Objects", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_objectTreePanel = new ObjectTreePanel(dock);
    dock->setWidget(m_objectTreePanel);

    addDockWidget(Qt::LeftDockWidgetArea, dock);

    connect(
        m_objectTreePanel,
        &ObjectTreePanel::objectSelected,
        this,
        &MainWindow::onObjectSelected
    );
}

void MainWindow::createPropertiesPanel()
{
    auto* dock = new QDockWidget("Properties", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_propertiesPanel = new PropertiesPanel(dock);
    dock->setWidget(m_propertiesPanel);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(
        m_propertiesPanel,
        &PropertiesPanel::positionChanged,
        this,
        &MainWindow::onPositionChanged
    );

    connect(
        m_propertiesPanel,
        &PropertiesPanel::boxChanged,
        this,
        &MainWindow::onBoxChanged
    );
}

void MainWindow::newDocument()
{
    if (!maybeSaveBeforeDestructiveAction()) {
        return;
    }

    m_currentFilePath.clear();

    m_commandStack.clear();

    m_document.clear();
    setDocumentModified(false);

    statusBar()->showMessage("New document created");
}

void MainWindow::openDocument()
{
    if (!maybeSaveBeforeDestructiveAction()) {
        return;
    }

    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open AICAD document",
        QString(),
        "AICAD Documents (*.aicad);;JSON Files (*.json);;All Files (*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (!loadDocumentFromFile(filePath)) {
        QMessageBox::warning(
            this,
            "Open failed",
            "Could not open this AICAD document."
        );
        return;
    }

    m_currentFilePath = filePath;
    setDocumentModified(false);

    m_commandStack.clear();

    statusBar()->showMessage(QString("Opened %1").arg(filePath));
}

void MainWindow::saveDocument()
{
    if (m_currentFilePath.isEmpty()) {
        saveDocumentAs();
        return;
    }

    if (!saveDocumentToFile(m_currentFilePath)) {
        QMessageBox::warning(
            this,
            "Save failed",
            "Could not save this AICAD document."
        );
        return;
    }

    setDocumentModified(false);
    statusBar()->showMessage(QString("Saved %1").arg(m_currentFilePath));
}

void MainWindow::saveDocumentAs()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Save AICAD document",
        m_currentFilePath,
        "AICAD Documents (*.aicad);;JSON Files (*.json);;All Files (*)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".aicad", Qt::CaseInsensitive)
        && !filePath.endsWith(".json", Qt::CaseInsensitive)) {
        filePath += ".aicad";
    }

    if (!saveDocumentToFile(filePath)) {
        QMessageBox::warning(
            this,
            "Save failed",
            "Could not save this AICAD document."
        );
        return;
    }

    m_currentFilePath = filePath;
    setDocumentModified(false);

    statusBar()->showMessage(QString("Saved %1").arg(filePath));
}

void MainWindow::undo()
{
    if (!m_commandStack.canUndo()) {
        statusBar()->showMessage("Nothing to undo");
        return;
    }

    if (!m_commandStack.undo()) {
        statusBar()->showMessage("Undo failed");
        return;
    }

    setDocumentModified(true);
    statusBar()->showMessage("Undo");
}

void MainWindow::redo()
{
    if (!m_commandStack.canRedo()) {
        statusBar()->showMessage("Nothing to redo");
        return;
    }

    if (!m_commandStack.redo()) {
        statusBar()->showMessage("Redo failed");
        return;
    }

    setDocumentModified(true);
    statusBar()->showMessage("Redo");
}

bool MainWindow::saveDocumentToFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    const QJsonDocument jsonDocument(m_document.toJson());

    file.write(jsonDocument.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool MainWindow::loadDocumentFromFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument jsonDocument =
        QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        return false;
    }

    if (!jsonDocument.isObject()) {
        return false;
    }

    return m_document.loadFromJson(jsonDocument.object());
}

void MainWindow::addAiMarker()
{
    const int existingMarkerCount =
        static_cast<int>(m_document.markers().size());

    const double offset = static_cast<double>(existingMarkerCount) * 20.0;

    auto command = std::make_unique<AddMarkerCommand>(
        m_document,
        20.0 + offset,
        20.0 + offset,
        80.0
    );

    if (!m_commandStack.executeCommand(std::move(command))) {
        statusBar()->showMessage("Failed to add marker");
        return;
    }

    setDocumentModified(true);
    statusBar()->showMessage("Marker added");
}

void MainWindow::deleteSelectedObject()
{
    if (m_selectedObjectKind == SelectedObjectKind::None
        || m_selectedObjectId <= 0) {
        statusBar()->showMessage("No object selected");
        return;
    }

    DeleteObjectKind commandKind = DeleteObjectKind::None;

    if (m_selectedObjectKind == SelectedObjectKind::Marker) {
        commandKind = DeleteObjectKind::Marker;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box) {
        commandKind = DeleteObjectKind::Box;
    }

    if (commandKind == DeleteObjectKind::None) {
        statusBar()->showMessage("Unsupported object type");
        return;
    }

    auto command = std::make_unique<DeleteObjectCommand>(
        m_document,
        m_selectedObjectId,
        commandKind
    );

    if (!m_commandStack.executeCommand(std::move(command))) {
        statusBar()->showMessage("Failed to delete selected object");
        return;
    }

    clearPropertiesPanel();

    if (m_view != nullptr) {
        m_view->selectObjectVisual(0, PickedObjectKind::None);
    }

    setDocumentModified(true);
    statusBar()->showMessage("Object deleted");
}

void MainWindow::onMarkerAdded(std::shared_ptr<AiMarker> marker)
{
    if (marker == nullptr || m_objectTreePanel == nullptr) {
        return;
    }

    m_objectTreePanel->addOrUpdateMarker(marker);
}

void MainWindow::onMarkerUpdated(std::shared_ptr<AiMarker> marker)
{
    if (marker == nullptr) {
        return;
    }

    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->addOrUpdateMarker(marker);
    }

    if (marker->id() == m_selectedObjectId) {
        showMarkerProperties(marker);
    }

    statusBar()->showMessage(
        QString("%1 updated: X=%2 Y=%3 Z=%4")
            .arg(marker->name())
            .arg(marker->x())
            .arg(marker->y())
            .arg(marker->z())
    );
}

void MainWindow::onMarkerRemoved(int markerId)
{
    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->removeObject(markerId);
    }

    if (m_selectedObjectKind == SelectedObjectKind::Marker
        && m_selectedObjectId == markerId) {
        clearPropertiesPanel();
    }

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
    }
}
void MainWindow::onDocumentCleared()
{
    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->clearObjects();
    }

    clearPropertiesPanel();

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
    }
}

void MainWindow::onObjectSelected(int objectId)
{
    if (objectId <= 0) {
        clearPropertiesPanel();

        if (m_view != nullptr) {
            m_view->selectObjectVisual(0, PickedObjectKind::None);
        }

        return;
    }

    std::shared_ptr<AiMarker> marker = m_document.findMarkerById(objectId);

    if (marker != nullptr) {
        showMarkerProperties(marker);

        if (m_view != nullptr) {
            m_view->selectObjectVisual(marker->id(), PickedObjectKind::Marker);
        }

        return;
    }

    std::shared_ptr<CadBox> box = m_document.findBoxById(objectId);

    if (box != nullptr) {
        showBoxProperties(box);

        if (m_view != nullptr) {
            m_view->selectObjectVisual(box->id(), PickedObjectKind::Box);
        }

        return;
    }

    clearPropertiesPanel();

    if (m_view != nullptr) {
        m_view->selectObjectVisual(0, PickedObjectKind::None);
    }
}

void MainWindow::onPositionChanged(double x, double y, double z)
{
    if (m_selectedObjectId <= 0) {
        return;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Marker) {
        std::shared_ptr<AiMarker> marker =
            m_document.findMarkerById(m_selectedObjectId);

        if (marker == nullptr) {
            return;
        }

        const bool unchanged =
            marker->x() == x
            && marker->y() == y
            && marker->z() == z;

        if (unchanged) {
            return;
        }

        auto command = std::make_unique<UpdateMarkerCommand>(
            m_document,
            marker->id(),
            marker->x(),
            marker->y(),
            marker->z(),
            x,
            y,
            z
        );

        if (!m_commandStack.executeCommand(std::move(command))) {
            statusBar()->showMessage("Failed to update marker");
            return;
        }

        setDocumentModified(true);
        statusBar()->showMessage("Marker updated");

        return;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box) {
        std::shared_ptr<CadBox> box =
            m_document.findBoxById(m_selectedObjectId);

        if (box == nullptr) {
            return;
        }

        const bool unchanged =
            box->x() == x
            && box->y() == y
            && box->z() == z;

        if (unchanged) {
            return;
        }

        auto command = std::make_unique<UpdateBoxCommand>(
            m_document,
            box->id(),

            box->x(),
            box->y(),
            box->z(),
            box->length(),
            box->width(),
            box->height(),

            x,
            y,
            z,
            box->length(),
            box->width(),
            box->height()
        );

        if (!m_commandStack.executeCommand(std::move(command))) {
            statusBar()->showMessage("Failed to update box position");
            return;
        }

        setDocumentModified(true);
        statusBar()->showMessage("Box updated");
    }
}

void MainWindow::onBoxChanged(
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
{
    if (m_selectedObjectId <= 0
        || m_selectedObjectKind != SelectedObjectKind::Box) {
        return;
    }

    std::shared_ptr<CadBox> box =
        m_document.findBoxById(m_selectedObjectId);

    if (box == nullptr) {
        return;
    }

    const bool unchanged =
        box->x() == x
        && box->y() == y
        && box->z() == z
        && box->length() == length
        && box->width() == width
        && box->height() == height;

    if (unchanged) {
        return;
    }

    auto command = std::make_unique<UpdateBoxCommand>(
        m_document,
        box->id(),

        box->x(),
        box->y(),
        box->z(),
        box->length(),
        box->width(),
        box->height(),

        x,
        y,
        z,
        length,
        width,
        height
    );

    if (!m_commandStack.executeCommand(std::move(command))) {
        statusBar()->showMessage("Failed to update box");
        return;
    }

    setDocumentModified(true);
    statusBar()->showMessage("Box updated");
}

void MainWindow::selectMarkerById(int markerId)
{
    selectObjectById(markerId, PickedObjectKind::Marker);
}

void MainWindow::showMarkerProperties(const std::shared_ptr<AiMarker>& marker)
{
    if (marker == nullptr || m_propertiesPanel == nullptr) {
        clearPropertiesPanel();
        return;
    }

    m_selectedObjectId = marker->id();
    m_selectedObjectKind = SelectedObjectKind::Marker;

    m_propertiesPanel->showMarker(
        marker->name(),
        marker->x(),
        marker->y(),
        marker->z()
    );

    statusBar()->showMessage(QString("%1 selected").arg(marker->name()));
}

void MainWindow::clearPropertiesPanel()
{
    m_selectedObjectId = 0;
    m_selectedObjectKind = SelectedObjectKind::None;

    if (m_propertiesPanel != nullptr) {
        m_propertiesPanel->showEmpty();
    }
}




bool MainWindow::maybeSaveBeforeDestructiveAction()
{
    if (!m_documentModified) {
        return true;
    }

    const QMessageBox::StandardButton result = QMessageBox::question(
        this,
        "Unsaved changes",
        "The current document has unsaved changes. Do you want to save it?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save
    );

    if (result == QMessageBox::Cancel) {
        return false;
    }

    if (result == QMessageBox::Discard) {
        return true;
    }

    saveDocument();

    return !m_documentModified;
}

void MainWindow::setDocumentModified(bool modified)
{
    m_documentModified = modified;
    updateWindowTitle();
}

void MainWindow::updateWindowTitle()
{
    QString documentName = "Untitled";

    if (!m_currentFilePath.isEmpty()) {
        documentName = QFileInfo(m_currentFilePath).fileName();
    }

    if (m_documentModified) {
        documentName += "*";
    }

    setWindowTitle(QString("AICAD - %1").arg(documentName));
}



void MainWindow::rebuildObjectPanelFromDocument()
{
    if (m_objectTreePanel == nullptr) {
        return;
    }

    m_objectTreePanel->rebuild(
        m_document.markers(),
        m_document.boxes()
    );
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!maybeSaveBeforeDestructiveAction()) {
        event->ignore();
        return;
    }

    event->accept();
}


void MainWindow::addBox()
{
    auto command = std::make_unique<AddBoxCommand>(
        m_document,
        0.0,
        0.0,
        0.0,
        100.0,
        80.0,
        60.0
    );

    if (!m_commandStack.executeCommand(std::move(command))) {
        statusBar()->showMessage("Failed to create box");
        return;
    }

    setDocumentModified(true);
    statusBar()->showMessage("Box created");
}


void MainWindow::onBoxAdded(std::shared_ptr<CadBox> box)
{
    if (box == nullptr || m_objectTreePanel == nullptr) {
        return;
    }

    m_objectTreePanel->addOrUpdateBox(box);
}

void MainWindow::showBoxProperties(const std::shared_ptr<CadBox>& box)
{
    if (box == nullptr || m_propertiesPanel == nullptr) {
        clearPropertiesPanel();
        return;
    }

    m_selectedObjectId = box->id();
    m_selectedObjectKind = SelectedObjectKind::Box;

    m_propertiesPanel->showBox(
        box->name(),
        box->x(),
        box->y(),
        box->z(),
        box->length(),
        box->width(),
        box->height()
    );

    statusBar()->showMessage(QString("%1 selected").arg(box->name()));
}


void MainWindow::onBoxUpdated(std::shared_ptr<CadBox> box)
{
    if (box == nullptr) {
        return;
    }

    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->addOrUpdateBox(box);
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box
        && box->id() == m_selectedObjectId) {
        showBoxProperties(box);
    }

    statusBar()->showMessage(
        QString("%1 updated").arg(box->name())
    );
}


void MainWindow::onBoxRemoved(int boxId)
{
    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->removeObject(boxId);
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box
        && m_selectedObjectId == boxId) {
        clearPropertiesPanel();
    }

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
    }
}

void MainWindow::selectObjectById(int objectId, PickedObjectKind kind)
{
    if (objectId <= 0 || kind == PickedObjectKind::None) {
        clearPropertiesPanel();

        if (m_view != nullptr) {
            m_view->selectObjectVisual(0, PickedObjectKind::None);
        }

        return;
    }

    if (kind == PickedObjectKind::Marker) {
        std::shared_ptr<AiMarker> marker = m_document.findMarkerById(objectId);

        if (marker == nullptr) {
            clearPropertiesPanel();
            return;
        }

        showMarkerProperties(marker);

        if (m_view != nullptr) {
            m_view->selectObjectVisual(objectId, PickedObjectKind::Marker);
        }
    }

    if (kind == PickedObjectKind::Box) {
        std::shared_ptr<CadBox> box = m_document.findBoxById(objectId);

        if (box == nullptr) {
            clearPropertiesPanel();
            return;
        }

        showBoxProperties(box);

        if (m_view != nullptr) {
            m_view->selectObjectVisual(objectId, PickedObjectKind::Box);
        }
    }

    if (m_objectTreePanel != nullptr) {
        m_objectTreePanel->selectObject(objectId);
    }
}
