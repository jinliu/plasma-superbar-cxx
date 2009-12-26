#include "TaskButton.h"

#include <Plasma/Applet>
#include <Plasma/IconWidget>
#include <KConfig>
#include <KDesktopFile>
#include <KUrl>
#include <taskmanager/abstractgroupableitem.h>
#include <taskmanager/groupmanager.h>
#include <taskmanager/taskactions.h>

#include <QGraphicsLinearLayout>

using TaskManager::AbstractGroupableItem;

class Superbar : public Plasma::Applet
{
    Q_OBJECT
public:
    Superbar(QObject *parent, const QVariantList &args);
    void init();

private slots:
    void taskGroupAdded(AbstractGroupableItem*);
    void taskGroupRemoved(AbstractGroupableItem*);
    void taskGroupPositionChanged(AbstractGroupableItem*);

private:
    QGraphicsLinearLayout* m_layout;
    TaskManager::GroupManager* m_groupManager;
};

K_EXPORT_PLASMA_APPLET(superbar-cxx, Superbar)


Superbar::Superbar(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_layout(new QGraphicsLinearLayout(this)),
    m_groupManager(new TaskManager::GroupManager(this))
{
    m_groupManager->setGroupingStrategy(TaskManager::GroupManager::ProgramGrouping);
    m_groupManager->setSortingStrategy(TaskManager::GroupManager::AlphaSorting);

    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
}


void Superbar::init()
{
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