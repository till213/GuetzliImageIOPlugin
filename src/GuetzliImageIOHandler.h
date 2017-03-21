#ifndef GUETZLIIMAGEIOHANDLER_H
#define GUETZLIIMAGEIOHANDLER_H

#include <QImageIOHandler>

class QImage;

class GuetzliImageIOHandler : public QImageIOHandler
{
public:
    GuetzliImageIOHandler();

    bool canRead() const override;
    bool read(QImage *image) override;
};

#endif // GUETZLIIMAGEIOHANDLER_H
