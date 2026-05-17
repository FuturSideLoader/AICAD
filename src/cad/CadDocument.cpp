#include "cad/CadDocument.hpp"

#include <QJsonArray>
#include <QJsonObject>

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

std::shared_ptr<AiMarker> CadDocument::addMarkerWithId(
    int id,
    const QString& name,
    double x,
    double y,
    double z
)
{
    auto marker = std::make_shared<AiMarker>(
        id,
        x,
        y,
        z
    );

    marker->setName(name);

    m_markers.push_back(marker);

    if (id >= m_nextObjectId) {
        m_nextObjectId = id + 1;
    }

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

void CadDocument::clear()
{
    m_markers.clear();
    m_nextObjectId = 1;

    emit documentCleared();
}

QJsonObject CadDocument::toJson() const
{
    QJsonObject rootObject;

    rootObject["format"] = "AICAD";
    rootObject["version"] = 1;
    rootObject["nextObjectId"] = m_nextObjectId;

    QJsonArray markersArray;

    for (const auto& marker : m_markers) {
        if (marker == nullptr) {
            continue;
        }

        QJsonObject markerObject;
        markerObject["id"] = marker->id();
        markerObject["name"] = marker->name();
        markerObject["x"] = marker->x();
        markerObject["y"] = marker->y();
        markerObject["z"] = marker->z();

        markersArray.append(markerObject);
    }

    rootObject["markers"] = markersArray;

    return rootObject;
}

bool CadDocument::loadFromJson(const QJsonObject& rootObject)
{
    if (rootObject.value("format").toString() != "AICAD") {
        return false;
    }

    const int version = rootObject.value("version").toInt();

    if (version != 1) {
        return false;
    }

    clear();

    m_nextObjectId = rootObject.value("nextObjectId").toInt(1);

    const QJsonArray markersArray =
        rootObject.value("markers").toArray();

    for (const QJsonValue& markerValue : markersArray) {
        const QJsonObject markerObject = markerValue.toObject();

        const int id = markerObject.value("id").toInt();
        const QString name = markerObject.value("name").toString(
            QString("Marker %1").arg(id)
        );

        const double x = markerObject.value("x").toDouble();
        const double y = markerObject.value("y").toDouble();
        const double z = markerObject.value("z").toDouble();

        if (id <= 0) {
            continue;
        }

        addMarkerWithId(id, name, x, y, z);
    }

    return true;
}