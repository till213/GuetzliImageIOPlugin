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
