#ifndef SUPERBAR_TASK_BUTTON_H
#define SUPERBAR_TASK_BUTTON_H

#include <Plasma/IconWidget>
#include <KUrl>

#include <taskmanager/abstractgroupableitem.h>

using TaskManager::AbstractGroupableItem;

class TaskButton : public Plasma::IconWidget
{
    Q_OBJECT
public:
    TaskButton(KUrl url, QGraphicsItem* parent);
    TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent);

private slots:
    void launch();

private:
    KUrl m_url;
    AbstractGroupableItem* m_taskItem;
};

#endif //SUPERBAR_TASK_BUTTON_H
