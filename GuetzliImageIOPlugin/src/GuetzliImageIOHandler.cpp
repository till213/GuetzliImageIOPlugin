#include <QImage>
#include <QFile>
#include <QVariant>
#include <QColor>

#include <guetzli/processor.h>
#include <guetzli/quality.h>
#include <guetzli/stats.h>

#include "GuetzliImageIOHandler.h"

// Private

namespace
{

#if !defined(GUETZLI_BLEND_MODE) || (GUETZLI_BLEND_MODE == 1) // Blend on black background (default)
inline uint8_t blendOnBackground(uint8_t val, uint8_t alpha) {
    return (static_cast<int>(val) * static_cast<int>(alpha) + 128) / 255;
}

inline uint8_t blendPremultipliedOnBackground(uint8_t val, uint8_t alpha) {
    Q_UNUSED(alpha)
    return val;
}

#elif GUETZLI_BLEND_MODE == 2 // Blend on white background

inline uint8_t blendOnBackground(uint8_t val, uint8_t alpha) {
    return (static_cast<int>(val) * static_cast<int>(alpha) + 255 * (255 - alpha) + 128) / 255;
}

inline uint8_t blendPremultipliedOnBackground(uint8_t val, uint8_t alpha) {
    return val + (255 * (255 - alpha) + 128) / 255;
}

#elif GUETZLI_BLEND_MODE == 3 // Ignore alpha

inline uint8_t blendOnBackground(uint8_t val, uint8_t alpha) {
    Q_UNUSED(alpha) // alpha is ignored
    return val;
}

inline uint8_t blendPremultipliedOnBackground(uint8_t val, uint8_t alpha) {
    Q_UNUSED(alpha) // alpha is ignored and
    return val;     // premultiplied value is returned "as is"
}
#else
#error "Unsupported blend mode - see DEFINES in Common.pri"
#endif

} // anonymous

// Public

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

    QVariant quality = option(ImageOption::Quality);
    int qualityValue;
    if (quality.isValid()) {
        qualityValue = quality.toInt();
    } else {
        qualityValue = DefaultQuality;
    }

    params.butteraugli_target = guetzli::ButteraugliScoreForQuality(qualityValue);

    fetchRGB(image, &rgb);

    std::string outData;
    if (!guetzli::Process(params, &stats, rgb, image.width(), image.height(), &outData)) {
      return false;
    }

    return device()->write(outData.data(), outData.size()) != -1;
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
    {
        // By default the guetzli encoder expects a minimum quality of 84
        int val = value.toInt();
        if (val != -1) {
            m_quality = qBound(84, val, 100);
        } else {
            m_quality = GuetzliImageIOHandler::DefaultQuality;
        }

        break;
    }
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

// Private

const int GuetzliImageIOHandler::DefaultQuality = 85;

void GuetzliImageIOHandler::fetchRGB(const QImage &image, std::vector<uint8_t> *rgb)
{
    rgb->resize(3 * image.width() * image.height());

    QImage rgbImage;
    const uint8_t *bits = image.bits();
    const int size = image.width() * image.height();
    uint8_t *rgbp = &(*rgb)[0];
    switch(image.format()) {
    case QImage::Format::Format_ARGB32:

        for (int i = 0; i < size; ++i) {
            // 0xAARRGGBB
            uint8_t b = *bits++;
            uint8_t g = *bits++;
            uint8_t r = *bits++;
            uint8_t a = *bits++;

            // Convert to RGB888
            *rgbp++ = ::blendOnBackground(r, a);
            *rgbp++ = ::blendOnBackground(g, a);
            *rgbp++ = ::blendOnBackground(b, a);
        }
        break;

    case QImage::Format::Format_ARGB32_Premultiplied:

        for (int i = 0; i < size; ++i) {
            // 0xAARRGGBB
            uint8_t b = *bits++;
            uint8_t g = *bits++;
            uint8_t r = *bits++;
            uint8_t a = *bits++;

            // Convert to RGB888
            *rgbp++ = ::blendPremultipliedOnBackground(r, a);
            *rgbp++ = ::blendPremultipliedOnBackground(g, a);
            *rgbp++ = ::blendPremultipliedOnBackground(b, a);
        }
        break;

    default:
        if (image.format() != QImage::Format::Format_RGB888) {
            rgbImage = image.convertToFormat(QImage::Format::Format_RGB888);
        } else {
            rgbImage = image;
        }

        // RGB
        for (int y = 0; y < rgbImage.height(); ++y) {
            const uint8_t *row_in = rgbImage.scanLine(y);
            uint8_t *row_out = &(*rgb)[3 * y * rgbImage.width()];
            memcpy(row_out, row_in, 3 * rgbImage.width());
        }
        break;
    }
}
