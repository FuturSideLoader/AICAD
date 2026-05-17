#include "cad/CadObject.hpp"

CadObject::CadObject(
    int id,
    QString name,
    CadObjectType type
)
    : m_id(id),
      m_name(std::move(name)),
      m_type(type)
{
}

int CadObject::id() const
{
    return m_id;
}

QString CadObject::name() const
{
    return m_name;
}

CadObjectType CadObject::type() const
{
    return m_type;
}

void CadObject::setName(const QString& name)
{
    m_name = name;
}