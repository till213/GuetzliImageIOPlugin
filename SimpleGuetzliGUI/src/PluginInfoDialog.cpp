#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QImageReader>
#include <QImageWriter>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#include "PluginInfoDialog.h"
#include "ui_PluginInfoDialog.h"

namespace
{
    constexpr int FormatColumn = 0;
    constexpr int CanReadColumn = 1;
    constexpr int CanWriteColumn = 2;
    constexpr int SizeColumn = 3;
    constexpr int QualityColumn = 4;
    constexpr int NameColumn = 5;
    constexpr int DescriptionColumn = 6;
} // Anonymous

// Public

PluginInfoDialog::PluginInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginInfoDialog)
{
    ui->setupUi(this);
    initUi();
    updateUi();
}

PluginInfoDialog::~PluginInfoDialog()
{
    delete ui;
}

// Private

const QString &PluginInfoDialog::ReadText = tr("Read");
const QString &PluginInfoDialog::WriteText = tr("Write");
const QString &PluginInfoDialog::ReadWriteText = tr("Read/Write");
const QString &PluginInfoDialog::YesText = tr("Yes");

void PluginInfoDialog::initUi()
{
    QTableWidgetItem *item;

    // Readers
    QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
    for (QByteArray supportedImageFormat : supportedImageFormats) {
        item = new QTableWidgetItem(QString(supportedImageFormat).toUpper());
        item->setFlags(item->flags()^= Qt::ItemIsEditable);
        int currentRowCount = ui->pluginTableWidget->rowCount();
        ui->pluginTableWidget->insertRow(currentRowCount);
        ui->pluginTableWidget->setItem(currentRowCount, ::FormatColumn, item);

        item = createCheckedReadOnlyItem();
        ui->pluginTableWidget->setItem(currentRowCount, ::CanReadColumn, item);
    }

    // Writers
    supportedImageFormats = QImageWriter::supportedImageFormats();
    for (QByteArray supportedImageFormat : supportedImageFormats) {
        int row = findFormat(supportedImageFormat);
        if (row >= 0) {
            item = createCheckedReadOnlyItem();
            ui->pluginTableWidget->setItem(row, CanWriteColumn, item);
        } else {
            item = new QTableWidgetItem(QString(supportedImageFormat).toUpper());
            item->setFlags(item->flags() ^= Qt::ItemIsEditable);
            int currentRowCount = ui->pluginTableWidget->rowCount();
            ui->pluginTableWidget->insertRow(currentRowCount);
            ui->pluginTableWidget->setItem(currentRowCount, ::FormatColumn, item);
            item = createCheckedReadOnlyItem();
            ui->pluginTableWidget->setItem(currentRowCount, ::CanWriteColumn, item);
        }
    }

    initPluginDetails();
    ui->pluginTableWidget->setSortingEnabled(true);

    // Library paths
    QStringList libraryPaths = QCoreApplication::libraryPaths();
    for (QString libraryPath : libraryPaths) {
        ui->pluginPathListWidget->addItem(QDir::toNativeSeparators(libraryPath));
    }
}

void PluginInfoDialog::initPluginDetails()
{
    QTableWidgetItem *item;

    // Plugin details
    for (int row = 0; row < ui->pluginTableWidget->rowCount(); ++row) {
        // Read properties
        item = ui->pluginTableWidget->item(row, ::CanReadColumn);
        if (item != nullptr) {
            QImageReader imageReader;
            imageReader.setFormat(item->text().toLocal8Bit());
            // Size
            if (imageReader.supportsOption(QImageIOHandler::ImageOption::Size)) {
                item = createCheckedReadOnlyItem();
                ui->pluginTableWidget->setItem(row, ::SizeColumn, item);
            }
            // Name
            if (imageReader.supportsOption(QImageIOHandler::ImageOption::Name)) {
                item = createCheckedReadOnlyItem(ReadText);
                ui->pluginTableWidget->setItem(row, ::NameColumn, item);
            }
            // Description
            if (imageReader.supportsOption(QImageIOHandler::ImageOption::Description)) {
                item = createCheckedReadOnlyItem(ReadText);
                ui->pluginTableWidget->setItem(row, ::DescriptionColumn, item);
            }
        }

        // Write properties
        item = ui->pluginTableWidget->item(row, CanWriteColumn);
        if (item != nullptr) {
            item = ui->pluginTableWidget->item(row, ::FormatColumn);
            QImageWriter imageWriter;
            imageWriter.setFormat(item->text().toLocal8Bit());
            // Quality (write)
            if (imageWriter.supportsOption(QImageIOHandler::ImageOption::Quality)) {
                item = createCheckedReadOnlyItem();
                ui->pluginTableWidget->setItem(row, ::QualityColumn, item);
            }
            // Name
            if (imageWriter.supportsOption(QImageIOHandler::ImageOption::Name)) {
                item = ui->pluginTableWidget->item(row, ::NameColumn);
                if (item != nullptr) {
                    item->setText(ReadWriteText);
                } else {
                    item = createCheckedReadOnlyItem(WriteText);
                    ui->pluginTableWidget->setItem(row, ::NameColumn, item);
                }
            }
            // Description
            if (imageWriter.supportsOption(QImageIOHandler::ImageOption::Description)) {
                item = ui->pluginTableWidget->item(row, ::DescriptionColumn);
                if (item != nullptr) {
                    item->setText(ReadWriteText);
                } else {
                    item = createCheckedReadOnlyItem(WriteText);
                    ui->pluginTableWidget->setItem(row, ::DescriptionColumn, item);
                }
            }
        }
    }
}

int PluginInfoDialog::findFormat(const QByteArray &format)
{
    int row;
    for (row = 0; row < ui->pluginTableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = ui->pluginTableWidget->item(row, 0);
        if (QString(format) == item->text().toLower()) {
            break;
        }
    }
    if (row == ui->pluginTableWidget->rowCount()) {
        row = -1;
    }
    return row;
}

QTableWidgetItem *PluginInfoDialog::createCheckedReadOnlyItem(const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setCheckState(Qt::CheckState::Checked);
    item->setFlags(item->flags()^= Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
    return item;
}

// Private slots

void PluginInfoDialog::updateUi()
{
    bool hasItems = ui->pluginPathListWidget->selectedItems().count() > 0;
    ui->showPushButton->setEnabled(hasItems);
}

void PluginInfoDialog::showPluginDirectory()
{
    QString path = ui->pluginPathListWidget->selectedItems().first()->text();
    QDesktopServices::openUrl(QUrl(QString("file:///") + QDir::fromNativeSeparators(path), QUrl::TolerantMode));
}

