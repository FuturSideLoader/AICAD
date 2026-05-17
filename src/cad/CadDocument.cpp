#include "cad/CadDocument.hpp"

#include <algorithm>

CadDocument::CadDocument(QObject* parent)
    : QObject(parent)
{
}

const std::vector<std::shared_ptr<AiMarker>>& CadDocument::markers() const
{
    return m_markers;
}

std::shared_ptr<AiMarker> CadDocument::addMarker(
    double x,
    double y,
    double z
)
{
    auto marker = std::make_shared<AiMarker>(
        m_nextObjectId++,
        x,
        y,
        z
    );

    m_markers.push_back(marker);

    emit markerAdded(marker);

    return marker;
}

std::shared_ptr<AiMarker> CadDocument::findMarkerById(int id) const
{
    for (const auto& marker : m_markers) {
        if (marker != nullptr && marker->id() == id) {
            return marker;
        }
    }

    return nullptr;
}

bool CadDocument::updateMarkerPosition(
    int id,
    double x,
    double y,
    double z
)
{
    std::shared_ptr<AiMarker> marker = findMarkerById(id);

    if (marker == nullptr) {
        return false;
    }

    marker->setPosition(x, y, z);
    emit markerUpdated(marker);

    return true;
}

bool CadDocument::removeMarkerById(int id)
{
    const auto it = std::remove_if(
        m_markers.begin(),
        m_markers.end(),
        [id](const std::shared_ptr<AiMarker>& marker) {
            return marker != nullptr && marker->id() == id;
        }
    );

    if (it == m_markers.end()) {
        return false;
    }

    m_markers.erase(it, m_markers.end());

    emit markerRemoved(id);

    return true;
}