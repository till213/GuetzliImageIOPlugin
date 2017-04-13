#ifndef GUETZLIIMAGEIOPLUGINTEST_H
#define GUETZLIIMAGEIOPLUGINTEST_H

#include <QObject>
#include <QImage>
#include <QSize>

class GuetzliImageIOPluginTest : public QObject
{
    Q_OBJECT

private:
    bool compareImages(const QImage &actualImage, const QImage &expectedImage);
    QImage create(const QSize &size, QImage::Format format);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void save_data();
    void save();
};


#endif // GUETZLIIMAGEIOPLUGINTEST_H
