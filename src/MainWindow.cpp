#include "MainWindow.hpp"
#include "OccView.hpp"

#include <QAction>
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QJsonDocument>
#include <QKeySequence>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QVariant>
#include <QWidget>
#include <QCloseEvent>
#include <QFileInfo>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_document(this),
      m_view(new OccView(this))
{
    updateWindowTitle();

    setCentralWidget(m_view);

    createMenus();
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
        this,
        &MainWindow::selectMarkerById
    );

    connect(
        m_view,
        &OccView::markerPicked,
        m_view,
        &OccView::selectMarkerVisual
    );

    connect(
        m_objectList,
        &QListWidget::itemClicked,
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

    clearPropertiesPanel();

    statusBar()->showMessage("AICAD V0.11 - Document save/load ready");
}

void MainWindow::createMenus()
{
    auto* fileMenu = menuBar()->addMenu("&File");

    QAction* newAction = fileMenu->addAction("New");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newDocument);

    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openDocument);

    QAction* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveDocument);

    QAction* saveAsAction = fileMenu->addAction("Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveDocumentAs);

    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    auto* editMenu = menuBar()->addMenu("&Edit");

    QAction* undoAction = editMenu->addAction("undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(
        undoAction,
        &QAction::triggered,
        this,
        &MainWindow::undo
    );

    QAction* redoAction = editMenu->addAction("redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(
        redoAction,
        &QAction::triggered,
        this,
        &MainWindow::redo
    );

    editMenu->addSeparator();

    QAction* deleteAction = editMenu->addAction("Delete Selected Object");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(
        deleteAction,
        &QAction::triggered,
        this,
        &MainWindow::deleteSelectedObject
    );

    auto* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Fit All", m_view, &OccView::fitAll);

    auto* createMenu = menuBar()->addMenu("&Create");
    createMenu->addAction("Box", this, &MainWindow::addBox);

    auto* aiMenu = menuBar()->addMenu("&AI");
    aiMenu->addAction("Add Marker", this, &MainWindow::addAiMarker);
}

