#pragma once

#include <QString>

class CadCommand
{
public:
    virtual ~CadCommand() = default;

    virtual QString name() const = 0;

    virtual bool execute() = 0;
    virtual bool undo() = 0;
};