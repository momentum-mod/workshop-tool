#include <QApplication>

#include "src/mainwindow.hpp"

#define QT_AUTO_SCREEN_SCALE_FACTOR 1

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
