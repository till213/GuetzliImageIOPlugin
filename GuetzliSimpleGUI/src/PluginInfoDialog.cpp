#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QImageReader>
#include <QImageWriter>
#include <QDir>

#include "PluginInfoDialog.h"
#include "ui_PluginInfoDialog.h"

// Public

PluginInfoDialog::PluginInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginInfoDialog)
{
    ui->setupUi(this);
    initUi();
}

PluginInfoDialog::~PluginInfoDialog()
{
    delete ui;
}

// Private

void PluginInfoDialog::initUi()
{
    constexpr int FormatColumn = 0;
    constexpr int CanReadColumn = 1;
    constexpr int CanWriteColumn = 2;

    // Readers
    QList<QByteArray> supportedImageFormats = QImageReader::supportedImageFormats();
    for (QByteArray supportedImageFormat : supportedImageFormats) {
        QTableWidgetItem *formatItem = new QTableWidgetItem(QString(supportedImageFormat).toUpper());
        formatItem->setFlags(formatItem->flags()^= Qt::ItemIsEditable);
        int currentRowCount = ui->pluginTableWidget->rowCount();
        ui->pluginTableWidget->insertRow(currentRowCount);
        ui->pluginTableWidget->setItem(currentRowCount, FormatColumn, formatItem);

        QTableWidgetItem *canReadItem = new QTableWidgetItem(tr("Yes"));
        canReadItem->setCheckState(Qt::CheckState::Checked);
        canReadItem->setFlags(canReadItem->flags()^= Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
        ui->pluginTableWidget->setItem(currentRowCount, CanReadColumn, canReadItem);
    }

    // Writers
    supportedImageFormats = QImageWriter::supportedImageFormats();
    for (QByteArray supportedImageFormat : supportedImageFormats) {

        QTableWidgetItem *canWriteItem = new QTableWidgetItem(tr("Yes"));
        canWriteItem->setCheckState(Qt::CheckState::Checked);
        canWriteItem->setFlags(canWriteItem->flags()^= Qt::ItemIsEditable | Qt::ItemIsUserCheckable);

        int row = findFormat(supportedImageFormat);
        if (row >= 0) {
           ui->pluginTableWidget->setItem(row, CanWriteColumn, canWriteItem);
        } else {
            QTableWidgetItem *formatItem = new QTableWidgetItem(QString(supportedImageFormat).toUpper());
            formatItem->setFlags(formatItem->flags() ^= Qt::ItemIsEditable);
            int currentRowCount = ui->pluginTableWidget->rowCount();
            ui->pluginTableWidget->insertRow(currentRowCount);
            ui->pluginTableWidget->setItem(currentRowCount, FormatColumn, formatItem);
            ui->pluginTableWidget->setItem(currentRowCount, CanWriteColumn, canWriteItem);
        }

    }
    ui->pluginTableWidget->setSortingEnabled(true);

    // Library paths
    QStringList libraryPaths = QCoreApplication::libraryPaths();
    for (QString libraryPath : libraryPaths) {
        ui->pluginPathListWidget->addItem(QDir::toNativeSeparators(libraryPath));
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
