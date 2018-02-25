#pragma once

#include <QString>

#include "steam/steam_api.h"
#include "language.hpp"

static const int MOMENTUM_APPID = 669270;

class WorkshopItem
{
public:
    WorkshopItem(int appID = MOMENTUM_APPID);
    void BeginUpload();
    void SetMapName(const QString& name);

    bool ItemExists() const { return m_nPublishedFileId != 0; }
private:
    void OnWorkshopItemCreated(CreateItemResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, CreateItemResult_t> m_ItemCreatedCallback;

    void OnWorkshopItemUpdated(SubmitItemUpdateResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, SubmitItemUpdateResult_t> m_ItemUpdateCallback;

    QString m_sMapName;
    QString m_sMapDescription;
    QString m_sMapAuthor; 
    const char* m_pszPreviewImageFilePath;
    const char* m_pszContentFolder; //The absolute path to a local folder containing the content for the item.
    Language m_Langugage;

    UGCUpdateHandle_t m_handle;
    PublishedFileId_t m_nPublishedFileId;
    int m_nAppId;
};

