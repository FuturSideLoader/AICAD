#pragma once

#include <QWidget>
#include <QPaintEngine>

#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <OpenGl_GraphicDriver.hxx>

class OccView final : public QWidget
{
    Q_OBJECT

public:
    explicit OccView(QWidget* parent = nullptr);

public slots:
    void fitAll();

protected:
    QPaintEngine* paintEngine() const override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    void initializeOpenCascade();
    void createDemoShape();

private:
    bool m_initialized = false;

    Handle(OpenGl_GraphicDriver) m_graphicDriver;
    Handle(V3d_Viewer) m_viewer;
    Handle(V3d_View) m_view;
    Handle(AIS_InteractiveContext) m_context;

    QPoint m_lastMousePosition;
};
