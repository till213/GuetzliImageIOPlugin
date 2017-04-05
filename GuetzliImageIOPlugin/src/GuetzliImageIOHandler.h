#ifndef GUETZLIIMAGEIOHANDLER_H
#define GUETZLIIMAGEIOHANDLER_H

#include <QImageIOHandler>
#include <QVariant>

class QImage;

class GuetzliImageIOHandler : public QImageIOHandler
{
public:

    const static int DefaultQuality;

    GuetzliImageIOHandler();

    virtual bool canRead() const override;
    virtual bool read(QImage *image) override;
    virtual bool write(const QImage &image) override;
    virtual QVariant option(ImageOption option) const override;
    virtual void setOption(ImageOption option, const QVariant &value) override;
    virtual bool supportsOption(ImageOption option) const override;

private:
    int m_quality;
};

#endif // GUETZLIIMAGEIOHANDLER_H
