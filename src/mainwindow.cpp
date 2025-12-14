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

    QProgressBar *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);

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
        // Reset progress bar
        progressBar->setValue(0);

        // Check if ffmpeg is installed
        QProcess checkFFmpeg;
        checkFFmpeg.start("ffmpeg", {"-version"});

        if (!checkFFmpeg.waitForFinished(2000)) {
            QMessageBox::warning(this, "Error", "FFmpeg is not installed. Please add it to the path.");
            return;
        }

        if (checkFFmpeg.exitCode() != 0) {
            QMessageBox::warning(this, "Error", "FFmpeg returned an error. Please make sure its installed.");
            return;
        }
        // Trims URL
        QString url = urlInput->text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a YouTube URL.");
            return;
        }

        // Creates process
        QProcess *process = new QProcess(this);
        process->setEnvironment(QProcess::systemEnvironment());

        QString program = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
        QStringList arguments;

        // Adds arguments
        arguments << "-o" << downloadLocation + "/%(title)s.%(ext)s";
        arguments << "--progress-template" << "%(progress._percent_str)s";
        if (downloadType->currentText() == "Audio") {
            arguments << "-x" << "--audio-format" << "mp3";
        }
        arguments << url;

        // Configures tthe process program and arguments
        process->setProgram(program);
        process->setArguments(arguments);

        // Read download progress
        connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
            QString output = process->readAllStandardOutput();

            QRegularExpression regex(R"((\d+(\.\d+)?)%)");
            QRegularExpressionMatch match = regex.match(output);

            if (match.hasMatch()) {
                int percent = match.captured(1).toFloat();
                progressBar->setValue(percent);
            }
        });

        // Check if success or error
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus status) {
            if (exitCode == 0) {
                 QMessageBox::information(this, "Success", "Download successfully completed.");
            } else {
                QMessageBox::critical(this, "Error", "Download has failed.");
            }
            process->deleteLater();
        });

        process->start();
    });

    // Add widgets to layout
    mainLayout->addWidget(urlInput);
    mainLayout->addWidget(progressBar);

    secondaryLayout->addWidget(locationButton);
    secondaryLayout->addWidget(downloadType);
    secondaryLayout->addWidget(downloadButton);

    // Add layouts to widget
    mainLayout->addLayout(secondaryLayout);
}

MainWindow::~MainWindow() {
    delete ui;
}
