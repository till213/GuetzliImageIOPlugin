#ifndef PLUGININFODIALOG_H
#define PLUGININFODIALOG_H

#include <QDialog>

class QByteArray;

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
    Ui::PluginInfoDialog *ui;

    void initUi();
    int findFormat(const QByteArray &format);
};

#endif // PLUGININFODIALOG_H
