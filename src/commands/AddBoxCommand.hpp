#pragma once

#include "commands/CadCommand.hpp"
#include "cad/CadBox.hpp"
#include "cad/CadDocument.hpp"

#include <memory>

class AddBoxCommand final : public CadCommand
{
public:
    AddBoxCommand(
        CadDocument& document,
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

    QString name() const override;

    bool execute() override;
    bool undo() override;

private:
    CadDocument& m_document;

    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

    double m_length = 100.0;
    double m_width = 80.0;
    double m_height = 60.0;

    int m_createdBoxId = 0;
    QString m_createdBoxName;
};