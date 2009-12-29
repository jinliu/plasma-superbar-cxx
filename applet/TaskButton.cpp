#include "TaskButton.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KIconLoader>
#include <KRun>
#include <taskmanager/task.h>
#include <taskmanager/taskitem.h>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>

using namespace TaskManager;
using namespace std;


TaskButton::TaskButton(KUrl url)
    : m_url(url)
{
    init();

    KDesktopFile desktopFile(url.toLocalFile());
    KConfigGroup config = desktopFile.desktopGroup();
    QString executable = config.readPathEntry("Exec", QString());
    /*
    QString title = desktopFile.readName();
    if (title.isEmpty())
        title = m_url.fileName();
    QString description = desktopFile.readGenericName();
    if (description.isEmpty())
        description = desktopFile.readComment();
    if (description.isEmpty())
        description = m_url.path();
    */
    QString icon = desktopFile.readIcon();
    m_icon = KIcon(icon);

    // Build matching keys.
    // A launcher matches a task if the task's window class matches
    // the launcher's title or comment or executable name.
    //TODO: refine this rule.
    if (!desktopFile.readName().isEmpty())
        m_keys.push_back(desktopFile.readName().toLower());
    if (!desktopFile.readComment().isEmpty())
        m_keys.push_back(desktopFile.readComment().toLower());
    m_keys.push_back(executable.split(" ").front()
                     .split("/").back().toLower());
}


TaskButton::TaskButton(AbstractGroupableItem* item, QString windowClass)
    : m_icon(item->icon())
{
    init();

    windowClass = windowClass.toLower();
    m_keys.push_back(windowClass);
    tryAddTaskItem(item, windowClass);
}


void TaskButton::init()
{
    setAcceptHoverEvents(true);

    m_background = new Plasma::FrameSvg(this);
    m_background->setImagePath("widgets/tasks");
    m_background->setElementPrefix("normal");
    m_background->getMargins(m_leftMargin, m_topMargin, m_rightMargin, m_bottomMargin);

    m_parentGroup = NULL;
}


bool TaskButton::hasTask()
{
    return !m_tasks.empty();
}


bool TaskButton::hasLauncher()
{
    return !m_url.isEmpty();
}


bool TaskButton::tryAddTaskItem(AbstractGroupableItem* item, QString windowClass)
{
    if (m_tasks.contains(item)) {
        qWarning("TaskButton::tryAddTaskItem: item already exist: %s", qPrintable(windowClass));
        return true;
    }
    windowClass = windowClass.toLower();
    foreach (QString key, m_keys)
        if (windowClass == key) {
            m_tasks.push_back(item);
            if (m_tasks.size() == 1) {
                m_parentGroup = item->parentGroup();
                if (m_parentGroup)
                    connect(m_parentGroup, SIGNAL(itemRemoved(AbstractGroupableItem*)),
                            this, SLOT(tryRemoveTaskItem(AbstractGroupableItem*)));
                else
                    qWarning("TaskButton::tryAddTaskItem: no parentGroup: %s", qPrintable(windowClass));
                update();
            }
            return true;
        }
    return false;
}


bool TaskButton::tryRemoveTaskItem(AbstractGroupableItem* item)
{
    if (m_tasks.removeAll(item) == 0)
        return false;
    if (m_tasks.empty()) {
        disconnect(m_parentGroup, SIGNAL(itemRemoved(AbstractGroupableItem*)),
                   this, SLOT(tryRemoveTaskItem(AbstractGroupableItem*)));
        m_parentGroup = NULL;
        if (hasLauncher())
            update();
        else
            delete this;
    }
    return true;
}


QSizeF TaskButton::buttonSize(qreal iconSize) const
{
    return QSizeF(iconSize+m_leftMargin+m_rightMargin,
                  iconSize+m_topMargin+m_bottomMargin);
}


void TaskButton::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
}


void TaskButton::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        if (hasTask()) { // task mode
            if (!m_tasks.front()->isGroupItem()) {
                TaskItem* taskItem = qobject_cast<TaskItem*>(m_tasks.front());
                if (taskItem) {
                    TaskPtr task = taskItem->task();
                    if (task)
                        task->activateRaiseOrIconify();
                }
            }
        } else // launcher mode
            new KRun(m_url, NULL);
        break;
    default:
        break;
    }
}


void TaskButton::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    const QRectF bounds(boundingRect());
    if (hasTask()) {
        m_background->resizeFrame(bounds.size());
        m_background->paintFrame(painter);
    }    
    m_icon.paint(painter,
                 bounds.adjusted(m_leftMargin, m_topMargin,
                                 -m_rightMargin, -m_bottomMargin
                     ).toRect());
}


QSizeF TaskButton::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    if (which == Qt::PreferredSize)
        return buttonSize(KIconLoader::SizeLarge);
    else if (which == Qt::MinimumSize)
        return buttonSize(KIconLoader::SizeSmall);
    else
        return QGraphicsWidget::sizeHint(which, constraint);
}


#include "TaskButton.moc"
