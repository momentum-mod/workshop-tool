#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>

#include "steam/steam_api.h"

#include "mainwindow.hpp"
#include "workshopitem.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetupUI();
    if (!SteamAPI_Init())
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr, "ERROR!", "Could not init Steam API");
        exit(-1);
    }
}
MainWindow::~MainWindow()
{

}

void MainWindow::OnUploadButtonClicked()
{
    auto item = new WorkshopItem; // yes i knwo theres a memory leak
    item->SetMapName(m_lnItemTitle->text());
    item->BeginUpload();
}

void MainWindow::SetupUI()
{
    setWindowTitle(tr("Momentum Mod - Workshop Upload Tool"));
    setGeometry(100, 100, 400, 400);

    auto layout = new QFormLayout;
    m_btnUpload = new QPushButton(tr("Upload"));
    connect(m_btnUpload, &QPushButton::clicked, 
        this, &MainWindow::OnUploadButtonClicked);
    m_lnItemTitle = new QLineEdit;
    m_lnItemDescription = new QLineEdit;

    layout->addRow("Map Title", m_lnItemTitle);
    layout->addRow("Map Description", m_lnItemDescription);
    layout->addRow("Language", m_languages.GetLanguageComboBox());

    layout->addWidget(m_btnUpload);

    auto frame = new QFrame;
    frame->setLayout(layout);

    setCentralWidget(frame);
}
