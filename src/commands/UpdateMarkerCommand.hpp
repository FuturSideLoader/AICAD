#pragma once

#include "commands/CadCommand.hpp"
#include "cad/CadDocument.hpp"

class UpdateMarkerCommand final : public CadCommand
{
public:
    UpdateMarkerCommand(
        CadDocument& document,
        int markerId,
        double oldX,
        double oldY,
        double oldZ,
        double newX,
        double newY,
        double newZ
    );

    QString name() const override;

    bool execute() override;
    bool undo() override;

private:
    CadDocument& m_document;

    int m_markerId = 0;

    double m_oldX = 0.0;
    double m_oldY = 0.0;
    double m_oldZ = 0.0;

    double m_newX = 0.0;
    double m_newY = 0.0;
    double m_newZ = 0.0;
};