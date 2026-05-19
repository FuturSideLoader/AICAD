#pragma once

#include <QString>

class CadDocument;

class DocumentIO final
{
public:
    DocumentIO() = delete;

    static bool saveToFile(
        const CadDocument& document,
        const QString& filePath
    );

    static bool loadFromFile(
        CadDocument& document,
        const QString& filePath
    );
};