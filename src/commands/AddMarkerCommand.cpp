#include "commands/AddMarkerCommand.hpp"

AddMarkerCommand::AddMarkerCommand(
    CadDocument& document,
    double x,
    double y,
    double z
)
    : m_document(document),
      m_x(x),
      m_y(y),
      m_z(z)
{
}

QString AddMarkerCommand::name() const
{
    return "Add Marker";
}

bool AddMarkerCommand::execute()
{
    if (m_createdMarkerId <= 0) {
        std::shared_ptr<AiMarker> marker = m_document.addMarker(
            m_x,
            m_y,
            m_z
        );

        if (marker == nullptr) {
            return false;
        }

        m_createdMarkerId = marker->id();
        m_createdMarkerName = marker->name();

        return true;
    }

    std::shared_ptr<AiMarker> marker = m_document.addMarkerWithId(
        m_createdMarkerId,
        m_createdMarkerName,
        m_x,
        m_y,
        m_z
    );

    return marker != nullptr;
}

bool AddMarkerCommand::undo()
{
    if (m_createdMarkerId <= 0) {
        return false;
    }

    return m_document.removeMarkerById(m_createdMarkerId);
}