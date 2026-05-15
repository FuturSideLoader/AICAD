#include <QApplication>
#include <QtGlobal>
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    // OpenCascade integrates more reliably with the X11 backend than Wayland
    // when attaching to a native Qt widget handle.
    if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")
        && qEnvironmentVariable("XDG_SESSION_TYPE") == "wayland") {
        qputenv("QT_QPA_PLATFORM", "xcb");
    }

    QApplication app(argc, argv);

    MainWindow window;
    window.resize(1280, 800);
    window.show();

    return app.exec();
}
