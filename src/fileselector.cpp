#include <QMimeData>
#include <QPainter>
#include <QDropEvent>
#include <QtDebug>

#include "fileselector.hpp"
#include <QVBoxLayout>

FileSelector::FileSelector(QWidget* parent)
    : QGroupBox(parent)
{
    setMinimumHeight(200);
    setAcceptDrops(true);

    QDir dir;
    dir.mkdir(g_tempDir); //create a temp folder for uploading the files
    dir.setCurrent(g_tempDir); //cd into it

    m_fileSystem = new QFileSystemModel(this);

    m_fileSystemView = new QListView(this);
    m_fileSystemView->setModel(m_fileSystem);
    m_fileSystemView->setRootIndex(m_fileSystem->setRootPath(dir.currentPath()));
    m_fileSystemView->setVisible(false);

    auto layout = new QVBoxLayout;
    layout->addWidget(m_fileSystemView);
    setLayout(layout);
}

FileSelector::~FileSelector()
{
    QDir dir;
    dir.setCurrent(m_fileSystem->rootPath());
    if (!dir.removeRecursively())
    {
        //for some reason this fails because i think the dir is being created as readonly? idk
        //at least the files are deleted lmao
        qDebug() << "couldn't remove temp directory.. ";
    }
}

void FileSelector::paintEvent(QPaintEvent* event)
{
    QGroupBox::paintEvent(event);
    QPainter paint(this);
    paint.setPen(QPen(Qt::lightGray));

    if (m_vFileNames.empty())
    {
        paint.drawText(event->rect(), Qt::AlignCenter, tr("Drag files here."));
    }
    else
    {
        m_fileSystemView->setVisible(true);
    }
}

void FileSelector::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) 
    {
        event->acceptProposedAction();
    }
}

void FileSelector::dropEvent(QDropEvent* event)
{
    for(auto& url : event->mimeData()->urls())
    {
        AddFile(url.toLocalFile());
    }
}

void FileSelector::AddFile(const QString& url)
{
    //open the file
    QFile file(url);
    //get the filename stripped of path 
    const QString filename = QFileInfo(file.fileName()).fileName();
    //
    QString newPath = QDir::cleanPath(m_fileSystem->rootPath() + QDir::separator() + filename);
    //copy the file into the temp dir
    if (file.copy(newPath))
    {
        m_vFileNames.emplace_back(newPath);
    }
    update();
    file.close();
}

QString FileSelector::GetAbsolutePathToContent() const
{
    QDir dir(m_fileSystem->rootPath());
    return dir.absolutePath();
}

