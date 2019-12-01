#include <QMessageBox>
#include <QtDebug>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

#include <thread>

#include "workshopitem.hpp"

WorkshopItem::WorkshopItem(int appID)
    : m_nAppId(appID)
{

    const auto call = SteamUGC()->CreateItem(appID, k_EWorkshopFileTypeCommunity);
    m_ItemCreatedCallback.Set(call, this, &WorkshopItem::OnWorkshopItemCreated);

    connect(this, &WorkshopItem::ItemUploadBegan,
        this, &WorkshopItem::OnUploadBegan);
}

void WorkshopItem::BeginUpload()
{
    //spawn a new thread so that our upload process does not block the CCallResult
    std::thread upload(&WorkshopItem::AsyncUpload, this); 
    upload.detach();
}

void WorkshopItem::UpdateUploadProgress()
{    
    SteamUGC()->GetItemUpdateProgress(m_handle, &m_BytesProcessed, &m_BytesTotal);
    emit ItemUploadStatus(m_BytesProcessed, m_BytesTotal);
}

void WorkshopItem::AsyncUpload()
{
    m_nPublishedFileId = m_promiseForFileID.get_future().get(); //block execution and wait for OnWorkshopItemCreated.
    //this is fine since AsyncUpload() is run in a seperate thread

    m_handle = SteamUGC()->StartItemUpdate(m_nAppId, m_nPublishedFileId);
    SteamUGC()->SetItemTitle(m_handle, m_sMapName.toUtf8().constData());
    SteamUGC()->SetItemDescription(m_handle, m_sMapDescription.toUtf8().constData());
    SteamUGC()->SetItemUpdateLanguage(m_handle, m_sLangugage.toUtf8().constData());
    SteamUGC()->SetItemContent(m_handle, m_sContentFolder.toUtf8().constData());
    SteamUGC()->SetItemPreview(m_handle, m_sPreviewImageFilePath.toUtf8().constData());
    SteamUGC()->SetItemTags(m_handle, m_Tags.GetTags());
    const auto call = SteamUGC()->SubmitItemUpdate(m_handle, "");
    m_ItemUpdateCallback.Set(call, this, &WorkshopItem::OnWorkshopItemUpdated);
    emit ItemUploadBegan();

    //thread exits
}

void WorkshopItem::OnUploadBegan()
{
    //create a timer that signals the upload process to any interested parties (i.e mainwindow)
    m_uploadProcess = new QTimer(this);
    connect(m_uploadProcess, &QTimer::timeout,
        this, &WorkshopItem::UpdateUploadProgress);
    m_uploadProcess->start(20); //run at 50 hz
}

//callback for when SteamUGC()->CreateItem is called
void WorkshopItem::OnWorkshopItemCreated(CreateItemResult_t* result, bool bIOFailure)
{
    if (result->m_eResult != k_EResultOK)
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr,
            "CreateItem ERROR!",
            QString("SteamUGC::CreateItem failed. Error: ") + QString::number(result->m_eResult));
        return;
    }
    m_promiseForFileID.set_value(result->m_nPublishedFileId); 
    emit WorkshopItemReady();
}

void WorkshopItem::OnWorkshopItemUpdated(SubmitItemUpdateResult_t* result, bool bIOFailure)
{
    m_uploadProcess->stop();

    if (result->m_eResult != k_EResultOK)
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr,
            "SubmitItemUpdate ERROR!",
            QString("SteamUGC::SubmitItemUpdate failed. Error: ") + QString::number(result->m_eResult));

        return;
    }
    if (result->m_bUserNeedsToAcceptWorkshopLegalAgreement)
    {
        QDesktopServices::openUrl(QUrl("https://steamcommunity.com/sharedfiles/workshoplegalagreement"));
    }
    QDesktopServices::openUrl(QUrl(QString("steam://url/CommunityFilePage/") + QString::number(m_nPublishedFileId)));
    emit ItemUploadCompleted();
}

void WorkshopItem::SetMapName(const QString& name)
{
    m_sMapName = name;
}

void WorkshopItem::SetMapDescription(const QString& text)
{
    m_sMapDescription = text;
}

void WorkshopItem::SetUpdateLanguage(Language lang)
{
    m_sLangugage = lang.first;
}

void WorkshopItem::SetContent(const QString& path)
{
    m_sContentFolder = path;
}

void WorkshopItem::SetPreviewImage(const QString& path)
{
    m_sPreviewImageFilePath = path;
}

void WorkshopItem::SetTags()
{
    //set uploader's steam ID

    const QString sid = QString::number(SteamUser()->GetSteamID().ConvertToUint64());
    m_Tags.AddNewTag(sid);
    //set gamemode

}
