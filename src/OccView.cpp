#include "OccView.hpp"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QShowEvent>

#include <Aspect_DisplayConnection.hxx>
#include <Aspect_NeutralWindow.hxx>

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <Quantity_Color.hxx>

OccView::OccView(QWidget* parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    setMouseTracking(true);
}

QPaintEngine* OccView::paintEngine() const
{
    return nullptr;
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
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.08, V3d_ZBUFFER);
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

    if (!m_view.IsNull() && event->button() == Qt::LeftButton) {
        m_view->StartRotation(event->position().x(), event->position().y());
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
