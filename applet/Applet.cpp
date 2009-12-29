#include "Applet.h"
#include "TaskButton.h"

#include <Plasma/Containment>
#include <KConfig>
#include <KDesktopFile>
#include <KUrl>
#include <taskmanager/taskactions.h>


K_EXPORT_PLASMA_APPLET(superbar-cxx, Superbar)


Superbar::Superbar(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_layout(new QGraphicsLinearLayout(this)),
    m_groupManager(new TaskManager::GroupManager(this))
{
    m_groupManager->setGroupingStrategy(TaskManager::GroupManager::ProgramGrouping);
    m_groupManager->setSortingStrategy(TaskManager::GroupManager::NoSorting);

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
        if (url.isValid() && url.isLocalFile() && KDesktopFile::isDesktopFile(url.toLocalFile())) {
            TaskButton* button = new TaskButton(url, this);
            m_layout->addItem(button);
        }
    }

    setLayout(m_layout);
    
    // Setup TaskManager
    connect(
        m_groupManager->rootGroup(), SIGNAL(itemAdded(AbstractGroupableItem*)),
        this, SLOT(taskGroupAdded(AbstractGroupableItem*)));
    connect(
        m_groupManager->rootGroup(), SIGNAL(itemRemoved(AbstractGroupableItem*)),
        this, SLOT(taskGroupRemoved(AbstractGroupableItem*)));
    connect(
        m_groupManager->rootGroup(), SIGNAL(itemPositionChanged(AbstractGroupableItem*)),
        this, SLOT(taskGroupPositionChanged(AbstractGroupableItem*)));
    m_groupManager->reconnect();
    foreach (AbstractGroupableItem* item, m_groupManager->rootGroup()->members())
        taskGroupAdded(item);
}


void Superbar::taskGroupAdded(AbstractGroupableItem* taskItem)
{
    for (int i=0; i<m_layout->count(); ++i)
    {
        TaskButton* button = static_cast<TaskButton*>(m_layout->itemAt(i));
        if (button->matches(taskItem)) {
            if (button->hasTask())
                qWarning("taskGroupAdded: item already exist: %s", qPrintable(taskItem->name()));
            else
                button->setTaskItem(taskItem);
            return;
        }
    }
    TaskButton* button = new TaskButton(taskItem, this);
    m_layout->addItem(button);
}


void Superbar::taskGroupRemoved(AbstractGroupableItem* taskItem)
{
    for (int i=0; i<m_layout->count(); ++i)
    {
        TaskButton* button = static_cast<TaskButton*>(m_layout->itemAt(i));
        if (button->matches(taskItem)) {
            if (button->hasLauncher())
                button->resetTaskItem();
            else {
                m_layout->removeAt(i);
                delete button;
            }
            return;
        }
    }
    qWarning("taskGroupRemoved: trying to remove non-existant task: %s", qPrintable(taskItem->name()));
}


void Superbar::taskGroupPositionChanged(AbstractGroupableItem*)
{}


#include "Applet.moc"
