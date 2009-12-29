#include "Applet.h"
#include "TaskButton.h"

#include <Plasma/Containment>
#include <KConfig>
#include <KDesktopFile>
#include <KUrl>
#include <taskmanager/task.h>
#include <taskmanager/taskgroup.h>
#include <taskmanager/taskitem.h>

using namespace TaskManager;


K_EXPORT_PLASMA_APPLET(superbar-cxx, Superbar)



Superbar::Superbar(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_layout(new QGraphicsLinearLayout(this)),
    m_groupManager(new GroupManager(this))
{
    m_groupManager->setGroupingStrategy(GroupManager::ProgramGrouping);
    m_groupManager->setSortingStrategy(GroupManager::AlphaSorting);

    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
}


void Superbar::init()
{
    m_layout->setContentsMargins(0, 0, 0, 0);

    // Create launchers from Kickoff favorites
    KConfig kickoffConf("kickoffrc", KConfig::NoGlobals);
    KConfigGroup favoritesGroup(&kickoffConf, "Favorites");
    QStringList favorites = favoritesGroup.readEntry("FavoriteURLs", QStringList());

    foreach (QString i, favorites) {
        KUrl url(i);
        if (url.isValid() && url.isLocalFile() && KDesktopFile::isDesktopFile(url.toLocalFile()))
            m_layout->addItem(new TaskButton(url));
    }

    setLayout(m_layout);
    
    // Setup TaskManager
    connect(
        m_groupManager->rootGroup(), SIGNAL(itemAdded(AbstractGroupableItem*)),
        this, SLOT(addTaskItem(AbstractGroupableItem*)));
    foreach (AbstractGroupableItem* item, m_groupManager->rootGroup()->members())
        addTaskItem(item);
}


void Superbar::addTaskItem(AbstractGroupableItem* item)
{
    // Get windowClassClass from item
    QString windowClass;
    if (item->isGroupItem())
        windowClass = item->name();
    else {
        TaskItem* taskItem = qobject_cast<TaskItem*>(item);
        if (!taskItem)
            return;
        TaskPtr task = taskItem->task();
        if (!task)
            return;
        windowClass = task->classClass();
    }

    for (int i=0; i<m_layout->count(); ++i) {
        TaskButton* button = static_cast<TaskButton*>(m_layout->itemAt(i));
        if (button->tryAddTaskItem(item, windowClass))
            return;
    }
    m_layout->addItem(new TaskButton(item, windowClass));
}


#include "Applet.moc"
