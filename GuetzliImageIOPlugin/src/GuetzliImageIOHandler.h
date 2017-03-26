#ifndef GUETZLIIMAGEIOHANDLER_H
#define GUETZLIIMAGEIOHANDLER_H

#include <QImageIOHandler>

class QImage;

class GuetzliImageIOHandler : public QImageIOHandler
{
public:

    const static int DefaultQuality;

    GuetzliImageIOHandler();

    virtual bool canRead() const override;
    virtual bool read(QImage *image) override;
    virtual bool write(const QImage &image) override;
    virtual bool supportsOption(ImageOption option) const override;
};

#endif // GUETZLIIMAGEIOHANDLER_H
