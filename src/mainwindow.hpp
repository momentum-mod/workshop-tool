#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

#include "language.hpp"

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

    void SetupUI();
};
