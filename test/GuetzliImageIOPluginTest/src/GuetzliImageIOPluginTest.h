#ifndef GUETZLIIMAGEIOPLUGINTEST_H
#define GUETZLIIMAGEIOPLUGINTEST_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QVector>

class GuetzliImageIOPluginTest : public QObject
{
    Q_OBJECT

private:
    static QVector<QRgb> m_clut;

    static QImage create(const QSize &size, QImage::Format format);
    static QImage createFilled(QRgb color, const QSize &size, QImage::Format format, bool alpha = false);

    static bool compareImages(const QImage &actualImage, const QImage &expectedImage);
    static QImage createJPEGData(const QImage &image);
    static void createTestData(const QSize &size, int quality);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void checkGuetzliPlugin();
    void compareWithJPEG_data();
    void compareWithJPEG();
};


#endif // GUETZLIIMAGEIOPLUGINTEST_H
