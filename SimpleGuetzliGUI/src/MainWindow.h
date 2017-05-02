#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QColor>

class QWidget;
class PluginInfoDialog;
class QDragEnterEvent;
class QDragLeaveEvent;
class QSize;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

protected:

    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void dropEvent(QDropEvent *event);

private:
    Ui::MainWindow *ui;
    PluginInfoDialog *m_pluginInfoDialog;
    QString m_lastSourceDirectory;
    QString m_sourceFilePath;
    QByteArray m_sourceFormat;
    QString m_targetFilePath;
    QImage m_sourceImage;
    QImage m_previewImage;
    qint64 m_elapsed;

    static const QString &BlackText;
    static const QString &WhiteText;
    static const QString &IgnoreAlphaText;

    QString suggestTargetFileName() const;
    void updateUi();
    bool hasGuetzliPlugin() const;
    void openImageFromSourceFilePath();
    static QImage createCheckeredBackground(const QSize &size);

private slots:
    // File menu
    void openImage();
    void saveImage();

    // Help menu
    void aboutQt() const;
    void showPluginInfo();

    void updateImagePreview();
    void updateBlendPreview(bool blend);
};

#endif // MAINWINDOW_H
