#pragma once

#include "cad/CadObject.hpp"

class AiMarker final : public CadObject
{
public:
    AiMarker(
        int id,
        double x,
        double y,
        double z
    );

    double x() const;
    double y() const;
    double z() const;

    void setPosition(double x, double y, double z);

private:
    double m_x = 0.0;
    double m_y = 0.0;
    double m_z = 0.0;
};