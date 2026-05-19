#pragma once

#include <QObject>
#include <QString>

class AppController final : public QObject
{
    Q_OBJECT

public:
    explicit AppController(QObject* parent = nullptr);

    QString currentFilePath() const;
    bool isDocumentModified() const;

    void setCurrentFilePath(const QString& filePath);
    void clearCurrentFilePath();

    void setDocumentModified(bool modified);

    QString windowTitle() const;

signals:
    void stateChanged();

private:
    QString m_currentFilePath;
    bool m_documentModified = false;
};