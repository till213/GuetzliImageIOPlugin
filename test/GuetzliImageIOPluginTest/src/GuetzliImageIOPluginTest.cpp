#include <QtTest/QtTest>
#include <QImage>
#include <QSize>
#include <QBuffer>

#include "GuetzliImageIOPluginTest.h"


// Private

bool GuetzliImageIOPluginTest::compareImages(const QImage &actualImage, const QImage &expectedImage)
{
    // mean square errors for red, green, blue and alpha
    double mseRed;
    double mseGreen;
    double mseBlue;
    double mseAlpha;
    const double MseThreshold = 20.0;

    mseRed = mseGreen = mseBlue = mseAlpha = 0.0;
    for (int y = 0; y < actualImage.height(); ++y) {

        for (int x = 0; x < actualImage.width(); ++x) {

            QRgb actualPixel = actualImage.pixel(x, y);
            QRgb expectedPixel = expectedImage.pixel(x, y);

            double diff = qRed(expectedPixel) - qRed(actualPixel);
            mseRed += diff * diff;

            diff = qGreen(expectedPixel) - qGreen(actualPixel);
            mseGreen += diff * diff;

            diff = qBlue(expectedPixel) - qBlue(actualPixel);
            mseBlue += diff * diff;

            diff = qAlpha(expectedPixel) - qAlpha(actualPixel);
            mseAlpha += diff * diff;

        }

    }

    double total = (actualImage.width() * actualImage.height());
    mseRed = mseRed / total;
    mseGreen = mseGreen / total;
    mseBlue = mseBlue / total;
    mseAlpha = mseAlpha / total;

    return mseRed < MseThreshold && mseGreen < MseThreshold && mseBlue < MseThreshold && mseAlpha < MseThreshold;

}

QImage GuetzliImageIOPluginTest::create(const QSize &size, QImage::Format format)
{
    QImage image = QImage(size, format);

    // Create some coloured image with alpha
    for (int y = 0; y < image.height(); ++y) {

        for (int x = 0; x < image.width(); ++x) {

            QColor color;

            int r =       qRound(static_cast<float>(y) / (image.height() - 1) * 255);
            int g =       qRound(static_cast<float>(x) / (image.width()  - 1) * 255);
            int b = 255 - qRound(static_cast<float>(x) / (image.width()  - 1) * 255);
            int a =       qRound(static_cast<float>(y) / (image.height() - 1) * 255);

            color.setRed(r);
            color.setGreen(g);
            color.setBlue(b);
            color.setAlpha(a);

            // @todo Check format whether it is monochrome or palette based
            image.setPixelColor(x, y, color);

        }

    }

    return image;
}

// Private slots

void GuetzliImageIOPluginTest::save_data()
{
    QSize size = {64, 64};

    QTest::addColumn<QImage>("sourceImage");
    QTest::addColumn<int>("quality");
    QTest::addColumn<QImage>("expectedImage");

    QImage image = create(size, QImage::Format_ARGB32);

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);
    image.save(&buffer, "JPG");

    QImage jpegImage;
    jpegImage.loadFromData(buffer.data(), "JPG");
    buffer.close();
    QTest::newRow("Format_ARGB32") << image << 85 << jpegImage;

}

void GuetzliImageIOPluginTest::save()
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
    QVERIFY(this->compareImages(actualImage, expectedImage));
}

void GuetzliImageIOPluginTest::initTestCase()
{
}

void GuetzliImageIOPluginTest::cleanupTestCase()
{
}

QTEST_MAIN(GuetzliImageIOPluginTest)
