#include "TaskButton.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KIconLoader>
#include <KRun>
#include <taskmanager/task.h>
#include <taskmanager/taskitem.h>
#include <taskmanager/taskgroup.h>

#include <QGraphicsSceneMouseEvent>

#include <limits>

using TaskManager::TaskPtr;
using TaskManager::TaskItem;
using TaskManager::TaskGroup;
using namespace std;


TaskButton::TaskButton(KUrl url, QGraphicsItem* parent)
    : QGraphicsWidget(parent),
      m_url(url),
      m_taskItem(NULL)
{
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

    init();
}


TaskButton::TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent)
    : QGraphicsWidget(parent),
      m_taskItem(taskItem),
      m_icon(taskItem->icon())
{
    init();
}


void TaskButton::init()
{
    setAcceptHoverEvents(true);

    m_background = new Plasma::FrameSvg(this);
    m_background->setImagePath("widgets/tasks");
    m_background->setElementPrefix("normal");
    m_background->getMargins(m_leftMargin, m_topMargin, m_rightMargin, m_bottomMargin);
}


void TaskButton::setTaskItem(AbstractGroupableItem* taskItem)
{
    m_taskItem = taskItem;
    update();
}


void TaskButton::resetTaskItem()
{
    m_taskItem = NULL;
    update();
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
    TaskPtr task = t->task();
    if (!task)
        return false;
    QString windowClass = task->classClass().toLower();
    foreach (QString key, m_keys)
        if (windowClass == key)
            return true;
    return false;
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

QSizeF TaskButton::buttonSize(qreal iconSize) const
{
    return QSizeF(iconSize+m_leftMargin+m_rightMargin,
                  iconSize+m_topMargin+m_bottomMargin);
}

void TaskButton::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const QRectF bounds(boundingRect());

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    if (m_taskItem)
    {
        m_background->resizeFrame(bounds.size());
        m_background->paintFrame(painter);
    }
    
    m_icon.paint(painter,
                 bounds.adjusted(m_leftMargin, m_topMargin,
                                 -m_rightMargin, -m_bottomMargin
                     ).toRect());
}


void TaskButton::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
}


void TaskButton::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        if (m_taskItem) // task mode
        {
            if (!m_taskItem->isGroupItem()) {
                TaskManager::TaskItem* t = static_cast<TaskManager::TaskItem*>(m_taskItem);
                TaskPtr task = t->task();
                if (task)
                    task->activateRaiseOrIconify();
            }
        } else // launcher mode
            new KRun(m_url, NULL);
        break;
    default:
        break;
    }
}


#include "TaskButton.moc"
