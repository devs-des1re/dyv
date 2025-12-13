#include "mainwindow.h"
#include <QApplication>
#include <QStyleHints>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QIcon>

void loadStylesheet(const QString &filename) {
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
        file.close();
    }
}

void applyTheme(Qt::ColorScheme scheme) {
    if (scheme == Qt::ColorScheme::Dark) {
        loadStylesheet(":/styles/dark.qss");
    } else {
        loadStylesheet(":/styles/light.qss");
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;

    // Confgure window
    const int WIDTH = 325;
    const int HEIGHT = 125;

    window.setWindowTitle("dyv");
    window.setWindowIcon(QIcon(":/icons/icon.ico"));
    window.setGeometry(0, 0, WIDTH, HEIGHT);
    window.setFixedSize(WIDTH, HEIGHT);

    // Setup settings
    QSettings settings("dyv", "Settings");

    int x = settings.value("X", 0).toInt();
    int y = settings.value("Y", 0).toInt();
    window.move(x, y);

    // Apply stylesheet
    applyTheme(QGuiApplication::styleHints()->colorScheme());
    QObject::connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
        &a, [](Qt::ColorScheme scheme) {
        applyTheme(scheme);
    });

    window.show();

    // Save configuration on exit
    int result = a.exec();
    settings.setValue("X", window.x());
    settings.setValue("Y", window.y());
    settings.setValue("Download Type", window.getDownloadType()->currentText());

    return result;
}
