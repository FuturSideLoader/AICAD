#include "ui/ObjectTreePanel.hpp"

#include "cad/AiMarker.hpp"
#include "cad/CadBox.hpp"

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QVariant>

ObjectTreePanel::ObjectTreePanel(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_list = new QListWidget(this);
    layout->addWidget(m_list);

    setLayout(layout);

    connect(
        m_list,
        &QListWidget::itemClicked,
        this,
        &ObjectTreePanel::onItemClicked
    );
}

void ObjectTreePanel::addOrUpdateMarker(
    const std::shared_ptr<AiMarker>& marker
)
{
    if (marker == nullptr) {
        return;
    }

    addOrUpdateItem(marker->id(), marker->name());
}

void ObjectTreePanel::addOrUpdateBox(
    const std::shared_ptr<CadBox>& box
)
{
    if (box == nullptr) {
        return;
    }

    addOrUpdateItem(box->id(), box->name());
}

void ObjectTreePanel::removeObject(int objectId)
{
    QListWidgetItem* item = findItemByObjectId(objectId);

    if (item == nullptr) {
        return;
    }

    const int row = m_list->row(item);
    delete m_list->takeItem(row);
}

void ObjectTreePanel::clearObjects()
{
    m_list->clear();
}

void ObjectTreePanel::rebuild(
    const std::vector<std::shared_ptr<AiMarker>>& markers,
    const std::vector<std::shared_ptr<CadBox>>& boxes
)
{
    clearObjects();

    for (const auto& marker : markers) {
        addOrUpdateMarker(marker);
    }

    for (const auto& box : boxes) {
        addOrUpdateBox(box);
    }
}

void ObjectTreePanel::selectObject(int objectId)
{
    QListWidgetItem* item = findItemByObjectId(objectId);

    if (item == nullptr) {
        m_list->clearSelection();
        return;
    }

    m_list->setCurrentItem(item);
}

void ObjectTreePanel::onItemClicked(QListWidgetItem* item)
{
    if (item == nullptr) {
        return;
    }

    const int objectId = item->data(Qt::UserRole).toInt();

    if (objectId > 0) {
        emit objectSelected(objectId);
    }
}

void ObjectTreePanel::addOrUpdateItem(int objectId, const QString& text)
{
    if (objectId <= 0) {
        return;
    }

    QListWidgetItem* existingItem = findItemByObjectId(objectId);

    if (existingItem != nullptr) {
        existingItem->setText(text);
        return;
    }

    auto* item = new QListWidgetItem(text);
    item->setData(Qt::UserRole, objectId);

    m_list->addItem(item);
}

QListWidgetItem* ObjectTreePanel::findItemByObjectId(int objectId) const
{
    if (m_list == nullptr) {
        return nullptr;
    }

    for (int index = 0; index < m_list->count(); ++index) {
        QListWidgetItem* item = m_list->item(index);

        if (item != nullptr && item->data(Qt::UserRole).toInt() == objectId) {
            return item;
        }
    }

    return nullptr;
}