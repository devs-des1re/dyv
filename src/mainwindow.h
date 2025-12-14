#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QProgressBar>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPalette>
#include <QColor>

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
