#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <memory>

#include "language.hpp"
#include "workshopitem.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

 private slots:
    void OnUploadButtonClicked();
private:
    QPushButton* m_btnUpload;
    QLineEdit* m_lnItemTitle, *m_lnItemDescription;

    QComboBox* m_cbLanguage;
    Languages m_languages;

    std::unique_ptr<WorkshopItem> m_currentItem; //only support one item at this time

    void SetupUI();
};
