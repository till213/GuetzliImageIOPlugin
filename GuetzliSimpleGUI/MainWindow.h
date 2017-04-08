#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QByteArray>

class QImage;
class QWidget;

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
    QString m_lastSourceDirectory;
    QString m_sourceFilePath;
    QByteArray m_sourceFormat;
    QString m_targetFilePath;
    QImage *m_image;
    qint64 m_elapsed;

    void updateUi();
    QString suggestTargetFileName();

private  slots:
    void openImage();
    void saveImage();
};

#endif // MAINWINDOW_H
