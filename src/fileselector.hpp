#pragma once
#include <QGroupBox>
#include <QString>
#include <QFileSystemModel>
#include <QListView>

#include <vector>

static const QString g_tempDir = ".temp";

class FileSelector : public QGroupBox
{
    Q_OBJECT
public:
    explicit FileSelector(QWidget* parent = nullptr);
    ~FileSelector();

    void paintEvent(QPaintEvent* event) override;
    //support drag/drop of files
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    void AddFile(const QString& url);
    bool ProcessBSP(QFile& file);

    QString GetAbsolutePathToContent() const;
public slots:
    void DeleteSelectedFile();
    void CreateFileDialog();
private:
    std::vector<QString> m_vFileNames;
    QFileSystemModel* m_fileSystem;
    QListView* m_fileSystemView;
};
