#include"Translation.h"

#include"xlsxdocument.h"


Translate& Translate::instance() {
    static Translate inst;
    return inst;
}

Translate::Translate(QObject *parent)
    : QObject(parent)
{
    m_currentLanguage = "";
}

bool Translate::loadTranslations(const QString&filePath){
    QXlsx::Document doc(filePath);
    int headerRow=1;
    int firstDataRow = headerRow + 1;
    int lastRow = doc.dimension().lastRow();
    int lastCol = doc.dimension().lastColumn();


    m_languages.clear();

    for(int col=2;col<=lastCol;col++){
        QVariant headerValue=doc.read(headerRow,col);
        QString lang=headerValue.toString().trimmed();
        m_languages.append(lang);
    }
    m_translations.clear();

    for(int row=firstDataRow;row<=lastRow;row++){
        QVariant keyVar=doc.read(row,1);

        QString key=keyVar.toString().trimmed();

        for(int col=2;col<=lastCol;col++){
            if (col - 2 < m_languages.size()) {
            QString lang=m_languages.at(col-2);
                QVariant transVar=doc.read(row,col);
            QString translation = transVar.isValid() ? transVar.toString().trimmed() : "";
            m_translations[key][lang] = translation;
        }
        }
    }


    return true;

}
QString Translate::translate(const QString&key,const QString&lang){

    if(m_translations.contains(key)&&m_translations[key].contains(lang))
        return m_translations[key][lang];
    return key;

}

QString Translate::currentLanguage()  {
    return m_currentLanguage;
}

void Translate::setCurrentLanguage(const QString &lang) {
    m_currentLanguage = lang;
}

QStringList Translate::availableLanguages(){
    return m_languages;
}

