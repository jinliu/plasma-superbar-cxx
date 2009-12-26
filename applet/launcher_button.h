#ifndef SUPERBAR_LAUNCHER_BUTTON_H
#define SUPERBAR_LAUNCHER_BUTTON_H

#include <Plasma/IconWidget>

#include <KUrl>

class LauncherButton : public Plasma::IconWidget
{
    Q_OBJECT
public:
    LauncherButton(KUrl url, QGraphicsItem* parent);

private slots:
    void launch();

private:
    KUrl m_url;
};

#endif //SUPERBAR_LAUNCHER_BUTTON_H
