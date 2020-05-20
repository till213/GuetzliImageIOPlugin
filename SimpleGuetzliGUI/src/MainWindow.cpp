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
#include <QPainter>
#include <QIcon>

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
    initUi();
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
        ui->imagePreviewLabel->setStyleSheet("border: 2qpx solid black");
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    ui->imagePreviewLabel->setStyleSheet("border: 1px solid black");
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls()) {
        m_sourceFilePath = mimeData->urls().first().toLocalFile();
        openImageFromSourceFilePath();
    }
    ui->imagePreviewLabel->setStyleSheet("border: 1px solid black");
}

// Private

const QString &MainWindow::BlackText = tr("Black");
const QString &MainWindow::WhiteText = tr("White");
const QString &MainWindow::IgnoreAlphaText = tr("Ignore alpha");

QString MainWindow::suggestTargetFileName() const
{
    QString suggestedName = QFileInfo(m_sourceFilePath).baseName() + ".jpg";
    return suggestedName;
}

void MainWindow::initUi()
{
    // The @2x variants are added automagically
    m_checkedIcon.addFile(":/img/CheckboxChecked16x16.png/");
    m_uncheckedIcon.addFile(":/img/CheckboxUnchecked16x16.png/");
}

void MainWindow::updateUi()
{
    const QSize LabelSize = QSize(16, 16);
    bool enabled = !m_sourceImage.isNull();
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
        this->setWindowTitle(tr("Guetzli Simple GUI"));
    }
    if (!m_sourceImage.isNull()) {
        ui->sourceFormatLabel->setText(QString(m_sourceFormat).toUpper());
        bool hasAlpha = m_sourceImage.hasAlphaChannel();
        if (hasAlpha) {
            ui->hasAlphaLabel->setPixmap(m_checkedIcon.pixmap(LabelSize));
        } else {
            ui->hasAlphaLabel->setPixmap(m_uncheckedIcon.pixmap(LabelSize));
        }
    } else {
        ui->sourceFormatLabel->setText(QString());
        ui->hasAlphaLabel->setPixmap(m_uncheckedIcon.pixmap(LabelSize));
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
    if (!m_sourceImage.isNull()) {
        bool hasAlpha = m_sourceImage.hasAlphaChannel();
        ui->previewBlendingAlphaCheckBox->setEnabled(hasAlpha);
        if (hasAlpha) {
#if !defined(GUETZLI_BLEND_MODE) || (GUETZLI_BLEND_MODE == 1)
                    ui->backgroundLabel->setText(BlackText);
#elif (GUETZLI_BLEND_MODE == 2)
        ui->backgroundLabel->setText(WhiteText);
#elif (GUETZLI_BLEND_MODE == 3)
        ui->backgroundLabel->setText(IgnoreAlphaText);
#else
#error "Unsupported blend mode - see DEFINES in Common.pri"
#endif
        } else {
            ui->previewBlendingAlphaCheckBox->setEnabled(false);
            ui->backgroundLabel->setText(tr("Fully opaque"));
        }
    } else {
        ui->previewBlendingAlphaCheckBox->setEnabled(false);
        ui->backgroundLabel->setText("");
    }
}

bool MainWindow::hasGuetzliPlugin() const
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

void MainWindow::openImageFromSourceFilePath()
{
    QImageReader imageReader(m_sourceFilePath);
    m_sourceFormat = imageReader.format();
    m_sourceImage = imageReader.read();
    if (!m_sourceImage.isNull()) {
        updateImagePreview();
    } else {
        m_sourceFormat = "";
    }
    updateUi();
}

