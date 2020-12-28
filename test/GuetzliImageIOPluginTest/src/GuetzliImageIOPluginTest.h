#ifndef GUETZLIIMAGEIOPLUGINTEST_H
#define GUETZLIIMAGEIOPLUGINTEST_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QVector>

/*!
 * Test cases for the Guetzli Qt image plugin.
 *
 * Note: even on a fast CPU (e.g. Intel Core i7 with 4.2 GHz) the tests may run longer
 * than the default Qt test timeout (5 minutes respectively 60 seconds within Qt Creator).
 * So either set the environment variable QTEST_FUNCTION_TIMEOUT (default: 300000 ms) to
 * a higher value or - when the tests are run within Qt Creator - increase the timeout in the
 * Qt Creator settings (default: 60 seconds).
 *
 * Also refer to https://doc.qt.io/qtcreator/creator-autotest.html#building-and-running-tests
 */
class GuetzliImageIOPluginTest : public QObject
{
    Q_OBJECT

private:
    static QVector<QRgb> m_clut;

    static QImage createARGB32(const QSize &size);
    static QImage create(const QSize &size, QImage::Format format);
    static QImage createFilled(QRgb color, const QSize &size, QImage::Format format, bool alpha = false);

    static bool compareImages(const QImage &actualImage, const QImage &expectedImage, int quality);
    //static QImage createJPEGData(const QImage &image);
    static QImage createReference(const QImage &image);
    static void createTestData(const QSize &size, int quality);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void checkGuetzliPlugin();    
    void compareWithReference_data();
    void compareWithReference();
    void benchmark_data();
    void benchmark();

};


#endif // GUETZLIIMAGEIOPLUGINTEST_H
