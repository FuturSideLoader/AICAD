#pragma once

#include <QPoint>
#include <QWidget>

#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveObject.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

#include "cad/AiMarker.hpp"

#include <memory>
#include <unordered_map>
#include "cad/CadBox.hpp"

class OccView final : public QWidget
{
    Q_OBJECT

public:
    explicit OccView(QWidget* parent = nullptr);

public slots:
    void fitAll();
    void displayMarker(std::shared_ptr<AiMarker> marker);
    void updateMarkerDisplay(std::shared_ptr<AiMarker> marker);
    void selectMarkerVisual(int markerId);
    void removeMarkerDisplay(int markerId);
    void clearSceneMarkers();
    void displayBox(std::shared_ptr<CadBox> box);
    void clearSceneObjects();
    void updateBoxDisplay(std::shared_ptr<CadBox> box);
    void removeBoxDisplay(int boxId);

signals:
    void markerPicked(int markerId);

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    struct MarkerVisual
    {
        Handle(AIS_InteractiveObject) sphere;
        Handle(AIS_InteractiveObject) point;
    };

    struct BoxVisual
    {
        Handle(AIS_InteractiveObject) shape;
    };

    std::unordered_map<int, BoxVisual> m_boxVisuals;

    void removeBoxVisual(int boxId);
    BoxVisual createBoxVisual(const std::shared_ptr<CadBox>& box);

private:
    void initializeOpenCascade();
    void createDemoShape();

    void removeMarkerVisual(int markerId);
    MarkerVisual createMarkerVisual(const std::shared_ptr<AiMarker>& marker);

    void tryPickMarkerAt(const QPoint& position);
    int findMarkerIdFromObject(const Handle(AIS_InteractiveObject)& object) const;

    void setMarkerColor(int markerId, const Quantity_Color& color);

private:
    bool m_initialized = false;

    int m_selectedMarkerId = 0;

    Handle(OpenGl_GraphicDriver) m_graphicDriver;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_context;

    QPoint m_lastMousePosition;
    QPoint m_mousePressPosition;

    std::unordered_map<int, MarkerVisual> m_markerVisuals;
};