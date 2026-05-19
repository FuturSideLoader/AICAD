#pragma once

#include "commands/CadCommand.hpp"
#include "cad/CadDocument.hpp"

#include <QString>

enum class DeleteObjectKind
{
    None,
    Marker,
    Box
};

class DeleteObjectCommand final : public CadCommand
{
public:
    DeleteObjectCommand(
        CadDocument& document,
        int objectId,
        DeleteObjectKind kind
    );

    QString name() const override;

    bool execute() override;
    bool undo() override;

private:
    bool captureObjectBeforeDelete();

private:
    CadDocument& m_document;

    int m_objectId = 0;
    DeleteObjectKind m_kind = DeleteObjectKind::None;

    bool m_captured = false;

    QString m_name;

    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

    double m_length = 0.0;
    double m_width = 0.0;
    double m_height = 0.0;
};