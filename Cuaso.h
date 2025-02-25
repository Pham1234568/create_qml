#ifndef CUASO_H
#define CUASO_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QString>
#include "header/xlsxdocument.h"

class Cuaso : public QWidget
{
    Q_OBJECT

public:
    explicit Cuaso(QWidget *parent = nullptr);

private slots:
    void loadFile();
    void loadFile2();
    void compare();
    void exportLanguageData(); // Slot cho chức năng xuất dữ liệu language

private:
    void compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2);
    QTableWidget* createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol);

    // Các widget chính
    QWidget *first;
    QWidget *second;
    QWidget *third;
    QTabWidget *m_tab;
    QTableWidget *m_tableWidget;

    // Các nút chức năng
    QPushButton *m_nutbam;
    QPushButton *m_nutbam1;
    QPushButton *sosanh;
    QPushButton *m_export;    // Nút xuất dữ liệu language
    QPushButton *m_exit;

    // Đường dẫn file Excel được nạp
    QString filepath1;
    QString filepath2;
};

#endif // CUASO_H
