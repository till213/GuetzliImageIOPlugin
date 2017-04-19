#include <QImageWriter>
#include <QImageReader>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QColorDialog>
#include <QIcon>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList standardLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (standardLocations.size() > 0) {
        m_lastSourceDirectory = standardLocations[0];
    }
    updateUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Private

QString MainWindow::suggestTargetFileName()
{
    QString suggestedName;

    suggestedName = QFileInfo(m_sourceFilePath).baseName() + ".jpg";
    return suggestedName;
}

void MainWindow::updateUi()
{
    bool enabled = !m_image.isNull();
    ui->saveAsAction->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);

    // Source file
    QFile sourceFile(m_sourceFilePath);
    if (!m_sourceFilePath.isEmpty() && sourceFile.exists()) {
        qint64 size = sourceFile.size();
        ui->sourceSizeLabel->setText(QString("%1").arg(size / 1024));
        this->setWindowTitle(QFileInfo(m_sourceFilePath).fileName());
    } else {
        ui->sourceSizeLabel->setText(QString());
        this->setWindowTitle(QString("Guetzli Simple GUI"));
    }
    if (!m_image.isNull()) {
        ui->sourceFormatLabel->setText(QString(m_sourceFormat));
    } else {
        ui->sourceFormatLabel->setText(QString());
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

// Private slots

void MainWindow::openImage()
{
    QString filter("Images (*.png *.tif *.tiff *.jpg *.jpeg);;PNG (*.png);;TIFF (*.tif *.tiff);; JPEG (*.jpg *.jpeg)");
    m_sourceFilePath = QFileDialog::getOpenFileName(this, tr("Open"), m_lastSourceDirectory, filter);
    if (!m_sourceFilePath.isNull()) {

        m_lastSourceDirectory = QFileInfo(m_sourceFilePath).absolutePath();
        QImageReader reader(m_sourceFilePath);
        m_sourceFormat = reader.format();
        m_image = reader.read();
        if (!m_image.isNull()) {

            QImage previewImage;
            if (m_image.width() > ui->imagePreviewLabel->maximumWidth() ||
                m_image.height() > ui->imagePreviewLabel->maximumHeight()
               ) {
                if (m_image.width() > m_image.height()) {
                    previewImage = m_image.scaledToWidth(ui->imagePreviewLabel->maximumWidth());
                } else {
                    previewImage = m_image.scaledToHeight(ui->imagePreviewLabel->maximumHeight());
                }
            } else {
                previewImage = m_image;
            }
            ui->imagePreviewLabel->setPixmap(QPixmap::fromImage(previewImage));
        } else {
            m_sourceFormat = "";
        }
    }
    updateUi();
}

void MainWindow::saveImage()
{
    QElapsedTimer elapsedTimer;

    if (!m_image.isNull()) {
        QString filter("Guetzli (*.jpg)");
        QString filePath;
        if (m_targetFilePath.isEmpty()) {
            filePath = m_lastSourceDirectory + "/" + this->suggestTargetFileName();
        } else {
            filePath = QFileInfo(m_targetFilePath).absolutePath() + "/" + this->suggestTargetFileName();
        }
        m_targetFilePath = QFileDialog::getSaveFileName(this, tr("Save"), filePath, filter);
        if (!m_targetFilePath.isNull()) {

            QImageWriter imageWriter;
            imageWriter.setFileName(m_targetFilePath);
            imageWriter.setFormat("guetzli");
            int quality = ui->qualitySpinBox->text().toInt();
            imageWriter.setQuality(quality);

            // Get some coffee now, together with some guetzli maybe
            QApplication::setOverrideCursor(Qt::WaitCursor);
            ui->statusBar->showMessage(tr("Baking guetzli..."));

            // Make sure that the file dialog gets a chance (event) to close
            // (Specifically on macOS)
            QCoreApplication::processEvents();

            elapsedTimer.start();
            bool success = imageWriter.write(m_image);
            m_elapsed = elapsedTimer.elapsed();
            if (success) {
                ui->statusBar->showMessage(tr("Guetzli ready."), 5000);
            } else {
                ui->statusBar->showMessage(tr("Baking guetzli failed."), 5000);
            }

            QApplication::restoreOverrideCursor();

        }
    }
    updateUi();
}


