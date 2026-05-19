#include "commands/UpdateBoxCommand.hpp"

UpdateBoxCommand::UpdateBoxCommand(
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
)
    : m_document(document),
      m_boxId(boxId),
      m_oldX(oldX),
      m_oldY(oldY),
      m_oldZ(oldZ),
      m_oldLength(oldLength),
      m_oldWidth(oldWidth),
      m_oldHeight(oldHeight),
      m_newX(newX),
      m_newY(newY),
      m_newZ(newZ),
      m_newLength(newLength),
      m_newWidth(newWidth),
      m_newHeight(newHeight)
{
}

QString UpdateBoxCommand::name() const
{
    return "Update Box";
}

bool UpdateBoxCommand::execute()
{
    if (m_boxId <= 0) {
        return false;
    }

    return m_document.updateBox(
        m_boxId,
        m_newX,
        m_newY,
        m_newZ,
        m_newLength,
        m_newWidth,
        m_newHeight
    );
}

bool UpdateBoxCommand::undo()
{
    if (m_boxId <= 0) {
        return false;
    }

    return m_document.updateBox(
        m_boxId,
        m_oldX,
        m_oldY,
        m_oldZ,
        m_oldLength,
        m_oldWidth,
        m_oldHeight
    );
}