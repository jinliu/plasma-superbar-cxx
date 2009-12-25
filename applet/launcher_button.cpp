#include "launcher_button.h"

#include <KConfigGroup>
#include <KDesktopFile>
#include <KRun>
#include <KUrl>

LauncherButton::LauncherButton(KUrl url, QGraphicsItem* parent)
    : Plasma::IconWidget(parent),
      m_url(url)
{
    KDesktopFile desktopFile(m_url.toLocalFile());
    KConfigGroup config = desktopFile.desktopGroup();
    QString executable = config.readPathEntry("Exec", QString());
    QString title = desktopFile.readName();
    if (title.isEmpty())
        title = m_url.fileName();
    QString description = desktopFile.readGenericName();
    if (description.isEmpty())
        description = desktopFile.readComment();
    if (description.isEmpty())
        description = m_url.path();
    QString icon = desktopFile.readIcon();
    setIcon(icon);

    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

void LauncherButton::launch()
{
    new KRun(m_url, NULL);
}

#include "launcher_button.moc"
