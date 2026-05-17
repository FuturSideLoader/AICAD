#include "cad/CadBox.hpp"

CadBox::CadBox(
    int id,
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
    : CadObject(
          id,
          QString("Box %1").arg(id),
          CadObjectType::Box
      ),
      m_x(x),
      m_y(y),
      m_z(z),
      m_length(length),
      m_width(width),
      m_height(height)
{
}

double CadBox::x() const
{
    return m_x;
}

double CadBox::y() const
{
    return m_y;
}

double CadBox::z() const
{
    return m_z;
}

double CadBox::length() const
{
    return m_length;
}

double CadBox::width() const
{
    return m_width;
}

double CadBox::height() const
{
    return m_height;
}

void CadBox::setPosition(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void CadBox::setDimensions(double length, double width, double height)
{
    m_length = length;
    m_width = width;
    m_height = height;
}