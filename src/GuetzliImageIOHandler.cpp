#include <QImage>

#include "GuetzliImageIOHandler.h"

// Public

GuetzliImageIOHandler::GuetzliImageIOHandler()
{

}

bool GuetzliImageIOHandler::canRead() const
{
    return false;
}

bool GuetzliImageIOHandler::read(QImage *image)
{
    Q_UNUSED(image);
    return false;
}

bool GuetzliImageIOHandler::write(const QImage &image)
{
    return false;
}

bool GuetzliImageIOHandler::supportsOption(ImageOption option) const
{
    bool support;
    switch (option) {
    case ImageOption::Quality:
        support = true;
    default:
        support = false;
    }

    return support;
}
