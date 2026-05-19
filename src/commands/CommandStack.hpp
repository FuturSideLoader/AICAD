#pragma once

#include "commands/CadCommand.hpp"

#include <QObject>

#include <memory>
#include <vector>

class CommandStack final : public QObject
{
    Q_OBJECT

public:
    explicit CommandStack(QObject* parent = nullptr);

    bool executeCommand(std::unique_ptr<CadCommand> command);

    bool undo();
    bool redo();

    bool canUndo() const;
    bool canRedo() const;

    QString undoText() const;
    QString redoText() const;

    void clear();

signals:
    void changed();

private:
    std::vector<std::unique_ptr<CadCommand>> m_undoStack;
    std::vector<std::unique_ptr<CadCommand>> m_redoStack;
};