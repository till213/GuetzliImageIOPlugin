#include <QImageWriter>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QStandardPaths>
#include <QElapsedTimer>


#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_image = new QImage();
    QStringList standardLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (standardLocations.size() > 0) {
        m_sourceFilePath = standardLocations[0];
    }
    updateUi();
}

MainWindow::~MainWindow()
{
    delete m_image;
    delete ui;
}

// Private

void MainWindow::updateUi()
{
    bool enabled = !m_image->isNull();
    ui->saveAsAction->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);

    // Source file
    QFile sourceFile(m_sourceFilePath);
    if (!m_sourceFilePath.isEmpty() && sourceFile.exists()) {
        qint64 size = sourceFile.size();
        ui->sourceSizeLabel->setText(QString("%1").arg(size / 1024));
    } else {
        ui->sourceSizeLabel->setText(QString());
    }
    if (!m_image->isNull()) {
        ui->sourceFormatLabel->setText(QString(m_sourceFormat));
        if (m_sourceGamma != 0.0) {
            ui->sourceGammaLabel->setText(QString("%1").arg(m_sourceGamma));
        } else {
            ui->sourceGammaLabel->setText(tr("N/A"));
        }
    } else {
        ui->sourceFormatLabel->setText(QString());
        ui->sourceGammaLabel->setText(QString());
    }

    // Target file (guetzli)
    QFile targetFile(m_targetFilePath);
    if (!m_targetFilePath.isEmpty() && targetFile.exists()) {
        qint64 size = targetFile.size();
        ui->targetSizeLabel->setText(QString("%1").arg(size / 1024));
        ui->durationLabel->setText(QString("%1").arg(m_elapsed / 1000.0f));
    } else {
        ui->targetSizeLabel->setText(QString());
        ui->durationLabel->setText(QString());
    }

}

// Private Slots

void MainWindow::openImage()
{
    QString filter("Images (*.png *.tif *.tiff *.jpg *.jpeg);;PNG (*.png);;TIFF (*.tif *.tiff);; JPEG (*.jpg *.jpeg)");
    m_sourceFilePath = QFileDialog::getOpenFileName(this, tr("Open"), m_sourceFilePath, filter);
    this->setWindowTitle(m_sourceFilePath);
    if (!m_sourceFilePath.isNull()) {

        QImageReader reader(m_sourceFilePath);
        m_sourceFormat = reader.format();
        *m_image = reader.read();
        if (!m_image->isNull()) {

            m_sourceGamma = reader.gamma();
            QImage previewImage;
            if (m_image->width() > ui->imagePreviewLabel->maximumWidth() ||
                m_image->height() > ui->imagePreviewLabel->maximumHeight()
               ) {
                if (m_image->width() > m_image->height()) {
                    previewImage = m_image->scaledToWidth(ui->imagePreviewLabel->maximumWidth());
                } else {
                    previewImage = m_image->scaledToHeight(ui->imagePreviewLabel->maximumHeight());
                }
            } else {
                previewImage = *m_image;
            }
            ui->imagePreviewLabel->setPixmap(QPixmap::fromImage(previewImage));
        } else {
            m_sourceFormat = "";
            m_sourceGamma = 0.0f;
        }
    }
    updateUi();
}

void MainWindow::saveImage()
{
    QElapsedTimer elapsedTimer;
    if (!m_image->isNull()) {
        m_targetFilePath = QFileDialog::getSaveFileName(this, tr("Save"), m_targetFilePath);
        if (!m_targetFilePath.isNull()) {
            QImageWriter imageWriter;
            imageWriter.setFileName(m_targetFilePath);
            imageWriter.setFormat("guetzli");
            int quality = ui->qualitySpinBox->text().toInt();
            imageWriter.setQuality(quality);
            elapsedTimer.start();
            imageWriter.write(*m_image);
            m_elapsed = elapsedTimer.elapsed();
        }
    }
    updateUi();
}

