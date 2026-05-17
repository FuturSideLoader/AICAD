#include "OccView.hpp"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QShowEvent>

#include <AIS_Point.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <Geom_CartesianPoint.hxx>
#include <Prs3d_PointAspect.hxx>
#include <Quantity_Color.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include <vector>
#include <cmath>
#include <gp_Trsf.hxx>
#include <TopLoc_Location.hxx>

OccView::OccView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    setMouseTracking(true);
}

void OccView::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!m_initialized) {
        initializeOpenCascade();
        createDemoShape();
        fitAll();
        m_initialized = true;
    }
}

void OccView::initializeOpenCascade()
{
    Handle(Aspect_DisplayConnection) displayConnection = new Aspect_DisplayConnection();

    m_graphicDriver = new OpenGl_GraphicDriver(displayConnection);

    m_viewer = new V3d_Viewer(m_graphicDriver);
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_context = new AIS_InteractiveContext(m_viewer);

    m_view = m_viewer->CreateView();

    Handle(Aspect_NeutralWindow) window = new Aspect_NeutralWindow();
    window->SetNativeHandle(static_cast<Aspect_Drawable>(winId()));
    window->SetSize(width(), height());

    m_view->SetWindow(window);

    if (!window->IsMapped()) {
        window->Map();
    }

    m_view->SetBackgroundColor(Quantity_NOC_GRAY20);
    m_view->TriedronDisplay(
        Aspect_TOTP_LEFT_LOWER,
        Quantity_NOC_WHITE,
        0.08,
        V3d_ZBUFFER
    );

    m_view->MustBeResized();
}

void OccView::createDemoShape()
{
    TopoDS_Shape box = BRepPrimAPI_MakeBox(100.0, 80.0, 60.0).Shape();

    Handle(AIS_Shape) aisBox = new AIS_Shape(box);
    aisBox->SetColor(Quantity_NOC_STEELBLUE);
    aisBox->SetDisplayMode(AIS_Shaded);

    m_context->Display(aisBox, Standard_True);
}

void OccView::displayMarker(std::shared_ptr<AiMarker> marker)
{
    if (m_context.IsNull() || marker == nullptr) {
        return;
    }

    removeMarkerVisual(marker->id());

    MarkerVisual visual = createMarkerVisual(marker);
    m_markerVisuals[marker->id()] = visual;

    if (marker->id() == m_selectedMarkerId) {
        setMarkerColor(marker->id(), Quantity_NOC_LIMEGREEN);
    }

    m_view->Redraw();
}

void OccView::updateMarkerDisplay(std::shared_ptr<AiMarker> marker)
{
    if (m_context.IsNull() || marker == nullptr) {
        return;
    }

    removeMarkerVisual(marker->id());

    MarkerVisual visual = createMarkerVisual(marker);
    m_markerVisuals[marker->id()] = visual;

    if (marker->id() == m_selectedMarkerId) {
        setMarkerColor(marker->id(), Quantity_NOC_LIMEGREEN);
    }

    m_view->Redraw();
}

void OccView::selectMarkerVisual(int markerId)
{
    if (m_context.IsNull()) {
        return;
    } 

    if (m_selectedMarkerId > 0 && m_selectedMarkerId != markerId) {
        setMarkerColor(m_selectedMarkerId, Quantity_NOC_YELLOW);
    }

    m_selectedMarkerId = markerId;

    if (m_selectedMarkerId > 0) {
        setMarkerColor(m_selectedMarkerId, Quantity_NOC_LIMEGREEN);
    }

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::removeMarkerDisplay(int markerId)
{
    if (markerId <= 0) {
        return;
    }

    removeMarkerVisual(markerId);

    if (m_selectedMarkerId == markerId) {
        m_selectedMarkerId = 0;
    }

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::removeMarkerVisual(int markerId)
{
    if (m_context.IsNull()) {
        return;
    }

    auto it = m_markerVisuals.find(markerId);

    if (it == m_markerVisuals.end()) {
        return;
    }

    if (!it->second.sphere.IsNull()) {
        m_context->Remove(it->second.sphere, Standard_False);
    }

    if (!it->second.point.IsNull()) {
        m_context->Remove(it->second.point, Standard_False);
    }

    m_markerVisuals.erase(it);
}

OccView::MarkerVisual OccView::createMarkerVisual(
    const std::shared_ptr<AiMarker>& marker
)
{
    MarkerVisual visual;

    TopoDS_Shape sphere = BRepPrimAPI_MakeSphere(
        gp_Pnt(marker->x(), marker->y(), marker->z()),
        5.0
    ).Shape();

    Handle(AIS_Shape) markerShape = new AIS_Shape(sphere);
    markerShape->SetColor(Quantity_NOC_YELLOW);
    markerShape->SetDisplayMode(AIS_Shaded);

    m_context->Display(markerShape, Standard_False);

    Handle(Geom_CartesianPoint) point =
        new Geom_CartesianPoint(
            marker->x(),
            marker->y(),
            marker->z()
        );

    Handle(AIS_Point) markerPoint = new AIS_Point(point);
    markerPoint->SetColor(Quantity_NOC_RED);

    Handle(Prs3d_PointAspect) pointAspect =
        new Prs3d_PointAspect(
            Aspect_TOM_PLUS,
            Quantity_NOC_RED,
            6.0
        );

    markerPoint->Attributes()->SetPointAspect(pointAspect);

    m_context->Display(markerPoint, Standard_False);

    visual.sphere = markerShape;
    visual.point = markerPoint;

    return visual;
}

void OccView::fitAll()
{
    if (!m_view.IsNull()) {
        m_view->FitAll();
        m_view->Redraw();
    }
}

void OccView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (!m_view.IsNull()) {
        m_view->MustBeResized();
        m_view->Redraw();
    }
}

void OccView::paintEvent(QPaintEvent*)
{
    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePosition = event->pos();
    m_mousePressPosition = event->pos();

    if (!m_view.IsNull() && event->button() == Qt::LeftButton) {
        m_view->StartRotation(event->pos().x(), event->pos().y());
    }
}

void OccView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    const QPoint delta = event->pos() - m_mousePressPosition;
    const int distanceSquared = delta.x() * delta.x() + delta.y() * delta.y();

    if (distanceSquared <= 16) {
        tryPickMarkerAt(event->pos());
    }
}

