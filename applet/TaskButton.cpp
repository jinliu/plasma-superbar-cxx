#include "TaskButton.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KRun>
#include <KUrl>
#include "taskmanager/taskitem.h"

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

    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

TaskButton::TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent)
    : Plasma::IconWidget(parent),
      m_taskItem(taskItem)
{
    setIcon(taskItem->icon());
    
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
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
