#ifndef TRANSLATION_H
#define TRANSLATION_H

#include<QObject>
#include<QMap>
#include<QString>
#include<QStringList>



class Translate:public QObject{
    Q_OBJECT

public:
    static Translate& instance();
    bool loadTranslations(const QString &filePath);
    QString translate(const QString&key, const QString&lang);
    QString currentLanguage();
    void setCurrentLanguage(const QString&lang);
    QStringList availableLanguages();

    bool isLoaded() const { return !m_filePath.isEmpty(); }
    QString currentFilePath() const { return m_filePath; }
    ~Translate();

private:

    explicit Translate(QObject *parent = nullptr);
    QMap<QString, QMap<QString, QString>> m_translations;
    QStringList m_languages;
    QString m_currentLanguage;
    bool m_loaded;
    QString m_filePath;

};














#endif // TRANSLATION_H
