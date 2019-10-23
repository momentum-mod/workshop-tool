#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QStatusBar>
#include <QLineEdit>
#include <QProgressBar>
#include <QTextEdit>

#include <memory>

#include "language.hpp"
#include "workshopitem.hpp"
#include "fileselector.hpp"
#include "imageselector.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

 private slots:
    void OnUploadButtonClicked();

    void OnItemReady();
    void OnItemUploadBegan();
    void OnItemStatusUpdate(uint64 bytesProcessed, uint64 bytesTotal);
    void OnItemUploadCompleted();
private:
    void SetupUI();

    QPushButton* m_btnUpload, *m_btnAddFiles, *m_btnSelectImage;
    QLineEdit* m_lnItemTitle;
    QTextEdit* m_txDescription;
    QStatusBar* m_statusBar;
    QProgressBar* m_progressBar;
    Languages m_languages;

    FileSelector* m_FileSelector;
    ImageSelector* m_ImageSelector;
    std::unique_ptr<WorkshopItem> m_currentItem; //only support one item at this time
};
