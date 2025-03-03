#ifndef CUASO_H
#define CUASO_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QString>
#include <QComboBox>
#include <QSqlDatabase>
#include <QInputDialog>
#include "header/xlsxdocument.h"
#include <QDialog>
#include <QLabel>
class Cuaso : public QWidget {
    Q_OBJECT
public:
    explicit Cuaso(QWidget *parent = nullptr);

    void updateUiTranslations();

    QPushButton *m_nutbam, *m_nutbam1, *m_nutbamss, *sosanh, *m_exit;
    QTableWidget *m_tableWidget;
    QTabWidget *m_tab;
    QComboBox *languageCombo;
    QWidget *first, *second, *third;
    QString filepath1, filepath2, filepath3;
    QLineEdit *searchline;

private slots:
    void loadFile();
    void loadFile2();
    void compare();
    void loadFile3();
    void filterTable(const QString &text);
private:
    void compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2);
    QTableWidget* createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol);
    void clearLayout(QLayout *layout);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};
#endif // CUASO_H
