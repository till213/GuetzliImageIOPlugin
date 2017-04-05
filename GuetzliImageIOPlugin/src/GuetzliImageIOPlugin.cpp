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

    if (format == "jpeg" || format == "jpg" || format == "guetzli") {
        return Capability::CanWrite;
    } else {
        return 0;
    }

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
