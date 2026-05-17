#pragma once

#include <QString>

enum class CadObjectType
{
    Unknown,
    AiMarker,
    Shape,
    Sketch,
    Assembly,
    ImportedComponent,
    Box,
};

class CadObject
{
public:
    CadObject(
        int id,
        QString name,
        CadObjectType type
    );

    virtual ~CadObject() = default;

    int id() const;
    QString name() const;
    CadObjectType type() const;

    void setName(const QString& name);

private:
    int m_id = 0;
    QString m_name;
    CadObjectType m_type = CadObjectType::Unknown;
};
