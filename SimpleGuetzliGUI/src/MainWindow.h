#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QColor>

class QWidget;
class PluginInfoDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    Ui::MainWindow *ui;
    PluginInfoDialog *m_pluginInfoDialog;
    QString m_lastSourceDirectory;
    QString m_sourceFilePath;
    QByteArray m_sourceFormat;
    QString m_targetFilePath;
    QImage m_image;
    qint64 m_elapsed;

    QString suggestTargetFileName();
    void updateUi();
    bool hasGuetzliPlugin();

private  slots:
    // File menu
    void openImage();
    void saveImage();

    // Help menu
    void aboutQt();
    void showPluginInfo();
};

#endif // MAINWINDOW_H
