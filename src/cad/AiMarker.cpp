#include "cad/AiMarker.hpp"

AiMarker::AiMarker(
    int id,
    double x,
    double y,
    double z
)
    : CadObject(
          id,
          QString("Marker %1").arg(id),
          CadObjectType::AiMarker
      ),
      m_x(x),
      m_y(y),
      m_z(z)
{
}

double AiMarker::x() const
{
    return m_x;
}

double AiMarker::y() const
{
    return m_y;
}

double AiMarker::z() const
{
    return m_z;
}

void AiMarker::setPosition(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}