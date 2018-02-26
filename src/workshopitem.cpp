#include <QMessageBox>
#include <QtDebug>

#include <thread>

#include "workshopitem.hpp"

WorkshopItem::WorkshopItem(int appID)
    : m_nAppId(appID)
{
    const auto call = SteamUGC()->CreateItem(appID, k_EWorkshopFileTypeCommunity);
    m_ItemCreatedCallback.Set(call, this, &WorkshopItem::OnWorkshopItemCreated);
}

void WorkshopItem::BeginUpload()
{
    //spawn a new thread so that our upload process does not block the CCallResult
    std::thread upload(&WorkshopItem::AsyncUpload, this); 
    upload.detach();
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
    m_pszLangugage = lang.first;
}

void WorkshopItem::SetContent(const QString& path)
{
    m_sContentFolder = path;
}

void WorkshopItem::AsyncUpload()
{
    const auto id = m_nPublishedFileId.get_future().get(); //block execution and wait for OnWorkshopItemCreated.
    //this is fine since AsyncUpload() is run in a seperate thread

    m_handle = SteamUGC()->StartItemUpdate(m_nAppId, id);
    SteamUGC()->SetItemTitle(m_handle, m_sMapName.toUtf8().constData());
    SteamUGC()->SetItemDescription(m_handle, m_sMapDescription.toUtf8().constData());
    SteamUGC()->SetItemUpdateLanguage(m_handle, m_pszLangugage);
    SteamUGC()->SetItemContent(m_handle, m_sContentFolder.toUtf8().constData());

    const auto call = SteamUGC()->SubmitItemUpdate(m_handle, "");
    m_ItemUpdateCallback.Set(call, this, &WorkshopItem::OnWorkshopItemUpdated);
    //thread joins
}

//callback for when SteamUGC()->CreateItem is called
void WorkshopItem::OnWorkshopItemCreated(CreateItemResult_t* result, bool bIOFailure)
{
    if (result->m_eResult != k_EResultOK)
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr, 
            "CreateItem ERROR!", 
            "SteamUGC::CreateItem failed. Error: " + result->m_eResult);
        return;
    }
    m_nPublishedFileId.set_value(result->m_nPublishedFileId); //store published file ID for later editing
    emit WorkshopItemReady();
}

void WorkshopItem::OnWorkshopItemUpdated(SubmitItemUpdateResult_t* result, bool bIOFailure)
{
    if (result->m_eResult != k_EResultOK)
    {
        QMessageBox fatalError;
        fatalError.critical(nullptr,
            "SubmitItemUpdate ERROR!",
            "SteamUGC::SubmitItemUpdate failed. Error: " + result->m_eResult);
        return;
    }
}
