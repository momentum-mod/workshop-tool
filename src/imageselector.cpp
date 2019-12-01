#include <QMimeData>
#include <QPainter>
#include <QDropEvent>
#include <QImageReader>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDir>

#include <QtDebug>

#include "imageselector.hpp"
#include <QFileDialog>


ImageSelector::ImageSelector(QWidget* parent)
    : QGroupBox(parent)
{
    setMinimumHeight(200);
    setAcceptDrops(true);

    m_imageView = new QGraphicsView(this);
    m_imageScene = new QGraphicsScene(this);
    m_imageView->setScene(m_imageScene);
    m_imageView->setVisible(false);

    auto layout = new QVBoxLayout;
    layout->addWidget(m_imageView);
    setLayout(layout);
}

ImageSelector::~ImageSelector()
{

}

void ImageSelector::paintEvent(QPaintEvent* event)
{
    QGroupBox::paintEvent(event);
    QPainter paint(this);
    paint.setPen(QPen(Qt::lightGray));

    if (m_sFileName.isEmpty())
    {
        paint.drawText(event->rect(), Qt::AlignCenter, tr("Drag preview image here."));
    }
    else
    {
        m_imageView->setVisible(true);

        m_imageScene->addPixmap(QPixmap::fromImage(m_image));
    }
}

void ImageSelector::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void ImageSelector::dropEvent(QDropEvent* event)
{
    //only open the first image
    OpenImage(event->mimeData()->urls().first().toLocalFile());
}

void ImageSelector::OpenImage(const QString& url)
{
    QImageReader reader(url);
    reader.setAutoTransform(true);
    m_image = reader.read();
    if (m_image.isNull())
    {
        QMessageBox::warning(this, 
            "Cannot load image",
            tr("Cannot load %1: %2").arg(QDir::toNativeSeparators(url), reader.errorString()));
        return;
    }
    m_sFileName = url;

    update();
}

QString ImageSelector::GetImageAbsolutePath() const
{
    QDir dir(m_sFileName);
    return dir.absolutePath();
}

void ImageSelector::CreateImageDialog()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("Images (*.png *.gif *.jpg *.jpeg)"));

    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
    }
    for (const auto& file : fileNames)
    {
        OpenImage(file);
    }

}

