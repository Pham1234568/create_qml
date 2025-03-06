#include "Translation.h"

Translate& Translate::instance() {
    static Translate inst;
    return inst;
}

Translate::Translate(QObject *parent)
    : QObject(parent),
    m_currentLanguage("")
{
}

bool Translate::loadTranslations(const QString &filePath) {
    // Loại bỏ translator cũ (nếu có)
    qApp->removeTranslator(&m_qtTranslator);
    if (m_qtTranslator.load(filePath)) {
        qApp->installTranslator(&m_qtTranslator);
        return true;
    }
    return false;
}

QString Translate::translate(const QString &key) const {
    // Sử dụng QObject::tr() để trả về văn bản đã dịch nếu file QM được load thành công
    return QObject::tr(qPrintable(key));
}

QString Translate::currentLanguage() const {
    return m_currentLanguage;
}

void Translate::setCurrentLanguage(const QString &lang) {
    m_currentLanguage = lang;
}

QStringList Translate::availableLanguages() const {
    // Không cần dùng nữa, danh sách file QM được load qua combo_box
    return QStringList();
}

Translate::~Translate() {
    // Destructor
}
