#pragma once

#include <map>
#include <QString>
#include <QComboBox>
#include <QTextCodec> 

typedef std::pair<const char*, QString> Language;

class Languages
{
public:

    Languages()
        : m_cbLanguages(new QComboBox)
    {
        this->m_cbLanguages->insertItems(0, GetAllLanguages());
    }
    std::vector<Language> m_languages
    {
        { "arabic", u8"العربية"},
        { "bulgarian", u8"български език"},
        { "schinese", u8"简体中文"},
        { "tchinese", u8"繁體中文"},
        { "czech", u8"čeština"},
        { "danish", u8"Dansk"},
        { "dutch" , u8"Nederlands"},
        { "english", u8"English" },
        { "finnish", u8"Suomi"},
        { "french", u8"Français" },
        { "german", u8"Deutsch" },
        { "greek", u8"Ελληνικά" },
        { "hungarian", u8"Magyar"},
        { "italian", u8"Italiano"},
        { "japanese", u8"日本語" },
        { "koreana", u8"한국어" },
        { "norwegian", u8"Norsk" },
        { "polish", u8"Polski" },
        { "portuguese", u8"Português"},
        { "brazilian", u8"Português - Brasil"},
        { "romanian", u8"Română"},
        { "russian", u8"Русский"},
        { "spanish", u8"Español"},
        { "swedish", u8"Svenska"},
        { "thai", u8"ไทย"},
        { "turkish", u8"Türkçe"},
        { "ukrainian", u8"Українська"}
    };

    QStringList GetAllLanguages()
    {
        QStringList list;
        for(auto& lang : m_languages)
        {
            const QString name(lang.first);
            list << QString(name + " - " + lang.second);
        }
        return list;
    }
    const char* GetLanguageAPICode(int index)
    {
        return m_languages[index].first;
    }
    QString GetLanguageNativeName(int index)
    {
        return m_languages[index].second;
    }
    QComboBox* Languages::GetLanguageComboBox() const
    {
        return m_cbLanguages;
    }
private:
    QComboBox * m_cbLanguages = nullptr;
};

