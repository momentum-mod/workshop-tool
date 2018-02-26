#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QTimer>
#include <QFileDialog>
#include <QtDebug>

#include "steam/steam_api.h"

#include "mainwindow.hpp"

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
    //create a new timer object that runs in the background
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, 
        SteamAPI_RunCallbacks); //run steamapi callbacks
    timer->start(10); //run at 100 hz
}
void MainWindow::OnUploadButtonClicked()
{
    m_currentItem = std::make_unique<WorkshopItem>();
    connect(m_currentItem.get(), &WorkshopItem::WorkshopItemReady,
        this, &MainWindow::OnItemReady);

    m_currentItem->SetMapName(m_lnItemTitle->text());
    m_currentItem->SetMapDescription(m_lnItemDescription->text());
    m_currentItem->SetUpdateLanguage(m_languages.GetCurrentLanguage());
    m_currentItem->SetContent(m_Selector->GetAbsolutePathToContent());

    m_currentItem->BeginUpload();
    m_statusBar->showMessage("Beginning upload...");
}

void MainWindow::OnItemReady()
{
    m_statusBar->showMessage("Workshop Item Ready!", 1000);
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
    m_Selector = new FileSelector;
    layout->addRow("Map Title", m_lnItemTitle);
    layout->addRow("Map Description", m_lnItemDescription);
    layout->addRow("Language", m_languages.GetLanguageComboBox());
    layout->addWidget(m_Selector);

    layout->addWidget(m_btnUpload);

    auto frame = new QFrame;
    frame->setLayout(layout);

    m_statusBar = new QStatusBar;
    setStatusBar(m_statusBar);
    m_statusBar->showMessage("Ready.");
    setCentralWidget(frame);
}
