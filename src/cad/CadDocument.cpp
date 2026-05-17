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

const std::vector<std::shared_ptr<CadBox>>& CadDocument::boxes() const
{
    return m_boxes;
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

std::shared_ptr<CadBox> CadDocument::addBox(
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
{
    auto box = std::make_shared<CadBox>(
        m_nextObjectId++,
        x,
        y,
        z,
        length,
        width,
        height
    );

    m_boxes.push_back(box);

    emit boxAdded(box);

    return box;
}

std::shared_ptr<CadBox> CadDocument::addBoxWithId(
    int id,
    const QString& name,
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
{
    auto box = std::make_shared<CadBox>(
        id,
        x,
        y,
        z,
        length,
        width,
        height
    );

    box->setName(name);

    m_boxes.push_back(box);

    if (id >= m_nextObjectId) {
        m_nextObjectId = id + 1;
    }

    emit boxAdded(box);

    return box;
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

std::shared_ptr<CadBox> CadDocument::findBoxById(int id) const
{
    for (const auto& box : m_boxes) {
        if (box != nullptr && box->id() == id) {
            return box;
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

bool CadDocument::updateBox(
    int id,
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
{
    std::shared_ptr<CadBox> box = findBoxById(id);

    if (box == nullptr) {
        return false;
    }

    if (length <= 0.0 || width <= 0.0 || height <= 0.0) {
        return false;
    }

    box->setPosition(x, y, z);
    box->setDimensions(length, width, height);

    emit boxUpdated(box);

    return true;
}

void CadDocument::clear()
{
    m_markers.clear();
    m_boxes.clear();
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

    QJsonArray boxesArray;

    for (const auto& box : m_boxes) {
        if (box == nullptr) {
            continue;
        }

        QJsonObject boxObject;
        boxObject["id"] = box->id();
        boxObject["name"] = box->name();
        boxObject["x"] = box->x();
        boxObject["y"] = box->y();
        boxObject["z"] = box->z();
        boxObject["length"] = box->length();
        boxObject["width"] = box->width();
        boxObject["height"] = box->height();

        boxesArray.append(boxObject);
    }

    rootObject["markers"] = markersArray;
    rootObject["boxes"] = boxesArray;

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

    const QJsonArray boxesArray =
        rootObject.value("boxes").toArray();

    for (const QJsonValue& boxValue : boxesArray) {
        const QJsonObject boxObject = boxValue.toObject();

        const int id = boxObject.value("id").toInt();
        const QString name = boxObject.value("name").toString(
            QString("Box %1").arg(id)
        );

        const double x = boxObject.value("x").toDouble();
        const double y = boxObject.value("y").toDouble();
        const double z = boxObject.value("z").toDouble();
        const double length = boxObject.value("length").toDouble(100.0);
        const double width = boxObject.value("width").toDouble(80.0);
        const double height = boxObject.value("height").toDouble(60.0);

        if (id <= 0) {
            continue;
        }

        addBoxWithId(id, name, x, y, z, length, width, height);
    }

    return true;
}