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
#include <QProgressBar>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QString>
#include <QComboBox>
#include <QInputDialog>
#include "header/xlsxdocument.h"
#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLayout>
#include <QSettings>

class Cuaso : public QWidget {
    Q_OBJECT
public:
    explicit Cuaso(QWidget *parent = nullptr);
    ~Cuaso();

    void updateUiTranslations();
    void loadAvailableTranslations();
    QPushButton *m_nutbam, *m_nutbam1, *m_nutbamss, *sosanh, *m_exit, *m_export;
    QTableWidget *m_tableWidget;
    QTabWidget *m_tab;
    QComboBox *languageCombo;
    QWidget *first, *second, *third;
    QString filepath1, filepath2, filepath3;
    QLineEdit *searchline;
    QProgressBar *load;
    QTranslator *m_translator;
private slots:
    void loadFile();
    void loadFile3();
    void compare();
    void filterTable(const QString &text);
    void saveLastSession();
    void loadLastSession();
    void exportComparisonResult();
private:
    void compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2);
    QTableWidget* createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol);
    void clearLayout(QLayout *layout);
};

#endif // CUASO_H
