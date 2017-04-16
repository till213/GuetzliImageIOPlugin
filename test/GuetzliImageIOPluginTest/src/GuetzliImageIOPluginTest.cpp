#include <QtTest/QtTest>
#include <QImage>
#include <QSize>
#include <QBuffer>
#include <QVector>
#include <QPainter>
#include <QImageWriter>

#include "GuetzliImageIOPluginTest.h"

namespace {
    // Set to true in order to write test images to disk; helpful
    // for debugging
    constexpr bool WriteImagesToDisk = true;
} // anonymous

// Private

QImage GuetzliImageIOPluginTest::create(const QSize &size, QImage::Format format)
{
    QImage image = QImage(size, format);

    switch (format) {
    case QImage::Format_Grayscale8:
        // Create some grayscale image
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {
                int gray = qRound(static_cast<float>(x) / (image.width() - 1) * 255.0f);
                image.setPixel(x, y, gray);
            }

        }
        break;
    case QImage::Format_Indexed8:
        image.setColorTable(m_clut);
        // Create some index based image
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {
                int index = qRound(static_cast<float>(x) / (image.width() - 1) * (m_clut.size() - 1));
                image.setPixel(x, y, index);
            }

        }
        break;
    default:

        // Create some coloured image with alpha
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {

                QColor color;

                int r =       qRound(static_cast<float>(y) / (image.height() - 1) * 255.0f);
                int g =       qRound(static_cast<float>(x) / (image.width()  - 1) * 255.0f);
                int b = 255 - qRound(static_cast<float>(x) / (image.width()  - 1) * 255.0f);
                int a =       qRound(static_cast<float>(y) / (image.height() - 1) * 255.0f);

                color.setRed(r);
                color.setGreen(g);
                color.setBlue(b);
                color.setAlpha(a);

                image.setPixelColor(x, y, color);

            }

        }
        break;

    }

    return image;
}

QImage GuetzliImageIOPluginTest::createFilled(QRgb rgb, const QSize &size, QImage::Format format, bool alpha)
{
    QImage image = QImage(size, format);

    switch (format) {
    case QImage::Format_Grayscale8:
    {
        int gray = qGray(rgb);

        // Create some grayscale image
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {
                image.setPixel(x, y, gray);
            }

        }
        break;
    }
    case QImage::Format_Indexed8:
    {
        QVector<QRgb> clut;
        clut.resize(1);
        clut[0] = rgb;
        image.setColorTable(clut);
        // Create some index based image
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {
                image.setPixel(x, y, 0);
            }

        }
        break;
    }
    default:

        // Create some coloured image with alpha
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {

                int a;
                if (alpha) {
                    a = qRound(static_cast<float>(y) / (image.height() - 1) * 255.0f);
                } else {
                    a = 255;
                }
                QColor color;
                color.setRed(qRed(rgb));
                color.setGreen(qGreen(rgb));
                color.setBlue(qBlue(rgb));
                color.setAlpha(a);

                image.setPixelColor(x, y, color);

            }

        }
        break;

    }

    return image;
}

bool GuetzliImageIOPluginTest::compareImages(const QImage &actualImage, const QImage &expectedImage)
{
    // mean square errors for red, green, blue and alpha
    float mseRed;
    float mseGreen;
    float mseBlue;
    float mseAlpha;
    const float MseThreshold = 20.0;

    mseRed = mseGreen = mseBlue = mseAlpha = 0.0f;
    for (int y = 0; y < actualImage.height(); ++y) {

        for (int x = 0; x < actualImage.width(); ++x) {

            QRgb actualPixel = actualImage.pixel(x, y);
            QRgb expectedPixel = expectedImage.pixel(x, y);

            float diff = qRed(expectedPixel) - qRed(actualPixel);
            mseRed += diff * diff;

            diff = qGreen(expectedPixel) - qGreen(actualPixel);
            mseGreen += diff * diff;

            diff = qBlue(expectedPixel) - qBlue(actualPixel);
            mseBlue += diff * diff;

            diff = qAlpha(expectedPixel) - qAlpha(actualPixel);
            mseAlpha += diff * diff;

        }

    }

    float total = (actualImage.width() * actualImage.height());
    mseRed = mseRed / total;
    mseGreen = mseGreen / total;
    mseBlue = mseBlue / total;
    mseAlpha = mseAlpha / total;

    return mseRed < MseThreshold && mseGreen < MseThreshold && mseBlue < MseThreshold && mseAlpha < MseThreshold;
}

