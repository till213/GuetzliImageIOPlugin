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
