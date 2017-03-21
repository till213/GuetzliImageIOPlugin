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
    // @todo Implement me
    return Capability::CanWrite;
}

QImageIOHandler *GuetzliImageIOPlugin::create(QIODevice *device, const QByteArray &format) const
{
    // @todo Implement me
    return new GuetzliImageIOHandler();
}
