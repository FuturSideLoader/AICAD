#include "commands/UpdateMarkerCommand.hpp"

UpdateMarkerCommand::UpdateMarkerCommand(
    CadDocument& document,
    int markerId,
    double oldX,
    double oldY,
    double oldZ,
    double newX,
    double newY,
    double newZ
)
    : m_document(document),
      m_markerId(markerId),
      m_oldX(oldX),
      m_oldY(oldY),
      m_oldZ(oldZ),
      m_newX(newX),
      m_newY(newY),
      m_newZ(newZ)
{
}

QString UpdateMarkerCommand::name() const
{
    return "Update Marker";
}

bool UpdateMarkerCommand::execute()
{
    if (m_markerId <= 0) {
        return false;
    }

    return m_document.updateMarkerPosition(
        m_markerId,
        m_newX,
        m_newY,
        m_newZ
    );
}

bool UpdateMarkerCommand::undo()
{
    if (m_markerId <= 0) {
        return false;
    }

    return m_document.updateMarkerPosition(
        m_markerId,
        m_oldX,
        m_oldY,
        m_oldZ
    );
}