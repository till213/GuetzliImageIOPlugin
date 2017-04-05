#include <QImage>
#include <QFile>
#include <QVariant>

#include <guetzli/processor.h>
#include <guetzli/quality.h>
#include <guetzli/stats.h>

#include "GuetzliImageIOHandler.h"

// Private

namespace
{

void fetchRGB(const QImage &image, std::vector<uint8_t> *rgb) {

    rgb->resize(3 * image.width() * image.height());

    // @todo Treat each format (that we want to support) individually - here we go the Cheap Way(tm)
    // by making sure that the image format is RGB, 8 bit per channel
    QImage rgbImage;
    if (image.format() != QImage::Format_RGB888) {
        rgbImage = image.convertToFormat(QImage::Format_RGB888);
    } else {
        rgbImage = image;
    }

    // RGB
    for (int y = 0; y < rgbImage.height(); ++y) {

        const uint8_t *row_in = rgbImage.scanLine(y);
        uint8_t *row_out = &(*rgb)[3 * y * rgbImage.width()];
        memcpy(row_out, row_in, 3 * rgbImage.width());

    }

}

} // anonymous

// Public

const int GuetzliImageIOHandler::DefaultQuality = 85;


GuetzliImageIOHandler::GuetzliImageIOHandler()
    : m_quality(DefaultQuality)
{

}

bool GuetzliImageIOHandler::canRead() const
{
    return false;
}

bool GuetzliImageIOHandler::read(QImage *image)
{
    Q_UNUSED(image)
    return false;
}

bool GuetzliImageIOHandler::write(const QImage &image)
{
    std::vector<uint8_t> rgb;
    guetzli::ProcessStats stats;
    guetzli::Params params;

    QVariant quality = this->option(ImageOption::Quality);
    int qualityValue;
    if (quality.isValid()) {
        qualityValue = quality.toInt();
        qDebug("Quality Option: %d", qualityValue);
    } else {
        qualityValue = DefaultQuality;
    }

    qDebug("Final Quality Option: %d", qualityValue);

    params.butteraugli_target = guetzli::ButteraugliScoreForQuality(qualityValue);

    ::fetchRGB(image, &rgb);

    std::string outData;
    if (!guetzli::Process(params, &stats, rgb, image.width(), image.height(), &outData)) {
      fprintf(stderr, "Guetzli processing failed\n");
      return false;
    }

    return this->device()->write(outData.data(), outData.size()) != -1;
}

QVariant GuetzliImageIOHandler::option(ImageOption option) const
{
    QVariant value;

    switch (option) {
    case ImageOption::Quality:
        value = m_quality;
        break;
    default:
        value = QImageIOHandler::option(option);
        break;
    }

    return value;
}

void GuetzliImageIOHandler::setOption(ImageOption option, const QVariant &value)
{
    switch (option) {
    case ImageOption::Quality:
        // By default the guetzli encoder expects a minimum quality of 84
        m_quality = qBound(84, value.toInt(), 100);
        break;
    default:
        QImageIOHandler::setOption(option, value);
        break;
    }
}

bool GuetzliImageIOHandler::supportsOption(ImageOption option) const
{
    bool support;
    switch (option) {
    case ImageOption::Quality:
        support = true;
        break;
    default:
        support = false;
        break;
    }

    return support;
}