QImage MainWindow::createCheckeredBackgroundImage(const QSize &size)
{
    static QImage checkeredBackgroundImage;

    if (checkeredBackgroundImage.isNull() || checkeredBackgroundImage.size() != size) {
        checkeredBackgroundImage = QImage(size, QImage::Format::Format_ARGB32_Premultiplied);
        QRgb *d = reinterpret_cast<QRgb *>(checkeredBackgroundImage.bits());
        for (int y = 0; y < checkeredBackgroundImage.height(); ++y) {
            for (int x = 0; x < checkeredBackgroundImage.width(); ++x) {
                QRgb gray;
                if ((x % 16 < 8) && (y % 16 < 8)) {
                    gray = 0xff999999;
                } else if ((x % 16 >= 8) && (y % 16 < 8)) {
                    gray = 0xff666666;
                }
                else if ((x % 16 < 8) && (y % 16 >= 8)) {
                    gray = 0xff666666;
                } else {
                    gray = 0xff999999;
                }
                *d++ = gray;
            }
        }
    }
    return checkeredBackgroundImage;
}

// Private slots

void MainWindow::openImage()
{
    QString filter(tr("Images (*.png *.tif *.tiff *.jpg *.jpeg);;PNG (*.png);;TIFF (*.tif *.tiff);; JPEG (*.jpg *.jpeg)"));
    QString sourceFilePath = QFileDialog::getOpenFileName(this, tr("Open"), m_lastSourceDirectory, filter);
    if (!sourceFilePath.isNull()) {
        m_sourceFilePath = sourceFilePath;
        m_lastSourceDirectory = QFileInfo(m_sourceFilePath).absolutePath();
        openImageFromSourceFilePath();
    }
}

void MainWindow::saveImage()
{
    QElapsedTimer elapsedTimer;

    if (!m_sourceImage.isNull()) {
        QString filter("Guetzli (*.jpg)");
        QString filePath;
        if (m_targetFilePath.isEmpty()) {
            filePath = m_lastSourceDirectory + "/" + suggestTargetFileName();
        } else {
            filePath = QFileInfo(m_targetFilePath).absolutePath() + "/" + suggestTargetFileName();
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
            bool success = imageWriter.write(m_sourceImage);
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

void MainWindow::aboutQt() const
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

void MainWindow::updateImagePreview()
{
    if (!m_sourceImage.isNull()) {
        if (m_sourceImage.width() > ui->imagePreviewLabel->maximumWidth() ||
            m_sourceImage.height() > ui->imagePreviewLabel->maximumHeight()
           )
        {
            if (m_sourceImage.width() > m_sourceImage.height()) {
                m_previewImage = m_sourceImage.scaledToWidth(ui->imagePreviewLabel->maximumWidth(), Qt::TransformationMode::SmoothTransformation);
            } else {
                m_previewImage = m_sourceImage.scaledToHeight(ui->imagePreviewLabel->maximumHeight(), Qt::TransformationMode::SmoothTransformation);
            }
        } else {
            m_previewImage = m_sourceImage;
        }
        updateBlendPreview(ui->previewBlendingAlphaCheckBox->isChecked());
    }
}

void MainWindow::updateBlendPreview(bool blend)
{
    QImage blendPreviewImage;
    if (m_previewImage.hasAlphaChannel()) {
        if (blend) {
            blendPreviewImage = QImage(m_previewImage.size(), QImage::Format::Format_ARGB32_Premultiplied);

#if !defined(GUETZLI_BLEND_MODE) || (GUETZLI_BLEND_MODE == 1)
            blendPreviewImage.fill(Qt::black);
            QPainter painter(&blendPreviewImage);
            painter.drawImage(0, 0, m_previewImage);
            painter.end();
#elif (GUETZLI_BLEND_MODE == 2)
            blendPreviewImage.fill(Qt::white);
            QPainter painter(&blendPreviewImage);
            painter.drawImage(0, 0, m_previewImage);
            painter.end();
#elif (GUETZLI_BLEND_MODE == 3)
            const uchar *sp = m_previewImage.bits();
            uchar *dp = blendPreviewImage.bits();
            for (int i = 0; i < m_previewImage.byteCount() / 4; ++i) {
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
            blendPreviewImage = createCheckeredBackgroundImage(m_previewImage.size());
            QPainter painter(&blendPreviewImage);
            painter.drawImage(0, 0, m_previewImage);
            painter.end();
        }
    } else {
        blendPreviewImage = m_previewImage;
    }
    ui->imagePreviewLabel->setPixmap(QPixmap::fromImage(blendPreviewImage));
}
