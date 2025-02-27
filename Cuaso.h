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

class Cuaso : public QWidget {
    Q_OBJECT
public:
    explicit Cuaso(QWidget *parent = nullptr);

    void updateUiTranslations();

    QComboBox *languageCombo;
    QWidget *first;
    QWidget *second;
    QWidget *third;
    QTabWidget *m_tab;
    QTableWidget *m_tableWidget;
    QPushButton *m_nutbam;
    QPushButton *m_nutbam1;
    QPushButton *sosanh;
    QPushButton *m_exit;
    QString filepath1;
    QString filepath2;

private slots:
    void loadFile();
    void loadFile2();
    void compare();

private:
    void compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2);
    QTableWidget* createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol);
    void clearLayout(QLayout *layout);
};

#endif // CUASO_H
