#ifndef SUPERBAR_TASK_BUTTON_H
#define SUPERBAR_TASK_BUTTON_H

#include <Plasma/FrameSvg>
#include <KIcon>
#include <KUrl>
#include <taskmanager/abstractgroupableitem.h>

#include <QGraphicsWidget>
#include <QList>
#include <QPainter>

using TaskManager::AbstractGroupableItem;

class TaskButton : public QGraphicsWidget
{
    Q_OBJECT
public:
    TaskButton(KUrl url, QGraphicsItem* parent);
    TaskButton(AbstractGroupableItem* taskItem, QGraphicsItem* parent);

    void setTaskItem(AbstractGroupableItem* taskItem);
    void resetTaskItem();
    bool hasTask() { return m_abstractItem; }
    bool hasLauncher() { return !m_url.isEmpty(); }

    bool matches(AbstractGroupableItem* abstractItem);

private:
    void init();
    QSizeF buttonSize(qreal iconSize) const;
    /*override*/ void paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget);
    /*override*/ void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /*override*/ void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /*override*/ QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const;
    
    KUrl m_url;
    AbstractGroupableItem* m_abstractItem;

    QList<QString> m_keys;
    KIcon m_icon;
    Plasma::FrameSvg* m_background;
    qreal m_leftMargin, m_topMargin, m_rightMargin, m_bottomMargin;
};

#endif //SUPERBAR_TASK_BUTTON_H
