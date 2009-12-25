#include "launcher_button.h"

#include <Plasma/Applet>

#include <KConfig>
#include <KDesktopFile>
#include <KUrl>

#include <QGraphicsLinearLayout>

class Superbar : public Plasma::Applet
{
    Q_OBJECT
public:
    Superbar(QObject *parent, const QVariantList &args);
    void init();

private:
    QGraphicsLinearLayout* m_layout;
};

K_EXPORT_PLASMA_APPLET(superbar-cxx, Superbar)


Superbar::Superbar(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
      m_layout(new QGraphicsLinearLayout(this))
{}


void Superbar::init()
{
    setHasConfigurationInterface(false);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);

    KConfig kickoffConf("kickoffrc", KConfig::NoGlobals);
    KConfigGroup favoritesGroup(&kickoffConf, "Favorites");
    QStringList favorites = favoritesGroup.readEntry("FavoriteURLs", QStringList());
    
    for (int i = 0; i < favorites.size(); ++i) {
        KUrl url(favorites[i]);
        if (url.isValid() && url.isLocalFile() && KDesktopFile::isDesktopFile(url.toLocalFile())) {
            LauncherButton* button = new LauncherButton(url, this);
            m_layout->addItem(button);
        }
    }
    
    setLayout(m_layout);
}

#include "main.moc"
