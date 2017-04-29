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
#include <QMessageBox>
#include <QByteArray>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

#include "PluginInfoDialog.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pluginInfoDialog(nullptr)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);

    QStringList standardLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    if (standardLocations.size() > 0) {
        m_lastSourceDirectory = standardLocations[0];
    }
    updateUi();

    if (!hasGuetzliPlugin()) {
        QMessageBox::warning(this, tr("Guetzli Plugin Not Found"), tr("No Guetzli image plugin found."));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_pluginInfoDialog != nullptr) {
        delete m_pluginInfoDialog;
        m_pluginInfoDialog = nullptr;
    }
}

// Protected

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls()) {
        event->accept();
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls()) {
        m_sourceFilePath = mimeData->urls().first().toLocalFile();
        openImage(m_sourceFilePath);
    }
}

// Private

QString MainWindow::suggestTargetFileName()
{
    QString suggestedName = QFileInfo(m_sourceFilePath).baseName() + ".jpg";
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

bool MainWindow::hasGuetzliPlugin()
{
    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();
    QByteArray guetzliFormat = QByteArray("guetzli");
    bool hasGuetzli;

    hasGuetzli = false;
    for (QByteArray format : supportedFormats) {
        if (guetzliFormat == format) {
            hasGuetzli = true;
            break;
        }
    }

    return hasGuetzli;
}

void MainWindow::openImage(const QString &filePath)
{
    QImageReader imageReader(filePath);
    m_sourceFormat = imageReader.format();
    m_image = imageReader.read();
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
    updateUi();
}

// Private slots

void MainWindow::openImage()
{
    QString filter("Images (*.png *.tif *.tiff *.jpg *.jpeg);;PNG (*.png);;TIFF (*.tif *.tiff);; JPEG (*.jpg *.jpeg)");
    m_sourceFilePath = QFileDialog::getOpenFileName(this, tr("Open"), m_lastSourceDirectory, filter);
    if (!m_sourceFilePath.isNull()) {
        m_lastSourceDirectory = QFileInfo(m_sourceFilePath).absolutePath();
        openImage(m_sourceFilePath);
    }
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

void MainWindow::aboutQt()
{
    QApplication::aboutQt();
}

void MainWindow::showPluginInfo()
{
    if (m_pluginInfoDialog == nullptr) {
       m_pluginInfoDialog = new PluginInfoDialog(this);
    }
    m_pluginInfoDialog->show();
}


