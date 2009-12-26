#ifndef SUPERBAR_TASK_BUTTON_H
#define SUPERBAR_TASK_BUTTON_H

#include <Plasma/IconWidget>

#include <KUrl>

class TaskButton : public Plasma::IconWidget
{
    Q_OBJECT
public:
    TaskButton(KUrl url, QGraphicsItem* parent);

private slots:
    void launch();

private:
    KUrl m_url;
};

#endif //SUPERBAR_TASK_BUTTON_H
