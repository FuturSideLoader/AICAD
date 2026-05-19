#include "app/AppController.hpp"

#include <QFileInfo>

AppController::AppController(QObject* parent)
    : QObject(parent)
{
}

QString AppController::currentFilePath() const
{
    return m_currentFilePath;
}

bool AppController::isDocumentModified() const
{
    return m_documentModified;
}

void AppController::setCurrentFilePath(const QString& filePath)
{
    if (m_currentFilePath == filePath) {
        return;
    }

    m_currentFilePath = filePath;
    emit stateChanged();
}

void AppController::clearCurrentFilePath()
{
    if (m_currentFilePath.isEmpty()) {
        return;
    }

    m_currentFilePath.clear();
    emit stateChanged();
}

void AppController::setDocumentModified(bool modified)
{
    if (m_documentModified == modified) {
        return;
    }

    m_documentModified = modified;
    emit stateChanged();
}

QString AppController::windowTitle() const
{
    QString documentName = "Untitled";

    if (!m_currentFilePath.isEmpty()) {
        documentName = QFileInfo(m_currentFilePath).fileName();
    }

    if (m_documentModified) {
        documentName += "*";
    }

    return QString("AICAD - %1").arg(documentName);
}
