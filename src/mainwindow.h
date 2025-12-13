#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPalette>
#include <QColor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QProcess>
#include <QMessageBox>
#include <QProgressBar>

QT_BEGIN_NAMESPACE namespace Ui {
    class MainWindow;
} QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        QComboBox* getDownloadType() const { return downloadType; }

    private:
        Ui::MainWindow *ui;
        QLineEdit *urlInput;
        QString downloadLocation;
        QComboBox *downloadType;
        QProgressBar *progressBar;
};

#endif
