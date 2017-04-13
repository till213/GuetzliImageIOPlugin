#include <QtTest/QtTest>
#include <QImage>
#include <QSize>
#include <QBuffer>
#include <QVector>

#include "GuetzliImageIOPluginTest.h"


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
    image.save(&buffer, "JPG");

    QImage jpegImage;
    jpegImage.loadFromData(buffer.data(), "JPG");
    buffer.close();

    return jpegImage;
}

// Private slots

void GuetzliImageIOPluginTest::compareWithJPEG_data()
{
    const int Quality = 85;
    QSize size = {64, 64};

    QTest::addColumn<QImage>("sourceImage");
    QTest::addColumn<int>("quality");
    QTest::addColumn<QImage>("expectedImage");

    // ARGB32
    QImage image = create(size, QImage::Format_ARGB32);
    QImage jpegImage = createJPEGData(image);
    QTest::newRow("Format_ARGB32") << image << Quality << jpegImage;

    // Indexed 8
    image = create(size, QImage::Format_Indexed8);
    jpegImage = createJPEGData(image);
    QTest::newRow("Format_Indexed8") << image << Quality << jpegImage;

    // Grayscale 8
    image = create(size, QImage::Format_Grayscale8);
    jpegImage = createJPEGData(image);
    QTest::newRow("Format_Grayscale8") << image << Quality << jpegImage;
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
    sourceImage.save(QString(QTest::currentDataTag()) + "source.png", "PNG");

    // Verify
    QImage actualImage;
    actualImage.loadFromData(buffer.data(), "JPG");
    actualImage.save(QString(QTest::currentDataTag()) + "actual.png", "PNG");
    buffer.close();

    QVERIFY2(!actualImage.isNull(), "Actual image is valid");
    QVERIFY2(actualImage.size() == expectedImage.size(), "Same size as expected image");
    QVERIFY2(compareImages(actualImage, expectedImage), "Image sufficiently similar to expected JPEG image");
}

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

QTEST_MAIN(GuetzliImageIOPluginTest)
