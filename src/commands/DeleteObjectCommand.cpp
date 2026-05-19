#include "commands/DeleteObjectCommand.hpp"

#include "cad/AiMarker.hpp"
#include "cad/CadBox.hpp"

DeleteObjectCommand::DeleteObjectCommand(
    CadDocument& document,
    int objectId,
    DeleteObjectKind kind
)
    : m_document(document),
      m_objectId(objectId),
      m_kind(kind)
{
}

QString DeleteObjectCommand::name() const
{
    return "Delete Object";
}

bool DeleteObjectCommand::execute()
{
    if (m_objectId <= 0 || m_kind == DeleteObjectKind::None) {
        return false;
    }

    if (!m_captured && !captureObjectBeforeDelete()) {
        return false;
    }

    if (m_kind == DeleteObjectKind::Marker) {
        return m_document.removeMarkerById(m_objectId);
    }

    if (m_kind == DeleteObjectKind::Box) {
        return m_document.removeBoxById(m_objectId);
    }

    return false;
}

bool DeleteObjectCommand::undo()
{
    if (!m_captured || m_objectId <= 0) {
        return false;
    }

    if (m_kind == DeleteObjectKind::Marker) {
        return m_document.addMarkerWithId(
                   m_objectId,
                   m_name,
                   m_x,
                   m_y,
                   m_z
               ) != nullptr;
    }

    if (m_kind == DeleteObjectKind::Box) {
        return m_document.addBoxWithId(
                   m_objectId,
                   m_name,
                   m_x,
                   m_y,
                   m_z,
                   m_length,
                   m_width,
                   m_height
               ) != nullptr;
    }

    return false;
}

bool DeleteObjectCommand::captureObjectBeforeDelete()
{
    if (m_kind == DeleteObjectKind::Marker) {
        std::shared_ptr<AiMarker> marker = m_document.findMarkerById(m_objectId);

        if (marker == nullptr) {
            return false;
        }

        m_name = marker->name();
        m_x = marker->x();
        m_y = marker->y();
        m_z = marker->z();

        m_captured = true;
        return true;
    }

    if (m_kind == DeleteObjectKind::Box) {
        std::shared_ptr<CadBox> box = m_document.findBoxById(m_objectId);

        if (box == nullptr) {
            return false;
        }

        m_name = box->name();
        m_x = box->x();
        m_y = box->y();
        m_z = box->z();
        m_length = box->length();
        m_width = box->width();
        m_height = box->height();

        m_captured = true;
        return true;
    }

    return false;
}