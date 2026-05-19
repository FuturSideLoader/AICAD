#pragma once

#include <QWidget>

#include <memory>
#include <vector>

class QListWidget;
class QListWidgetItem;

class AiMarker;
class CadBox;

class ObjectTreePanel final : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectTreePanel(QWidget* parent = nullptr);

    void addOrUpdateMarker(const std::shared_ptr<AiMarker>& marker);
    void addOrUpdateBox(const std::shared_ptr<CadBox>& box);

    void removeObject(int objectId);
    void clearObjects();

    void rebuild(
        const std::vector<std::shared_ptr<AiMarker>>& markers,
        const std::vector<std::shared_ptr<CadBox>>& boxes
    );

    void selectObject(int objectId);

signals:
    void objectSelected(int objectId);

private slots:
    void onItemClicked(QListWidgetItem* item);

private:
    void addOrUpdateItem(int objectId, const QString& text);
    QListWidgetItem* findItemByObjectId(int objectId) const;

private:
    QListWidget* m_list = nullptr;
};