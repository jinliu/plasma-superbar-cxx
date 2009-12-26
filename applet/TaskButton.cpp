#include "TaskButton.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KRun>
#include <KUrl>
#include <KWindowSystem>
#include <taskmanager/taskitem.h>
#include <taskmanager/taskgroup.h>

using TaskManager::TaskItem;
using TaskManager::TaskGroup;

TaskButton::TaskButton(KUrl url, QGraphicsItem* parent)
    : Plasma::IconWidget(parent),
      m_url(url),
      m_taskItem(NULL)
{
    KDesktopFile desktopFile(m_url.toLocalFile());
    KConfigGroup config = desktopFile.desktopGroup();
    QString executable = config.readPathEntry("Exec", QString());
    QString title = desktopFile.readName();
    if (title.isEmpty())
        title = m_url.fileName();
    QString description = desktopFile.readGenericName();
    if (description.isEmpty())
        description = desktopFile.readComment();
    if (description.isEmpty())
        description = m_url.path();
    QString icon = desktopFile.readIcon();
    setIcon(icon);

    // Build matching keys.
    // A launcher matches a task if the task's window class matches
    // the launcher's title or executable name.
    //TODO: refine this rule.
    m_keys.push_back(title.toLower());
    m_keys.push_back(executable.split(" ").front().split("/").back());
    
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

TaskButton::TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent)
    : Plasma::IconWidget(parent),
      m_taskItem(taskItem)
{
    setIcon(taskItem->icon());
    
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

void TaskButton::setTaskItem(AbstractGroupableItem* taskItem)
{
    m_taskItem = taskItem;
}

void TaskButton::resetTaskItem()
{
    m_taskItem = NULL;
}

bool TaskButton::matches(AbstractGroupableItem* taskItem)
{
    if (m_taskItem)
        return m_taskItem == taskItem;
    TaskItem* t;
    if (taskItem->isGroupItem())
        t = static_cast<TaskItem*>(static_cast<TaskGroup*>(taskItem)->members().first());
    else
        t = static_cast<TaskItem*>(taskItem);
    QString windowClass = t->task()->classClass().toLower();
    foreach (QString key, m_keys)
        if (windowClass == key)
            return true;
    return false;
}

void TaskButton::launch()
{
    if (m_taskItem) // task mode
    {
        if (!m_taskItem->isGroupItem()) {
            TaskManager::TaskItem* t = static_cast<TaskManager::TaskItem*>(m_taskItem);
            t->task()->activateRaiseOrIconify();
        }
    } else // launcher mode
        new KRun(m_url, NULL);
}

#include "TaskButton.moc"
