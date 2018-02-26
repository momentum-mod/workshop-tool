#pragma once

#include <QString>

#include "steam/steam_api.h"
#include "language.hpp"

#include <future>
static const int MOMENTUM_APPID = 669270;

class WorkshopItem : public QObject
{
    Q_OBJECT
public:
    WorkshopItem(int appID = MOMENTUM_APPID);
    void BeginUpload();

    void SetMapName(const QString& name);
    void SetMapDescription(const QString& text);
    void SetUpdateLanguage(Language lang);
    void SetContent(const QString& path); //the actual map file(s)

    /*
    void SetMapPreviewImage(...);
    */
signals:
    void WorkshopItemReady();
    void ItemUploadComplete();
private:
    void AsyncUpload(); 
    void OnWorkshopItemCreated(CreateItemResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, CreateItemResult_t> m_ItemCreatedCallback;

    void OnWorkshopItemUpdated(SubmitItemUpdateResult_t* result, bool bIOFailure);
    CCallResult<WorkshopItem, SubmitItemUpdateResult_t> m_ItemUpdateCallback;

    QString m_sMapName;
    QString m_sMapDescription;
    QString m_sPreviewImageFilePath;
    QString m_sContentFolder; //The absolute path to a local folder containing the content for the item.
    const char* m_pszLangugage;

    UGCUpdateHandle_t m_handle;
    std::promise<PublishedFileId_t> m_nPublishedFileId;

    int m_nAppId;
};