void MainWindow::createObjectPanel()
{
    auto* dock = new QDockWidget("Objects", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_objectList = new QListWidget(dock);
    dock->setWidget(m_objectList);

    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

void MainWindow::createPropertiesPanel()
{
    auto* dock = new QDockWidget("Properties", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    auto* panel = new QWidget(dock);
    auto* layout = new QFormLayout(panel);

    m_propertyNameValue = new QLabel(panel);
    m_propertyTypeValue = new QLabel(panel);

    m_propertyXEditor = new QDoubleSpinBox(panel);
    m_propertyYEditor = new QDoubleSpinBox(panel);
    m_propertyZEditor = new QDoubleSpinBox(panel);

    m_propertyLengthEditor = new QDoubleSpinBox(panel);
    m_propertyWidthEditor = new QDoubleSpinBox(panel);
    m_propertyHeightEditor = new QDoubleSpinBox(panel);

    for (QDoubleSpinBox* editor : {
             m_propertyXEditor,
             m_propertyYEditor,
             m_propertyZEditor,
             m_propertyLengthEditor,
             m_propertyWidthEditor,
             m_propertyHeightEditor
         }) {
        editor->setRange(-1000000.0, 1000000.0);
        editor->setDecimals(2);
        editor->setSingleStep(1.0);
        editor->setSuffix(" mm");
    }

    m_propertyLengthEditor->setMinimum(0.01);
    m_propertyWidthEditor->setMinimum(0.01);
    m_propertyHeightEditor->setMinimum(0.01);

    layout->addRow("Name:", m_propertyNameValue);
    layout->addRow("Type:", m_propertyTypeValue);
    layout->addRow("X:", m_propertyXEditor);
    layout->addRow("Y:", m_propertyYEditor);
    layout->addRow("Z:", m_propertyZEditor);
    layout->addRow("Length:", m_propertyLengthEditor);
    layout->addRow("Width:", m_propertyWidthEditor);
    layout->addRow("Height:", m_propertyHeightEditor);

    panel->setLayout(layout);
    dock->setWidget(panel);

    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(
        m_propertyXEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onPositionEditorChanged
    );

    connect(
        m_propertyYEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onPositionEditorChanged
    );

    connect(
        m_propertyZEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onPositionEditorChanged
    );

    connect(
        m_propertyLengthEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onBoxEditorChanged
    );

    connect(
        m_propertyWidthEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onBoxEditorChanged
    );

    connect(
        m_propertyHeightEditor,
        qOverload<double>(&QDoubleSpinBox::valueChanged),
        this,
        &MainWindow::onBoxEditorChanged
    );

    connect(
        &m_document,
        &CadDocument::boxRemoved,
        m_view,
        &OccView::removeBoxDisplay
    );

    connect(
        &m_document,
        &CadDocument::boxRemoved,
        this,
        &MainWindow::onBoxRemoved
    );
}

void MainWindow::newDocument()
{
    if (!maybeSaveBeforeDestructiveAction()) {
        return;
    }

    m_currentFilePath.clear();
    m_undoStack.clear();
    m_redoStack.clear();

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

    m_undoStack.clear();
    m_redoStack.clear();

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
    if (m_undoStack.isEmpty()) {
        statusBar()->showMessage("Nothing to undo");
        return;
    }

    m_redoStack.push_back(m_document.toJson());

    const QJsonObject snapshot = m_undoStack.takeLast();
    restoreDocumentSnapshot(snapshot);

    setDocumentModified(true);
    statusBar()->showMessage("Undo");
}

void MainWindow::redo()
{
    if (m_redoStack.isEmpty()) {
        statusBar()->showMessage("Nothing to redo");
        return;
    }

    m_undoStack.push_back(m_document.toJson());

    const QJsonObject snapshot = m_redoStack.takeLast();
    restoreDocumentSnapshot(snapshot);

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

    saveUndoSnapshot();


    const int existingMarkerCount =
        static_cast<int>(m_document.markers().size());

    const double offset = static_cast<double>(existingMarkerCount) * 20.0;

    std::shared_ptr<AiMarker> marker = m_document.addMarker(
        20.0 + offset,
        20.0 + offset,
        80.0
    );

    statusBar()->showMessage(
        QString("%1 added at X=%2 Y=%3 Z=%4")
            .arg(marker->name())
            .arg(marker->x())
            .arg(marker->y())
            .arg(marker->z())
    );
    setDocumentModified(true);
}

void MainWindow::deleteSelectedObject()
{
    if (m_selectedObjectKind == SelectedObjectKind::None
        || m_selectedObjectId <= 0) {
        statusBar()->showMessage("No object selected");
        return;
    }

    const int objectIdToDelete = m_selectedObjectId;

    saveUndoSnapshot();

    if (m_selectedObjectKind == SelectedObjectKind::Marker) {
        if (!m_document.removeMarkerById(objectIdToDelete)) {
            statusBar()->showMessage("Failed to delete selected marker");
            return;
        }

        statusBar()->showMessage(
            QString("Marker %1 deleted").arg(objectIdToDelete)
        );

        setDocumentModified(true);
        return;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box) {
        if (!m_document.removeBoxById(objectIdToDelete)) {
            statusBar()->showMessage("Failed to delete selected box");
            return;
        }

        statusBar()->showMessage(
            QString("Box %1 deleted").arg(objectIdToDelete)
        );

        setDocumentModified(true);
        return;
    }
}

void MainWindow::onMarkerAdded(std::shared_ptr<AiMarker> marker)
{
    if (marker == nullptr || m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* existingItem = m_objectList->item(index);

        if (existingItem != nullptr
            && existingItem->data(Qt::UserRole).toInt() == marker->id()) {
            existingItem->setText(marker->name());
            return;
        }
    }

    auto* item = new QListWidgetItem(marker->name());
    item->setData(Qt::UserRole, marker->id());

    m_objectList->addItem(item);
}

void MainWindow::onMarkerUpdated(std::shared_ptr<AiMarker> marker)
{
    if (marker == nullptr) {
        return;
    }

    updateObjectListItem(marker);

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
    removeObjectListItem(markerId);

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
    if (m_objectList != nullptr) {
        m_objectList->clear();
    }

    clearPropertiesPanel();

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
    }
}

void MainWindow::onObjectSelected(QListWidgetItem* item)
{
    if (item == nullptr) {
        clearPropertiesPanel();
        m_view->selectMarkerVisual(0);
        return;
    }

    const int selectedId = item->data(Qt::UserRole).toInt();

    std::shared_ptr<AiMarker> marker = m_document.findMarkerById(selectedId);

    if (marker != nullptr) {
        showMarkerProperties(marker);
        m_view->selectMarkerVisual(marker->id());
        return;
    }

    std::shared_ptr<CadBox> box = m_document.findBoxById(selectedId);

    if (box != nullptr) {
        showBoxProperties(box);
        m_view->selectMarkerVisual(0);
        return;
    }

    clearPropertiesPanel();
    m_view->selectMarkerVisual(0);
}

void MainWindow::onPositionEditorChanged()
{
    if (m_isUpdatingPropertiesUi
        || m_selectedObjectId <= 0
        || m_isRestoringSnapshot) {
        return;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Marker) {
        saveUndoSnapshot();

        if (m_document.updateMarkerPosition(
                m_selectedObjectId,
                m_propertyXEditor->value(),
                m_propertyYEditor->value(),
                m_propertyZEditor->value()
            )) {
            setDocumentModified(true);
        }

        return;
    }

    if (m_selectedObjectKind == SelectedObjectKind::Box) {
        onBoxEditorChanged();
    }
}

void MainWindow::onBoxEditorChanged()
{
    if (m_isUpdatingPropertiesUi
        || m_selectedObjectId <= 0
        || m_isRestoringSnapshot
        || m_selectedObjectKind != SelectedObjectKind::Box) {
        return;
    }

    saveUndoSnapshot();

    if (m_document.updateBox(
            m_selectedObjectId,
            m_propertyXEditor->value(),
            m_propertyYEditor->value(),
            m_propertyZEditor->value(),
            m_propertyLengthEditor->value(),
            m_propertyWidthEditor->value(),
            m_propertyHeightEditor->value()
        )) {
        setDocumentModified(true);
    }
}

void MainWindow::selectMarkerById(int markerId)
{
    std::shared_ptr<AiMarker> marker = m_document.findMarkerById(markerId);

    if (marker == nullptr) {
        clearPropertiesPanel();
        m_view->selectMarkerVisual(0);
        return;
    }

    showMarkerProperties(marker);
    m_view->selectMarkerVisual(markerId);

    if (m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* item = m_objectList->item(index);

        if (item != nullptr && item->data(Qt::UserRole).toInt() == markerId) {
            m_objectList->setCurrentItem(item);
            break;
        }
    }
}

void MainWindow::showMarkerProperties(const std::shared_ptr<AiMarker>& marker)
{
    if (marker == nullptr) {
        clearPropertiesPanel();
        return;
    }

    m_isUpdatingPropertiesUi = true;

    m_selectedObjectId = marker->id();
    m_selectedObjectKind = SelectedObjectKind::Marker;

    m_propertyNameValue->setText(marker->name());
    m_propertyTypeValue->setText("AI Marker");

    m_propertyXEditor->setEnabled(true);
    m_propertyYEditor->setEnabled(true);
    m_propertyZEditor->setEnabled(true);

    m_propertyLengthEditor->setEnabled(false);
    m_propertyWidthEditor->setEnabled(false);
    m_propertyHeightEditor->setEnabled(false);

    m_propertyXEditor->setValue(marker->x());
    m_propertyYEditor->setValue(marker->y());
    m_propertyZEditor->setValue(marker->z());

    m_propertyLengthEditor->setValue(0.01);
    m_propertyWidthEditor->setValue(0.01);
    m_propertyHeightEditor->setValue(0.01);

    m_isUpdatingPropertiesUi = false;

    statusBar()->showMessage(QString("%1 selected").arg(marker->name()));
}

void MainWindow::clearPropertiesPanel()
{
    m_isUpdatingPropertiesUi = true;

    m_selectedObjectId = 0;
    m_selectedObjectKind = SelectedObjectKind::None;

    m_propertyNameValue->setText("-");
    m_propertyTypeValue->setText("-");

    m_propertyXEditor->setValue(0.0);
    m_propertyYEditor->setValue(0.0);
    m_propertyZEditor->setValue(0.0);

    m_propertyLengthEditor->setValue(0.01);
    m_propertyWidthEditor->setValue(0.01);
    m_propertyHeightEditor->setValue(0.01);

    m_propertyXEditor->setEnabled(false);
    m_propertyYEditor->setEnabled(false);
    m_propertyZEditor->setEnabled(false);

    m_propertyLengthEditor->setEnabled(false);
    m_propertyWidthEditor->setEnabled(false);
    m_propertyHeightEditor->setEnabled(false);

    m_isUpdatingPropertiesUi = false;
}

void MainWindow::updateObjectListItem(const std::shared_ptr<AiMarker>& marker)
{
    if (marker == nullptr || m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* item = m_objectList->item(index);

        if (item != nullptr && item->data(Qt::UserRole).toInt() == marker->id()) {
            item->setText(marker->name());
            return;
        }
    }
}

void MainWindow::removeObjectListItem(int markerId)
{
    if (m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* item = m_objectList->item(index);

        if (item != nullptr && item->data(Qt::UserRole).toInt() == markerId) {
            delete m_objectList->takeItem(index);
            return;
        }
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

void MainWindow::saveUndoSnapshot()
{
    if (m_isRestoringSnapshot) {
        return;
    }

    m_undoStack.push_back(m_document.toJson());
    m_redoStack.clear();

    constexpr int maxUndoSteps = 100;

    while (m_undoStack.size() > maxUndoSteps) {
        m_undoStack.removeFirst();
    }
}

void MainWindow::restoreDocumentSnapshot(const QJsonObject& snapshot)
{
    m_isRestoringSnapshot = true;

    m_document.loadFromJson(snapshot);

    rebuildObjectPanelFromDocument();
    clearPropertiesPanel();

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
        m_view->fitAll();
    }

    m_isRestoringSnapshot = false;
}

void MainWindow::rebuildObjectPanelFromDocument()
{
    if (m_objectList == nullptr) {
        return;
    }

    m_objectList->clear();

    for (const std::shared_ptr<AiMarker>& marker : m_document.markers()) {
        if (marker == nullptr) {
            continue;
        }

        auto* item = new QListWidgetItem(marker->name());
        item->setData(Qt::UserRole, marker->id());
        m_objectList->addItem(item);
    }

    for (const std::shared_ptr<CadBox>& box : m_document.boxes()) {
        if (box == nullptr) {
            continue;
        }

        auto* item = new QListWidgetItem(box->name());
        item->setData(Qt::UserRole, box->id());
        m_objectList->addItem(item);
    }
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
    saveUndoSnapshot();

    std::shared_ptr<CadBox> box = m_document.addBox(
        0.0,
        0.0,
        0.0,
        100.0,
        80.0,
        60.0
    );

    statusBar()->showMessage(
        QString("%1 created").arg(box->name())
    );

    setDocumentModified(true);
}


void MainWindow::onBoxAdded(std::shared_ptr<CadBox> box)
{
    if (box == nullptr || m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* existingItem = m_objectList->item(index);

        if (existingItem != nullptr
            && existingItem->data(Qt::UserRole).toInt() == box->id()) {
            existingItem->setText(box->name());
            return;
        }
    }

    auto* item = new QListWidgetItem(box->name());
    item->setData(Qt::UserRole, box->id());

    m_objectList->addItem(item);
}

void MainWindow::showBoxProperties(const std::shared_ptr<CadBox>& box)
{
    if (box == nullptr) {
        clearPropertiesPanel();
        return;
    }

    m_isUpdatingPropertiesUi = true;

    m_selectedObjectId = box->id();
    m_selectedObjectKind = SelectedObjectKind::Box;

    m_propertyNameValue->setText(box->name());
    m_propertyTypeValue->setText("Box");

    m_propertyXEditor->setEnabled(true);
    m_propertyYEditor->setEnabled(true);
    m_propertyZEditor->setEnabled(true);

    m_propertyLengthEditor->setEnabled(true);
    m_propertyWidthEditor->setEnabled(true);
    m_propertyHeightEditor->setEnabled(true);

    m_propertyXEditor->setValue(box->x());
    m_propertyYEditor->setValue(box->y());
    m_propertyZEditor->setValue(box->z());

    m_propertyLengthEditor->setValue(box->length());
    m_propertyWidthEditor->setValue(box->width());
    m_propertyHeightEditor->setValue(box->height());

    m_isUpdatingPropertiesUi = false;

    statusBar()->showMessage(QString("%1 selected").arg(box->name()));
}


void MainWindow::onBoxUpdated(std::shared_ptr<CadBox> box)
{
    if (box == nullptr) {
        return;
    }

    updateObjectListItem(box);

    if (m_selectedObjectKind == SelectedObjectKind::Box
        && box->id() == m_selectedObjectId) {
        showBoxProperties(box);
    }

    statusBar()->showMessage(
        QString("%1 updated").arg(box->name())
    );
}

void MainWindow::updateObjectListItem(const std::shared_ptr<CadBox>& box)
{
    if (box == nullptr || m_objectList == nullptr) {
        return;
    }

    for (int index = 0; index < m_objectList->count(); ++index) {
        QListWidgetItem* item = m_objectList->item(index);

        if (item != nullptr && item->data(Qt::UserRole).toInt() == box->id()) {
            item->setText(box->name());
            return;
        }
    }
}

void MainWindow::onBoxRemoved(int boxId)
{
    removeObjectListItem(boxId);

    if (m_selectedObjectKind == SelectedObjectKind::Box
        && m_selectedObjectId == boxId) {
        clearPropertiesPanel();
    }

    if (m_view != nullptr) {
        m_view->selectMarkerVisual(0);
    }
}