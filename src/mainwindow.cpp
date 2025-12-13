#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Setup settings
    QSettings settings("dyv", "Settings");
    downloadLocation = settings.value("Download Location", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).toString();

    // Create widget
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    // Create layout
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);
    QHBoxLayout *secondaryLayout = new QHBoxLayout;

    // Remove layout margins
    centralWidget()->layout()->setContentsMargins(15, 0, 15, 0);

    // Create ui widgets
    urlInput = new QLineEdit(this);
    urlInput->setPlaceholderText("Enter Youtube URL...");

    QPushButton *locationButton = new QPushButton("Location", this);
    QPushButton *downloadButton = new QPushButton("Download", this);

    downloadType = new QComboBox(this);
    downloadType->addItem("Video");
    downloadType->addItem("Audio");
    downloadType->setCurrentText(settings.value("Download Type", "Video").toString());

    // Button Triggers
    QObject::connect(locationButton, &QPushButton::clicked, [&]() {
        QString folder = QFileDialog::getExistingDirectory(
            this,
            "Select Folder",
            downloadLocation,
            QFileDialog::ShowDirsOnly
        );

        if (!folder.isEmpty()) {
            QSettings settings("dyv", "Settings");
            settings.setValue("Download Location", folder);
            downloadLocation = folder;
        }
    });

    QObject::connect(downloadButton, &QPushButton::clicked, this, [=]() {
        QString url = urlInput->text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a YouTube URL.");
            return;
        }

        QProcess *process = new QProcess(this);
        QString program = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
        QStringList arguments;

        arguments << "-o" << downloadLocation + "/%(title)s.%(ext)s";
        if (downloadType->currentText() == "Audio") {
            arguments << "-x" << "--audio-format" << "mp3";
        }
        arguments << url;

        process->setProgram(program);
        process->setArguments(arguments);

        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus status) {
            if (exitCode == 0) {
                 QMessageBox::information(this, "Done", "Download completed.");
            } else {
                QMessageBox::critical(this, "Error", "Download failed.");
            }
            process->deleteLater();
        });

        process->start();
    });

    // Add widgets to layout
    mainLayout->addWidget(urlInput);
    secondaryLayout->addWidget(locationButton);
    secondaryLayout->addWidget(downloadType);
    secondaryLayout->addWidget(downloadButton);

    // Add layouts to widget
    mainLayout->addLayout(secondaryLayout);
}

MainWindow::~MainWindow() {
    delete ui;
}
