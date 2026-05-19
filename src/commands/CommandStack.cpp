#include "commands/CommandStack.hpp"

CommandStack::CommandStack(QObject* parent)
    : QObject(parent)
{
}

bool CommandStack::executeCommand(std::unique_ptr<CadCommand> command)
{
    if (!command) {
        return false;
    }

    if (!command->execute()) {
        return false;
    }

    m_undoStack.push_back(std::move(command));
    m_redoStack.clear();

    emit changed();

    return true;
}

bool CommandStack::undo()
{
    if (m_undoStack.empty()) {
        return false;
    }

    std::unique_ptr<CadCommand> command = std::move(m_undoStack.back());
    m_undoStack.pop_back();

    if (!command->undo()) {
        return false;
    }

    m_redoStack.push_back(std::move(command));

    emit changed();

    return true;
}

bool CommandStack::redo()
{
    if (m_redoStack.empty()) {
        return false;
    }

    std::unique_ptr<CadCommand> command = std::move(m_redoStack.back());
    m_redoStack.pop_back();

    if (!command->execute()) {
        return false;
    }

    m_undoStack.push_back(std::move(command));

    emit changed();

    return true;
}

bool CommandStack::canUndo() const
{
    return !m_undoStack.empty();
}

bool CommandStack::canRedo() const
{
    return !m_redoStack.empty();
}

QString CommandStack::undoText() const
{
    if (!canUndo()) {
        return "Undo";
    }

    return QString("Undo %1").arg(m_undoStack.back()->name());
}

QString CommandStack::redoText() const
{
    if (!canRedo()) {
        return "Redo";
    }

    return QString("Redo %1").arg(m_redoStack.back()->name());
}

void CommandStack::clear()
{
    m_undoStack.clear();
    m_redoStack.clear();

    emit changed();
}