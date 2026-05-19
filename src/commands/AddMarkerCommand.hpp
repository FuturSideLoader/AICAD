#pragma once

#include "commands/CadCommand.hpp"
#include "cad/AiMarker.hpp"
#include "cad/CadDocument.hpp"

#include <memory>

class AddMarkerCommand final : public CadCommand
{
public:
    AddMarkerCommand(
        CadDocument& document,
        double x,
        double y,
        double z
    );

    QString name() const override;

    bool execute() override;
    bool undo() override;

private:
    CadDocument& m_document;

    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

    int m_createdMarkerId = 0;
    QString m_createdMarkerName;
};