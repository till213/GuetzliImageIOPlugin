#include <QImageWriter>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>


#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_image = new QImage();
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
}

// Private Slots

void MainWindow::openImage()
{
    qDebug("Open Action");
    m_filePath = QFileDialog::getOpenFileName(this, tr("Open"), m_filePath);
    this->setWindowTitle(m_filePath);
    if (!m_filePath.isNull()) {

        bool success = m_image->load(m_filePath);
        if (success) {
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
        }
    }
    updateUi();
}

void MainWindow::saveImage()
{
    if (!m_image->isNull()) {
        QString saveFilePath = QFileDialog::getSaveFileName(this, tr("Save"), m_filePath);

        QImageWriter imageWriter;
        imageWriter.setFileName(saveFilePath);
        imageWriter.setFormat("guetzli");
        imageWriter.setQuality(85);
        imageWriter.write(*m_image);
    }
}

