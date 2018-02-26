#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QStatusBar>
#include <QLineEdit>
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
    void OnItemReady();
private:
    QPushButton* m_btnUpload;
    QLineEdit* m_lnItemTitle, *m_lnItemDescription;
    QStatusBar* m_statusBar;
    Languages m_languages;

    FileSelector* m_Selector;

    std::unique_ptr<WorkshopItem> m_currentItem; //only support one item at this time

    void SetupUI();
};
