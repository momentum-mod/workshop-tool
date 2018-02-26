#include <QMimeData>
#include <QPainter>
#include <QDropEvent>
#include <QtDebug>
#include <QShortcut>

#include "fileselector.hpp"
#include <QVBoxLayout>

FileSelector::FileSelector(QWidget* parent)
    : QGroupBox(parent)
{
    setMinimumHeight(100);
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

    //pressing DEL activates the slots only when list widget has focus
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), m_fileSystemView);
    connect(shortcut, &QShortcut::activated, 
        this, &FileSelector::DeleteSelectedFile);
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

void FileSelector::DeleteSelectedFile()
{
    QModelIndex index = m_fileSystemView->currentIndex();
    const QString selected = index.data().toString();

    //theres probably a better way to do this but idk what im doing :&)
    for(auto it = m_vFileNames.begin(); it != m_vFileNames.end(); ++it)
    {
        if (it->contains(selected))
        {
            QFile f(*it);
            f.remove();
            break;
        }
    }
}

