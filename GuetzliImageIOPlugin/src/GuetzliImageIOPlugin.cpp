#include "GuetzliImageIOHandler.h"
#include "GuetzliImageIOPlugin.h"

// Public

GuetzliImageIOPlugin::GuetzliImageIOPlugin(QObject *parent) :
    QImageIOPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(GuetzliIOPlugin, ImageIOPlugin)
#endif // QT_VERSION < 0x050000

GuetzliImageIOPlugin::Capabilities GuetzliImageIOPlugin::capabilities(QIODevice *device, const QByteArray &format) const
{
    Q_UNUSED(device)

     QFlags<Capability> capabilities;

    // Note: even though the Guetzli encoder writes perfectly valid JPEG images we do not "register" it
    //       as such (format "jpg" or "jpeg"), because we would "hide" (depending on the plugin loading
    //       order) the actual Qt JPEG plugin
    if (format == "guetzli") {
        capabilities |= Capability::CanWrite;
    }
    return capabilities;
}

QImageIOHandler *GuetzliImageIOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    Q_UNUSED(format)

    GuetzliImageIOHandler *guetzliImageIOHandler;

    guetzliImageIOHandler = new GuetzliImageIOHandler();
    guetzliImageIOHandler->setFormat("guetzli");
    guetzliImageIOHandler->setDevice(device);

    return guetzliImageIOHandler;
}
