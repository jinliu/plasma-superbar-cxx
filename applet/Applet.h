#ifndef SUPERBAR_APPLET_H
#define SUPERBAR_APPLET_H

#include <Plasma/Applet>
#include <taskmanager/abstractgroupableitem.h>
#include <taskmanager/groupmanager.h>

#include <QGraphicsLinearLayout>

using TaskManager::AbstractGroupableItem;

class Superbar : public Plasma::Applet
{
    Q_OBJECT
public:
    Superbar(QObject *parent, const QVariantList &args);
    /*override*/ void init();

private slots:
    void taskGroupAdded(AbstractGroupableItem*);
    void taskGroupRemoved(AbstractGroupableItem*);
    void taskGroupPositionChanged(AbstractGroupableItem*);

private:    
    QGraphicsLinearLayout* m_layout;
    TaskManager::GroupManager* m_groupManager;
};

#endif //SUPERBAR_APPLET_H
