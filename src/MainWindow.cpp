#include "MainWindow.hpp"
#include "OccView.hpp"

#include <QMenuBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_view(new OccView(this))
{
    setWindowTitle("AICAD - CAD for Linux");

    setCentralWidget(m_view);

    auto* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Exit", this, &QWidget::close);

    auto* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Fit All", m_view, &OccView::fitAll);

    statusBar()->showMessage("AICAD V0.1 - OpenCascade viewer ready");
}