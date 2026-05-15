#pragma once

#include <QMainWindow>

class OccView;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    OccView* m_view;

};