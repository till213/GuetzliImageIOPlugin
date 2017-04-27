#ifndef PLUGININFODIALOG_H
#define PLUGININFODIALOG_H

#include <QDialog>

class QString;
class QByteArray;
class QTableWidgetItem;

namespace Ui {
class PluginInfoDialog;
}

class PluginInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginInfoDialog(QWidget *parent = 0);
    virtual ~PluginInfoDialog();

private:
    static const QString &ReadText;
    static const QString &WriteText;
    static const QString &ReadWriteText;
    static const QString &YesText;

    Ui::PluginInfoDialog *ui;

    void initUi();
    void initPluginDetails();
    int findFormat(const QByteArray &format);
    QTableWidgetItem *createCheckedReadOnlyItem(const QString &text = tr("Yes"));
};

#endif // PLUGININFODIALOG_H
