#include "commands/AddBoxCommand.hpp"

AddBoxCommand::AddBoxCommand(
    CadDocument& document,
    double x,
    double y,
    double z,
    double length,
    double width,
    double height
)
    : m_document(document),
      m_x(x),
      m_y(y),
      m_z(z),
      m_length(length),
      m_width(width),
      m_height(height)
{
}

QString AddBoxCommand::name() const
{
    return "Add Box";
}

bool AddBoxCommand::execute()
{
    if (m_length <= 0.0 || m_width <= 0.0 || m_height <= 0.0) {
        return false;
    }

    if (m_createdBoxId <= 0) {
        std::shared_ptr<CadBox> box = m_document.addBox(
            m_x,
            m_y,
            m_z,
            m_length,
            m_width,
            m_height
        );

        if (box == nullptr) {
            return false;
        }

        m_createdBoxId = box->id();
        m_createdBoxName = box->name();

        return true;
    }

    std::shared_ptr<CadBox> box = m_document.addBoxWithId(
        m_createdBoxId,
        m_createdBoxName,
        m_x,
        m_y,
        m_z,
        m_length,
        m_width,
        m_height
    );

    return box != nullptr;
}

bool AddBoxCommand::undo()
{
    if (m_createdBoxId <= 0) {
        return false;
    }

    return m_document.removeBoxById(m_createdBoxId);
}