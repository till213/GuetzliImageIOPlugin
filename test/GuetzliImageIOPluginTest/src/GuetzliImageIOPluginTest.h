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
    QVector<QRgb> m_clut;

    QImage create(const QSize &size, QImage::Format format);

    static bool compareImages(const QImage &actualImage, const QImage &expectedImage);
    static QImage createJPEGData(const QImage &image);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void compareWithJPEG_data();
    void compareWithJPEG();
};


#endif // GUETZLIIMAGEIOPLUGINTEST_H
