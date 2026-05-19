#include "io/DocumentIO.hpp"

#include "cad/CadDocument.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

bool DocumentIO::saveToFile(
    const CadDocument& document,
    const QString& filePath
)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    const QJsonDocument jsonDocument(document.toJson());

    const qint64 bytesWritten =
        file.write(jsonDocument.toJson(QJsonDocument::Indented));

    file.close();

    return bytesWritten >= 0;
}

bool DocumentIO::loadFromFile(
    CadDocument& document,
    const QString& filePath
)
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

    return document.loadFromJson(jsonDocument.object());
}