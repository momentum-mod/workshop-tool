#pragma once
#include <QImage>
#include <QGroupBox>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <vector>


class ImageSelector : public QGroupBox
{
    Q_OBJECT
public:
    explicit ImageSelector(QWidget* parent = nullptr);
    ~ImageSelector();

    void paintEvent(QPaintEvent* event) override;
    //support drag/drop of files
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    void OpenImage(const QString& url);
    QString GetImageAbsolutePath() const;
public slots:
    void CreateImageDialog();
private:
    QString m_sFileName;
    QImage m_image;
    QGraphicsScene* m_imageScene;
    QGraphicsView* m_imageView;
};
