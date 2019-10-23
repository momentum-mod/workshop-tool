#include <QMimeData>
#include <QPainter>
#include <QDropEvent>
#include <QtDebug>
#include <QShortcut>
#include <QVBoxLayout>
#include <QFileDialog>

#ifdef _WIN32
#include <Windows.h>
#include <fileapi.h>
#endif

#include "fileselector.hpp"

FileSelector::FileSelector(QWidget* parent)
    : QGroupBox(parent)
{
    setMinimumHeight(100);
    setAcceptDrops(true);

    QDir dir;
    dir.mkdir(g_tempDir); //create a temp folder for uploading the files
    dir.setCurrent(g_tempDir); //cd into it

#ifdef _WIN32 //set folder to hidden
    const char* p = dir.currentPath().toUtf8().constData();
    const int attr = GetFileAttributes(p);
    if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
        SetFileAttributes(p, attr | FILE_ATTRIBUTE_HIDDEN);
    }
#endif
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
        //for some reason this fails because the dir is still mounted by the filesystem or something
        //at least the files are deleted!
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
        paint.drawText(event->rect(), Qt::AlignCenter, tr("Drag map file here."));
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
    const QString ext = QFileInfo(file.fileName()).suffix();

    //depending on the file type, we process each file differently.
    //for maps we make a new dir called 'maps' and copy the map into it.
    //this is required because the way the game mounts workshop content is that each
    //workshop item is a root "GAME" directory, the game then searches for maps inside the 'maps' folder
    QString path;
    if (ext == "bsp")
    {
        ProcessBSP(file);
    }
    else
    {
        //for now, only allow people to add bsp files.
        return;
    }
    file.close();
    update();
}

//copies the bsp file into maps subdir
bool FileSelector::ProcessBSP(QFile& file)
{
    //get the filename stripped of path 
    const QString filename = QFileInfo(file.fileName()).fileName();
    const QString ext = QFileInfo(file.fileName()).suffix();

    //make a new dir called "maps" inside the temp dir
    QDir dir(m_fileSystem->rootPath());
    dir.mkdir("maps");
    dir.setCurrent("maps");

    m_fileSystemView->setRootIndex(m_fileSystem->setRootPath(dir.currentPath())); //temp, set root path to inside the 'maps' folder

    const QString path = dir.cleanPath(dir.currentPath() + QDir::separator() + filename);
    //copy the file into the temp dir
    if (file.copy(path))
    {
        m_vFileNames.emplace_back(path);
        return true;
    }
    return false;
}

QString FileSelector::GetAbsolutePathToContent() const
{
    QDir dir(m_fileSystem->rootPath());
    return dir.absolutePath();
}

void FileSelector::CreateFileDialog()
{
    QFileDialog dialog(this->parentWidget());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("Momentum Mod Maps (*.bsp)"));

    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
    }
    for (const auto& file : fileNames)
    {
        AddFile(file);
    }
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

