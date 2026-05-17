#pragma once

#include "cad/AiMarker.hpp"

#include <QObject>
#include <memory>
#include <vector>

class CadDocument final : public QObject
{
    Q_OBJECT

public:
    explicit CadDocument(QObject* parent = nullptr);

    const std::vector<std::shared_ptr<AiMarker>>& markers() const;

    std::shared_ptr<AiMarker> addMarker(double x, double y, double z);
    std::shared_ptr<AiMarker> findMarkerById(int id) const;

    bool updateMarkerPosition(int id, double x, double y, double z);
    bool removeMarkerById(int id);

signals:
    void markerAdded(std::shared_ptr<AiMarker> marker);
    void markerUpdated(std::shared_ptr<AiMarker> marker);
    void markerRemoved(int markerId);

private:
    std::vector<std::shared_ptr<AiMarker>> m_markers;
    int m_nextObjectId = 1;
};