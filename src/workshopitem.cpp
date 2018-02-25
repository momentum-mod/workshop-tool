#include <QMessageBox>
#include <QtDebug>

#include "workshopitem.hpp"

WorkshopItem::WorkshopItem(int appID)
    : m_nPublishedFileId(0), m_nAppId(appID)
{
    const auto call = SteamUGC()->CreateItem(appID, k_EWorkshopFileTypeCommunity);
    m_ItemCreatedCallback.Set(call, this, &WorkshopItem::OnWorkshopItemCreated);
}

void WorkshopItem::BeginUpload()
{
    m_handle = SteamUGC()->StartItemUpdate(m_nAppId, m_nPublishedFileId);
    SteamUGC()->SetItemTitle(m_handle, m_pszMapName);
    SteamUGC()->SetItemDescription(m_handle, m_pszMapDescription);
    SteamUGC()->SetItemUpdateLanguage(m_handle, m_pszLangugage);

    const auto call = SteamUGC()->SubmitItemUpdate(m_handle, "");
    m_ItemUpdateCallback.Set(call, this, &WorkshopItem::OnWorkshopItemUpdated);
}

void WorkshopItem::SetMapName(const QString& name)
{
    m_pszMapName = name.toUtf8().constData();
}

void WorkshopItem::SetMapDescription(const QString& text)
{
    m_pszMapDescription = text.toUtf8().constData();
}

void WorkshopItem::SetUpdateLanguage(Language lang)
{
    m_pszLangugage = lang.first;
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
