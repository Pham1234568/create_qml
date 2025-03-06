#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QStringList>
#include <QTranslator>
#include <QApplication>

class Translate : public QObject {
    Q_OBJECT
public:
    static Translate& instance();
    // Load file QM sử dụng QTranslator
    bool loadTranslations(const QString &filePath);
    // Hàm dịch: sẽ sử dụng QObject::tr() để trả về văn bản đã dịch
    QString translate(const QString &key) const;
    QString currentLanguage() const;
    void setCurrentLanguage(const QString &lang);
    QStringList availableLanguages() const; // Không dùng nữa (danh sách được lấy từ combobox)

private:
    explicit Translate(QObject *parent = nullptr);
    ~Translate();

    // Không cho phép copy
    Translate(const Translate &) = delete;
    Translate &operator=(const Translate &) = delete;

    QString m_currentLanguage;
    QTranslator m_qtTranslator;
};

#endif // TRANSLATION_H
