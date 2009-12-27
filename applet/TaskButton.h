#ifndef SUPERBAR_TASK_BUTTON_H
#define SUPERBAR_TASK_BUTTON_H

#include <Plasma/IconWidget>
#include <KUrl>
#include <taskmanager/abstractgroupableitem.h>

#include <QList>

using TaskManager::AbstractGroupableItem;

class TaskButton : public Plasma::IconWidget
{
    Q_OBJECT
public:
    TaskButton(KUrl url, QGraphicsItem* parent);
    TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent);

    void setTaskItem(AbstractGroupableItem* taskItem);
    void resetTaskItem();
    bool hasTask() { return m_taskItem; }
    bool hasLauncher() { return !m_url.isEmpty(); }
    
    bool matches(AbstractGroupableItem* taskItem);
    
private slots:
    void launch();

private:
    KUrl m_url;
    QList<QString> m_keys;
    AbstractGroupableItem* m_taskItem;
};

#endif //SUPERBAR_TASK_BUTTON_H
