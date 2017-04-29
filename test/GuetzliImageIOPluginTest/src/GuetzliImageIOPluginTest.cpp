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
    constexpr bool WriteImagesToDisk = false;

    constexpr int MinimumQuality = 84;
    constexpr int HighQuality = 90;
    constexpr int MaxQuality = 100;
    constexpr int DefaultQuality = -1;

    constexpr QSize SmallSize = {32, 48};
    constexpr QSize OddSize = {201, 157};
    constexpr QSize LargeSize = {256, 256};

} // anonymous

// Private

QVector<QRgb> GuetzliImageIOPluginTest::m_clut;

QImage GuetzliImageIOPluginTest::createARGB32(const QSize &size)
{
    QImage image = QImage(size, QImage::Format_ARGB32);

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

    return image;
}

QImage GuetzliImageIOPluginTest::create(const QSize &size, QImage::Format format)
{
    QImage image;

    switch (format) {
    case QImage::Format_Grayscale8:
    {
        QImage grayscale8Image = QImage(size, QImage::Format_Grayscale8);
        // Create some grayscale image
        for (int y = 0; y < size.height(); ++y) {

            for (int x = 0; x < size.width(); ++x) {
                uint gray = qRound(static_cast<float>(x) / (size.width() - 1) * 255.0f);
                // Note: even though the final format uses 8bit gray values we still need to
                // set a proper RGB (gray) value here; also refer to https://bugreports.qt.io/browse/QTBUG-53101
                QColor grayColor = QColor(gray, gray, gray);
                grayscale8Image.setPixel(x, y, grayColor.rgb());
            }

        }
        image = grayscale8Image;
        break;

    }

    case QImage::Format_Indexed8:
    {
        QImage indexed8Image = QImage(size, QImage::Format_Indexed8);
        indexed8Image.setColorTable(m_clut);
        // Create some index based image
        for (int y = 0; y < size.height(); ++y) {

            for (int x = 0; x < size.width(); ++x) {
                int index = qRound(static_cast<float>(x) / (size.width() - 1) * (m_clut.size() - 1));
                indexed8Image.setPixel(x, y, index);
            }

        }
        image = indexed8Image;
        break;
    }

    case QImage::Format_ARGB32_Premultiplied:
    {
        QImage argb32Image = createARGB32(size);
        image = argb32Image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        break;
    }

    default:
        image = createARGB32(size);
        break;
    }

    return image;
}

