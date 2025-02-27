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
    // Hàm bổ sung: kiểm tra xem file dịch đã được nạp chưa.
    bool isLoaded() const { return !m_filePath.isEmpty(); }
    QString currentFilePath() const { return m_filePath; }

private:

    explicit Translate(QObject *parent = nullptr);
    // Lưu theo dạng: key -> { language: translation, ... }
    QMap<QString, QMap<QString, QString>> m_translations;
    QStringList m_languages;
    QString m_currentLanguage;
    bool m_loaded;         // true nếu file dịch đã được nạp thành công.
    QString m_filePath;

};














#endif // TRANSLATION_H