void OccView::mouseMoveEvent(QMouseEvent* event)
{
    if (m_view.IsNull()) {
        return;
    }

    const QPoint currentPosition = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        m_view->Rotation(
            currentPosition.x(),
            currentPosition.y()
        );
    }

    if (event->buttons() & Qt::MiddleButton) {
        const QPoint delta = currentPosition - m_lastMousePosition;
        m_view->Pan(delta.x(), -delta.y());
    }

    m_lastMousePosition = currentPosition;
}

void OccView::tryPickMarkerAt(const QPoint& position)
{
    if (m_context.IsNull() || m_view.IsNull()) {
        return;
    }

    m_context->MoveTo(position.x(), position.y(), m_view, Standard_False);

    if (!m_context->HasDetected()) {
        return;
    }

    Handle(AIS_InteractiveObject) detectedObject =
        m_context->DetectedInteractive();

    const int markerId = findMarkerIdFromObject(detectedObject);

    if (markerId > 0) {
        emit markerPicked(markerId);
    }
}

int OccView::findMarkerIdFromObject(
    const Handle(AIS_InteractiveObject)& object
) const
{
    if (object.IsNull()) {
        return 0;
    }

    for (const auto& [markerId, visual] : m_markerVisuals) {
        if (!visual.sphere.IsNull() && visual.sphere == object) {
            return markerId;
        }

        if (!visual.point.IsNull() && visual.point == object) {
            return markerId;
        }
    }

    return 0;
}

void OccView::setMarkerColor(int markerId, const Quantity_Color& color)
{
    auto it = m_markerVisuals.find(markerId);

    if (it == m_markerVisuals.end()) {
        return;
    }

    if (!it->second.sphere.IsNull()) {
        Handle(AIS_Shape) sphereShape =
            Handle(AIS_Shape)::DownCast(it->second.sphere);

        if (!sphereShape.IsNull()) {
            sphereShape->SetColor(color);
            m_context->Redisplay(sphereShape, Standard_False);
        }
    }
}

void OccView::clearSceneMarkers()
{
    if (m_context.IsNull()) {
        return;
    }

    std::vector<int> idsToRemove;

    for (const auto& [markerId, visual] : m_markerVisuals) {
        Q_UNUSED(visual);
        idsToRemove.push_back(markerId);
    }

    for (int markerId : idsToRemove) {
        removeMarkerVisual(markerId);
    }

    m_selectedMarkerId = 0;

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::displayBox(std::shared_ptr<CadBox> box)
{
    if (m_context.IsNull() || box == nullptr) {
        return;
    }

    removeBoxVisual(box->id());

    BoxVisual visual = createBoxVisual(box);
    m_boxVisuals[box->id()] = visual;

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::updateBoxDisplay(std::shared_ptr<CadBox> box)
{
    if (m_context.IsNull() || box == nullptr) {
        return;
    }

    removeBoxVisual(box->id());

    BoxVisual visual = createBoxVisual(box);
    m_boxVisuals[box->id()] = visual;

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}

void OccView::removeBoxDisplay(int boxId)
{
    if (boxId <= 0) {
        return;
    }

    removeBoxVisual(boxId);

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}


void OccView::removeBoxVisual(int boxId)
{
    if (m_context.IsNull()) {
        return;
    }

    auto it = m_boxVisuals.find(boxId);

    if (it == m_boxVisuals.end()) {
        return;
    }

    if (!it->second.shape.IsNull()) {
        m_context->Remove(it->second.shape, Standard_False);
    }

    m_boxVisuals.erase(it);
}

OccView::BoxVisual OccView::createBoxVisual(
    const std::shared_ptr<CadBox>& box
)
{
    BoxVisual visual;

    TopoDS_Shape shape = BRepPrimAPI_MakeBox(
        box->length(),
        box->width(),
        box->height()
    ).Shape();

    gp_Trsf transform;
    transform.SetTranslation(gp_Vec(box->x(), box->y(), box->z()));
    shape.Location(TopLoc_Location(transform));

    Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
    aisShape->SetColor(Quantity_NOC_SKYBLUE);
    aisShape->SetDisplayMode(AIS_Shaded);

    m_context->Display(aisShape, Standard_False);

    visual.shape = aisShape;

    return visual;
}

void OccView::clearSceneObjects()
{
    clearSceneMarkers();

    std::vector<int> boxIds;

    for (const auto& [boxId, visual] : m_boxVisuals) {
        Q_UNUSED(visual);
        boxIds.push_back(boxId);
    }

    for (int boxId : boxIds) {
        removeBoxVisual(boxId);
    }

    if (!m_view.IsNull()) {
        m_view->Redraw();
    }
}