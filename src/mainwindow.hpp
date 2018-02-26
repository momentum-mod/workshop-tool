#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QStatusBar>
#include <QLineEdit>
#include <QProgressBar>

#include <memory>

#include "language.hpp"
#include "workshopitem.hpp"
#include "fileselector.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

 private slots:
    void OnUploadButtonClicked();
    void OnAddFilesButtonClicked();

    void OnItemReady();
    void OnItemUploadBegan();
    void OnItemStatusUpdate(uint64 pBytesProcessed, uint64 pBytesTotal);
    void OnItemUploadCompleted();
private:
    QPushButton* m_btnUpload, *m_btnAddFiles;
    QLineEdit* m_lnItemTitle, *m_lnItemDescription;
    QStatusBar* m_statusBar;
    QProgressBar* m_progressBar;
    Languages m_languages;

    FileSelector* m_Selector;

    std::unique_ptr<WorkshopItem> m_currentItem; //only support one item at this time

    void SetupUI();
};