QImage GuetzliImageIOPluginTest::createJPEGData(const QImage &image)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    QImage noAlphaImage;
    // Guetzli blends images with an alpha channel against a
    // black background - so we do the same here
    if (image.hasAlphaChannel()) {
        noAlphaImage = QImage(image.size(), image.format());
        noAlphaImage.fill(Qt::black);
        QPainter painter(&noAlphaImage);
        painter.drawImage(0, 0, image);
        painter.end();
    } else {
        noAlphaImage = image;
    }

    noAlphaImage.save(&buffer, "JPG");

    QImage jpegImage;
    jpegImage.loadFromData(buffer.data(), "JPG");
    buffer.close();

    return jpegImage;
}

// Private slots

void GuetzliImageIOPluginTest::initTestCase()
{
    const int Size = 256;
    m_clut.resize(Size);

    for (int i = 0; i < Size; ++i) {

        QColor color;
        int r = qRound(static_cast<float>(i) / (Size - 1) * 255.0f);
        int g = qRound(255.0f - static_cast<float>(i) / (Size - 1) * 255.0f);
        int b = 255;
        color.setRed(r);
        color.setGreen(g);
        color.setBlue(b);

        m_clut[i] = color.rgb();
    }
}

void GuetzliImageIOPluginTest::cleanupTestCase()
{
    m_clut.clear();
}

void GuetzliImageIOPluginTest::checkGuetzliPlugin()
{

    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    bool hasGuetzliFormat = false;

    // Exercise
    for (QByteArray format : formats) {
        if (format == "guetzli") {
            hasGuetzliFormat = true;
            break;
        }
    }

    // Verify
    QVERIFY2(hasGuetzliFormat, "Has Guetzli Plugin");
}

void GuetzliImageIOPluginTest::compareWithJPEG_data()
{
    constexpr int MinimumQuality = 84;
    constexpr int HighQuality = 90;
    constexpr int MaxQuality = 100;
    constexpr int DefaultQuality = -1;
    QVector<int> qualities = {MinimumQuality, HighQuality, MaxQuality, DefaultQuality};
    constexpr QSize SmallSize = {32, 48};
    constexpr QSize OddSize = {201, 157};
    constexpr QSize AllColorsSize = {256, 256};
    QVector<QSize> sizes = {SmallSize, OddSize, AllColorsSize};

    QTest::addColumn<QImage>("sourceImage");
    QTest::addColumn<int>("quality");
    QTest::addColumn<QImage>("expectedImage");

    for (int quality : qualities) {
        for (QSize size : sizes) {
            // ARGB32 red with alpha gradient
            QImage image = createFilled(QColor(Qt::red).rgb(), size, QImage::Format_ARGB32, true);
            QImage jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_ARGB32-red-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;

            // ARGB32 green with alpha gradient
            image = createFilled(QColor(Qt::green).rgb(), size, QImage::Format_ARGB32, true);
            jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_ARGB32-green-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;

            // ARGB32 green with alpha gradient
            image = createFilled(QColor(Qt::blue).rgb(), size, QImage::Format_ARGB32, true);
            jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_ARGB32-blue-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;

            // ARGB32 coloured
            image = create(size, QImage::Format_ARGB32);
            jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_ARGB32-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;

            // Indexed 8
            image = create(size, QImage::Format_Indexed8);
            jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_Indexed8-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;

            // Grayscale 8
            image = create(size, QImage::Format_Grayscale8);
            jpegImage = createJPEGData(image);
            QTest::newRow(QString("Format_Grayscale8-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
                    << image << quality << jpegImage;
        }
    }
}

void GuetzliImageIOPluginTest::compareWithJPEG()
{
    QFETCH(QImage, sourceImage);
    QFETCH(int, quality);
    QFETCH(QImage, expectedImage);

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    // Exercise
    sourceImage.save(&buffer, "guetzli", quality);

    // Verify
    QImage actualImage;
    actualImage.loadFromData(buffer.data(), "JPG");
    buffer.close();

    if (WriteImagesToDisk) {
        sourceImage.save(QString(QTest::currentDataTag()) + "-source.png", "PNG");
        actualImage.save(QString(QTest::currentDataTag()) + "-actual.png", "PNG");
        expectedImage.save(QString(QTest::currentDataTag()) + "-expect.png", "PNG");
    }

    QVERIFY2(!actualImage.isNull(), "Actual image is valid");
    QVERIFY2(actualImage.size() == expectedImage.size(), "Same size as expected image");
    QVERIFY2(compareImages(actualImage, expectedImage), "Image sufficiently similar to expected JPEG image");
}

QTEST_MAIN(GuetzliImageIOPluginTest)
