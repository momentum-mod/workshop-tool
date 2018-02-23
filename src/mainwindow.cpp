#include <QLabel>
#include <QMessageBox>

#include <QVBoxLayout>
#include "mainwindow.hpp"
#include "steam/steam_api.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupUI();
    if (!SteamAPI_Init())
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr, "ERROR!", "Could not init Steam API");
        fatalError.setFixedSize(500, 200);
        exit(-1);
    }
}
MainWindow::~MainWindow()
{
}
void MainWindow::SetupUI()
{
    setWindowTitle(tr("Momentum Mod - Workshop Upload Tool"));
    setGeometry(100, 100, 400, 400);

    auto label = new QLabel("Hello!");
    auto layout = new QVBoxLayout;
    layout->addWidget(label);

    auto frame = new QFrame;
    frame->setLayout(layout);

    setCentralWidget(frame);
}