#pragma once

#include "cad/CadObject.hpp"

class CadBox final : public CadObject
{
public:
    CadBox(
        int id,
        double x,
        double y,
        double z,
        double length,
        double width,
        double height
    );

    double x() const;
    double y() const;
    double z() const;

    double length() const;
    double width() const;
    double height() const;

    void setPosition(double x, double y, double z);
    void setDimensions(double length, double width, double height);

private:
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;

    double m_length = 100.0;
    double m_width = 80.0;
    double m_height = 60.0;
};