#ifndef GUETZLIIMAGEIOHANDLER_H
#define GUETZLIIMAGEIOHANDLER_H

#include <vector>

#include <QImageIOHandler>
#include <QVariant>
#include <QColor>

class QImage;

class GuetzliImageIOHandler : public QImageIOHandler
{
public:
    GuetzliImageIOHandler();

    virtual bool canRead() const override;
    virtual bool read(QImage *image) override;
    virtual bool write(const QImage &image) override;
    virtual QVariant option(ImageOption option) const override;
    virtual void setOption(ImageOption option, const QVariant &value) override;
    virtual bool supportsOption(ImageOption option) const override;

private:
    const static int DefaultQuality;
    int m_quality;

    void fetchRGB(const QImage &image, std::vector<uint8_t> *rgb);
};

#endif // GUETZLIIMAGEIOHANDLER_H
