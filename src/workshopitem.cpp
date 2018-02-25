#include <QMessageBox>

#include "workshopitem.hpp"

WorkshopItem::WorkshopItem(int appID)
    : m_nPublishedFileId(0), m_nAppId(appID)
{
    SteamUGC()->CreateItem(appID, k_EWorkshopFileTypeCommunity);
}

void WorkshopItem::BeginUpload()
{
    m_handle = SteamUGC()->StartItemUpdate(m_nAppId, m_nPublishedFileId);
    SteamUGC()->SetItemTitle(m_handle, m_sMapName.toUtf8().constData());
    //... 
    SteamUGC()->SubmitItemUpdate(m_handle, "");
}

void WorkshopItem::SetMapName(const QString& name)
{
    m_sMapName = name;
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
    m_nPublishedFileId = result->m_nPublishedFileId; //store published file ID for later editing
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
