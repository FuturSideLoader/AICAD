#pragma once

#include "cad/AiMarker.hpp"
#include "cad/CadBox.hpp"

#include <QJsonObject>
#include <QObject>

#include <memory>
#include <vector>

class CadDocument final : public QObject
{
    Q_OBJECT

public:
    explicit CadDocument(QObject* parent = nullptr);

    const std::vector<std::shared_ptr<AiMarker>>& markers() const;
    const std::vector<std::shared_ptr<CadBox>>& boxes() const;

    std::shared_ptr<AiMarker> addMarker(double x, double y, double z);
    std::shared_ptr<AiMarker> addMarkerWithId(
        int id,
        const QString& name,
        double x,
        double y,
        double z
    );

    std::shared_ptr<CadBox> addBox(
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

    std::shared_ptr<CadBox> addBoxWithId(
        int id,
        const QString& name,
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

    std::shared_ptr<AiMarker> findMarkerById(int id) const;
    std::shared_ptr<CadBox> findBoxById(int id) const;

    bool updateMarkerPosition(int id, double x, double y, double z);
    bool removeMarkerById(int id);

    void clear();

    QJsonObject toJson() const;
    bool loadFromJson(const QJsonObject& rootObject);

signals:
    void markerAdded(std::shared_ptr<AiMarker> marker);
    void markerUpdated(std::shared_ptr<AiMarker> marker);
    void markerRemoved(int markerId);

    void boxAdded(std::shared_ptr<CadBox> box);

    void documentCleared();

private:
    std::vector<std::shared_ptr<AiMarker>> m_markers;
    std::vector<std::shared_ptr<CadBox>> m_boxes;

    int m_nextObjectId = 1;
};