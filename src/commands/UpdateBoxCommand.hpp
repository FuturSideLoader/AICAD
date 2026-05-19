#pragma once

#include "commands/CadCommand.hpp"
#include "cad/CadDocument.hpp"

class UpdateBoxCommand final : public CadCommand
{
public:
    UpdateBoxCommand(
        CadDocument& document,
        int boxId,

        double oldX,
        double oldY,
        double oldZ,
        double oldLength,
        double oldWidth,
        double oldHeight,

        double newX,
        double newY,
        double newZ,
        double newLength,
        double newWidth,
        double newHeight
    );

    QString name() const override;

    bool execute() override;
    bool undo() override;

private:
    CadDocument& m_document;

    int m_boxId = 0;

    double m_oldX = 0.0;
    double m_oldY = 0.0;
    double m_oldZ = 0.0;
    double m_oldLength = 0.0;
    double m_oldWidth = 0.0;
    double m_oldHeight = 0.0;

    double m_newX = 0.0;
    double m_newY = 0.0;
    double m_newZ = 0.0;
    double m_newLength = 0.0;
    double m_newWidth = 0.0;
    double m_newHeight = 0.0;
};