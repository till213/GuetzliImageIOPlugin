#ifndef GUETZLIIMAGEIOPLUGIN_H
#define GUETZLIIMAGEIOPLUGIN_H

#include <QImageIOPlugin>


class GuetzliImageIOPlugin : public QImageIOPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QImageIOHandlerFactoryInterface" FILE "GuetzliIOPlugin.json")
#endif // QT_VERSION >= 0x050000

public:
    GuetzliImageIOPlugin(QObject *parent = nullptr);

    virtual Capabilities capabilities(QIODevice *device, const QByteArray &format) const override;
    virtual QImageIOHandler *create(QIODevice *device, const QByteArray &format) const override;
};

#endif // GUETZLIIMAGEIOPLUGIN_H
