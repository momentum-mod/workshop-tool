#pragma once

#include <QString>

#include "steam/steam_api.h"
#include "language.hpp"

static const int MOMENTUM_APPID = 669270;

class WorkshopItem : public QObject
{
public:
    WorkshopItem(int appID = MOMENTUM_APPID);
    void BeginUpload();

    void SetMapName(const QString& name);
    void SetMapDescription(const QString& text);
    void SetUpdateLanguage(Language lang);

    /*
    void SetMapPreviewImage(...);
    void SetContent(); //the actual map file(s)
    */
    bool ItemExists() const { return m_nPublishedFileId != 0; }

private:
    void OnWorkshopItemCreated(CreateItemResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, CreateItemResult_t> m_ItemCreatedCallback;

    void OnWorkshopItemUpdated(SubmitItemUpdateResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, SubmitItemUpdateResult_t> m_ItemUpdateCallback;

    const char* m_pszMapName;
    const char* m_pszMapDescription;
    const char* m_pszPreviewImageFilePath;
    const char* m_pszContentFolder; //The absolute path to a local folder containing the content for the item.
    const char* m_pszLangugage;

    UGCUpdateHandle_t m_handle;
    PublishedFileId_t m_nPublishedFileId;
    int m_nAppId;
};

