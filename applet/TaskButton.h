#ifndef SUPERBAR_TASK_BUTTON_H
#define SUPERBAR_TASK_BUTTON_H

#include <Plasma/FrameSvg>
#include <KIcon>
#include <KUrl>
#include <taskmanager/abstractgroupableitem.h>
#include <taskmanager/taskgroup.h>

#include <QGraphicsWidget>
#include <QList>

using TaskManager::AbstractGroupableItem;

class TaskButton : public QGraphicsWidget
{
    Q_OBJECT
public:
    TaskButton(KUrl url);
    TaskButton(AbstractGroupableItem* item, QString windowClass);

    bool hasTask();
    bool hasLauncher();

    bool tryAddTaskItem(AbstractGroupableItem* item, QString windowClass);
    
public slots:    
    bool tryRemoveTaskItem(AbstractGroupableItem* item);

private:
    void init();
    QSizeF buttonSize(qreal iconSize) const;
    /*override*/ void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /*override*/ void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /*override*/ void paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget);
    /*override*/ QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const;
    
    KUrl m_url;
    QList<AbstractGroupableItem*> m_tasks;

    QList<QString> m_keys;
    KIcon m_icon;
    Plasma::FrameSvg* m_background;
    qreal m_leftMargin, m_topMargin, m_rightMargin, m_bottomMargin;
    TaskManager::TaskGroup* m_parentGroup;
};

#endif //SUPERBAR_TASK_BUTTON_H
