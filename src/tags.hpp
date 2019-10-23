#pragma once

#include "steam_api.h"

class QString;

class WorkshopItemTagArray
{
public:
    WorkshopItemTagArray()
    {
        //allocate data for strings
        m_data.m_ppStrings = const_cast<const char**>(new char*[NUM_TAGS]);
        for (int i = 0; i < NUM_TAGS; ++i)
        {
            m_data.m_ppStrings[i] = const_cast<const char*>(new char[TAG_LENGTH]);
        }
        m_data.m_nNumStrings = 0;
    }
    ~WorkshopItemTagArray()
    {
        for (int i = 0; i < NUM_TAGS; ++i) {
            delete[] m_data.m_ppStrings[i];
        }
        delete[] m_data.m_ppStrings;
    }

    void AddNewTag(const QString& tag);
    int32 GetNumTags() const { return m_data.m_nNumStrings; }
    SteamParamStringArray_t* GetTags() { return &m_data; }
private:
    SteamParamStringArray_t m_data;

    static const int NUM_TAGS = 20;
    static const int TAG_LENGTH = 255;

};

inline void WorkshopItemTagArray::AddNewTag(const QString& tag)
{
    const int idx = m_data.m_nNumStrings;
    const char* data = tag.toUtf8().constData();

    strncpy(const_cast<char* const>(m_data.m_ppStrings[idx]), data, TAG_LENGTH);
    m_data.m_nNumStrings++;
}
