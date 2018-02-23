#include <QLabel>

#include <QVBoxLayout>
#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupUI();
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