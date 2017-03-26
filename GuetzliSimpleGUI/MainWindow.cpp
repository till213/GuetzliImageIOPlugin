#include <QImageWriter>
#include <QImage>
#include <QFileDialog>


#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openAction_triggered()
{
    qDebug("Open Action");
    m_filePath = QFileDialog::getOpenFileName(this, tr("Open"), m_filePath);
    ui->m_filePathLineEdit->setText(m_filePath);
}

void MainWindow::on_saveAsAction_triggered()
{
    QList<QByteArray> supportedImageFormats = QImageWriter::supportedImageFormats();

    foreach (QByteArray imageFormat, supportedImageFormats) {
        qDebug("%s", qPrintable(imageFormat));
    }

    QImage image;

    if (!m_filePath.isEmpty()) {
        bool res = image.load(this->m_filePath, "jpg");
        qDebug("Image load success: %d", res);

        QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save"), m_filePath);

        QImageWriter imageWriter;
        imageWriter.setFileName(saveFilePath);
        imageWriter.setFormat("guetzli");
        imageWriter.setQuality(85);
        imageWriter.write(image);
    }

}