QImage GuetzliImageIOPluginTest::createFilled(QRgb rgb, const QSize &size, QImage::Format format, bool alphaGradient)
{
    QImage image = QImage(size, format);

    switch (format) {
    case QImage::Format_Grayscale8:
    {
        int gray = qGray(rgb);
        // Note: even though the final format uses 8bit gray values we still need to
        // set a proper RGB (gray) value here; also refer to https://bugreports.qt.io/browse/QTBUG-53101
        QColor grayColor = QColor(gray, gray, gray);

        // Create some grayscale image
        for (int y = 0; y < image.height(); ++y) {

            for (int x = 0; x < image.width(); ++x) {               
                image.setPixel(x, y, grayColor.rgb());            }

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
                if (alphaGradient) {
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

bool GuetzliImageIOPluginTest::compareImages(const QImage &actualImage, const QImage &expectedImage, int quality)
{
    // mean square errors for red, green, blue and alpha
    float mseRed;
    float mseGreen;
    float mseBlue;
    float mseAlpha;
    // Heuristically evaluated threshold such that small size, low quality (q=84)
    // guetzli images still pass the test:
    // Quality 84: Threshold 100.0f
    // Quality 100: Threshold 40.0f
    const float MseThreshold = 100.0f - (60.0f - 60.0f * (1.0f - ((quality - 84.0f) / 16.0f)));

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

    qDebug("width: %d height: %d quality: %d mseRed: %f mseGreen: %f mseBlue: %f mseAlpha: %f threshold: %f",
           actualImage.width(), actualImage.height(), quality, mseRed, mseGreen, mseBlue, mseAlpha, MseThreshold);

    return mseRed <= MseThreshold && mseGreen <= MseThreshold && mseBlue <= MseThreshold && mseAlpha <= MseThreshold;
}

QImage GuetzliImageIOPluginTest::createReference(const QImage &image)
{
    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    QImage noAlphaImage;

    if (image.hasAlphaChannel()) {
        noAlphaImage = QImage(image.size(), image.format());

#if !defined(GUETZLI_BLEND_MODE) || (GUETZLI_BLEND_MODE == 1)
        noAlphaImage.fill(Qt::black);
        QPainter painter(&noAlphaImage);
        painter.drawImage(0, 0, image);
        painter.end();
#elif (GUETZLI_BLEND_MODE == 2)
        noAlphaImage.fill(Qt::white);
        QPainter painter(&noAlphaImage);
        painter.drawImage(0, 0, image);
        painter.end();
#elif (GUETZLI_BLEND_MODE == 3)
        noAlphaImage = QImage(image.size(), QImage::Format::Format_RGB32);
        const uchar *sp = image.bits();
        uchar *dp = noAlphaImage.bits();
        for (int i = 0; i < image.byteCount() / 4; ++i) {
            *dp++ = *sp++; // blue
            *dp++ = *sp++; // green
            *dp++ = *sp++; // red
            *dp++ = 0xff; // opaque alpha
            // ignore source alpha
            sp++;
        }

#else
#error "Unsupported blend mode - see DEFINES in Common.pri"
#endif

    } else {
        noAlphaImage = image;
    }

    return noAlphaImage;
}

void GuetzliImageIOPluginTest::createTestData(const QSize &size, int quality)
{
    // ARGB32 red with alpha gradient
    QImage image = createFilled(QColor(Qt::red).rgb(), size, QImage::Format_ARGB32, true);
    QImage referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32-red-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 green with alpha gradient
    image = createFilled(QColor(Qt::green).rgb(), size, QImage::Format_ARGB32, true);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32-green-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 green with alpha gradient
    image = createFilled(QColor(Qt::blue).rgb(), size, QImage::Format_ARGB32, true);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32-blue-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 premultiplied red with alpha gradient
    image = createFilled(QColor(Qt::red).rgb(), size, QImage::Format_ARGB32_Premultiplied, true);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32_Premultiplied-red-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 premultiplied green with alpha gradient
    image = createFilled(QColor(Qt::green).rgb(), size, QImage::Format_ARGB32_Premultiplied, true);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32_Premultiplied-green-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 premultiplied blue with alpha gradient
    image = createFilled(QColor(Qt::blue).rgb(), size, QImage::Format_ARGB32_Premultiplied, true);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32_Premultiplied-blue-alpha-gradient-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // Format_Grayscale8 gray
    image = createFilled(QColor(Qt::gray).rgb(), size, QImage::Format_Grayscale8);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_Grayscale8-gray-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // Format_Indexed8 red
    image = createFilled(QColor(Qt::red).rgb(), size, QImage::Format_Indexed8);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_Indexed8-red-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 pattern
    image = create(size, QImage::Format_ARGB32);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // ARGB32 Premultiplied pattern
    image = create(size, QImage::Format_ARGB32_Premultiplied);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_ARGB32_Premultiplied-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // Indexed 8 pattern
    image = create(size, QImage::Format_Indexed8);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_Indexed8-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;

    // Grayscale 8 pattern
    image = create(size, QImage::Format_Grayscale8);
    referenceImage = createReference(image);
    QTest::newRow(QString("Format_Grayscale8-pattern-%1x%2-q%3").arg(size.width()).arg(size.height()).arg(quality).toLatin1())
            << image << quality << referenceImage;
}

// Private slots

void GuetzliImageIOPluginTest::initTestCase()
{
    if (QSysInfo::macVersion() != QSysInfo::MV_None) {
        // The guetzli plugin is in the SimpleGuetzliGUI application bundle
        QString applicationPluginDirectory = QCoreApplication::applicationDirPath() + "/SimpleGuetzliGUI.app/Contents/plugins";
        QCoreApplication::addLibraryPath(applicationPluginDirectory);
    }

    // Build color lookup table (clut)
    constexpr int Size = 256;
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

void GuetzliImageIOPluginTest::compareWithReference_data()
{
    const QVector<int> qualities = {::MinimumQuality, ::HighQuality, ::MaxQuality, ::DefaultQuality};
    const QVector<QSize> sizes = {::SmallSize, ::OddSize, ::LargeSize};

    QTest::addColumn<QImage>("sourceImage");
    QTest::addColumn<int>("quality");
    QTest::addColumn<QImage>("expectedImage");

    for (const QSize size : sizes) {
        // Test patterns
        createTestData(size, HighQuality);
    }

    for (const int quality : qualities) {
        // Test patterns
        createTestData(SmallSize, quality);

        // Photo 1
        QImage image = QImage(":/img/Los Angeles.png");
        QImage referenceImage = createReference(image);
        QTest::newRow(QString("Los Angeles-%1x%2-q%3").arg(image.width()).arg(image.height()).arg(quality).toLatin1())
                << image << quality << referenceImage;

        // Photo 2
        image = QImage(":/img/Route 66.png");
        referenceImage = createReference(image);
        QTest::newRow(QString("Route 66-%1x%2-q%3").arg(image.width()).arg(image.height()).arg(quality).toLatin1())
                << image << quality << referenceImage;

        // Photo 3
        image = QImage(":/img/Zurich.png");
        referenceImage = createReference(image);
        QTest::newRow(QString("Zurich-%1x%2-q%3").arg(image.width()).arg(image.height()).arg(quality).toLatin1())
                << image << quality << referenceImage;
    }
}

void GuetzliImageIOPluginTest::compareWithReference()
{
    // Setup
    QFETCH(QImage, sourceImage);
    QFETCH(int, quality);
    QFETCH(QImage, expectedImage);

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    // Exercise
    sourceImage.save(&buffer, "guetzli", quality);

    // Verify
    QImage actualImage;
    actualImage.loadFromData(buffer.data(), "jpg");
    buffer.close();

    if (WriteImagesToDisk) {
        sourceImage.save(QString(QTest::currentDataTag()) + "-source.png", "PNG");
        actualImage.save(QString(QTest::currentDataTag()) + "-actual.png", "PNG");
        expectedImage.save(QString(QTest::currentDataTag()) + "-expect.png", "PNG");
    }

    QVERIFY2(!actualImage.isNull(), "Actual image is valid");
    QVERIFY2(actualImage.size() == expectedImage.size(), "Same size as expected image");
    QVERIFY2(compareImages(actualImage, expectedImage, quality), "Image sufficiently similar to expected reference image");
}

void GuetzliImageIOPluginTest::benchmark_data()
{
    const QVector<int> qualities = {::MinimumQuality, ::HighQuality, ::MaxQuality};

    QTest::addColumn<QImage>("image");
    QTest::addColumn<int>("quality");
    for (const int quality : qualities) {
        // Photo 1
        QImage image = QImage(":/img/Los Angeles.png");
        QTest::newRow(QString("Los Angeles-%1x%2-q%3").arg(image.width()).arg(image.height()).arg(quality).toLatin1())
                << image << quality;
    }
}

void GuetzliImageIOPluginTest::benchmark()
{
    // Setup
    QFETCH(QImage, image);
    QFETCH(int, quality);

    QBuffer buffer;
    buffer.open(QBuffer::ReadWrite);

    // Exercise
    QBENCHMARK {
        image.save(&buffer, "guetzli", quality);
    }

    buffer.close();
}


QTEST_MAIN(GuetzliImageIOPluginTest)
