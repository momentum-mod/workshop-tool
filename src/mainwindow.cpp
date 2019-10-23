#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>
#include <QTimer>
#include <QFileDialog>

#include <QtDebug>

#include "steam_api.h"

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
void MainWindow::SetupUI()
{
    setWindowIcon(QIcon(":images/logo.png"));

    setWindowTitle(tr("Momentum Mod - Workshop Upload Tool"));
    setGeometry(100, 100, 0, 0);

    m_statusBar = new QStatusBar;
    m_progressBar = new QProgressBar;
    m_statusBar->addPermanentWidget(m_progressBar);
    setStatusBar(m_statusBar);
    m_statusBar->showMessage(tr("Ready."));

    auto layout = new QFormLayout;
    m_btnUpload = new QPushButton(tr("Upload"));
    m_btnUpload->setMinimumHeight(50);
    m_btnAddFiles = new QPushButton(tr("Add files"));
    m_btnSelectImage = new QPushButton(tr("Select Preview Image"));

    m_lnItemTitle = new QLineEdit;
    m_txDescription = new QTextEdit;
    m_FileSelector = new FileSelector;
    m_ImageSelector = new ImageSelector;

    connect(m_btnUpload, &QPushButton::clicked,
        this, &MainWindow::OnUploadButtonClicked);
    connect(m_btnAddFiles, &QPushButton::clicked,
        m_FileSelector, &FileSelector::CreateFileDialog);
    connect(m_btnSelectImage, &QPushButton::clicked,
        m_ImageSelector, &ImageSelector::CreateImageDialog);

    layout->addRow(tr("Map Title"), m_lnItemTitle);
    layout->addRow(tr("Map Description"), m_txDescription);
    layout->addRow(tr("Language"), m_languages.GetLanguageComboBox());
    layout->addRow(m_btnAddFiles);
    layout->addRow(m_FileSelector);
    layout->addRow(m_btnSelectImage);
    layout->addRow(m_ImageSelector);
    //layout->addWidget(m_btnUpload);

    auto terms = new QLabel;
    terms->setText("<a>By submitting this item,</a><br> <a>you agree to the</a><br> \
        <a href=\"https://steamcommunity.com/sharedfiles/workshoplegalagreement\"> workshop terms of service.</a>");
    terms->setTextFormat(Qt::RichText);
    terms->setTextInteractionFlags(Qt::TextBrowserInteraction);
    terms->setOpenExternalLinks(true);

    //add a spacer to force the terms to the bottom
    layout->addItem(new QSpacerItem(0, 1000, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addRow(terms, m_btnUpload);

    auto frame = new QFrame;
    frame->setLayout(layout);
    setCentralWidget(frame);
}
void MainWindow::OnUploadButtonClicked()
{
    m_currentItem = std::make_unique<WorkshopItem>();
    connect(m_currentItem.get(), &WorkshopItem::WorkshopItemReady,
        this, &MainWindow::OnItemReady);
    connect(m_currentItem.get(), &WorkshopItem::ItemUploadStatus,
        this, &MainWindow::OnItemStatusUpdate);
    connect(m_currentItem.get(), &WorkshopItem::ItemUploadBegan,
        this, &MainWindow::OnItemUploadBegan);
    connect(m_currentItem.get(), &WorkshopItem::ItemUploadCompleted,
        this, &MainWindow::OnItemUploadCompleted);

    m_currentItem->SetMapName(m_lnItemTitle->text());
    m_currentItem->SetMapDescription(m_txDescription->toPlainText());
    m_currentItem->SetUpdateLanguage(m_languages.GetCurrentLanguage());
    m_currentItem->SetContent(m_FileSelector->GetAbsolutePathToContent());
    m_currentItem->SetPreviewImage(m_ImageSelector->GetImageAbsolutePath());
    m_currentItem->SetTags();
    //disable the UI elements so the user can't mess with stuff while we're uploading
    m_lnItemTitle->setEnabled(false);
    m_txDescription->setEnabled(false);
    m_languages.GetLanguageComboBox()->setEnabled(false);
    m_FileSelector->setEnabled(false);

    m_currentItem->BeginUpload();
}
void MainWindow::OnItemReady()
{
    m_statusBar->showMessage(tr("Workshop Item Ready!"));
}

void MainWindow::OnItemUploadBegan()
{
    m_statusBar->showMessage(tr("Upload began!"));
}

void MainWindow::OnItemStatusUpdate(uint64 bytesProcessed, uint64 bytesTotal)
{
    m_progressBar->setRange(0, bytesTotal);
    m_progressBar->setValue(bytesProcessed);
}

void MainWindow::OnItemUploadCompleted()
{
    m_statusBar->showMessage(tr("Upload Complete!"));
    m_progressBar->reset();
    m_progressBar->setRange(0, 1); //for some reason we have to set a non-empty range or else the progress bar will just idle
}
