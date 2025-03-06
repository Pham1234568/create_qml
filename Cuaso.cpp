#include "Cuaso.h"
#include "header/xlsxdocument.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>
#include <QSpacerItem>
#include <algorithm>
#include <QApplication>
#include <QSqlQuery>
#include <QSignalBlocker>
#include <QScrollBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QSettings>
#include <QTimer>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QListView>
#include <QDir>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QTranslator>

#define tr(key) QCoreApplication::translate("Default", key)

void Cuaso::clearLayout(QLayout *layout) {
    if (!layout)
        return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget())
            delete item->widget();
        delete item;
    }
}

Cuaso::Cuaso(QWidget *parent) : QWidget(parent) {
    setMinimumSize(800, 600);
    m_translator = new QTranslator(this);

    first = new QWidget();
    second = new QWidget();
    third = new QWidget();

    QString buttonStyle =
        "QPushButton {"
        "  border: 2px solid #8f8f91;"
        "  border-radius: 12px;"
        "  background-color: cyan;"
        "  min-width: 80px;"
        "}"
        "QPushButton:pressed {"
        "  background-color: white;"
        "}";

    m_nutbam  = new QPushButton(tr("Please load Excel file 1"), this);
    m_nutbam1 = new QPushButton(tr("Please load Excel file 2"), this);
    // Language file button not needed → hide it.
    m_nutbamss = new QPushButton(tr("Please load language file"), this);
    m_nutbamss->hide();

    sosanh    = new QPushButton(tr("Click to compare"), this);
    m_export= new QPushButton(tr("Export file compare"), this);
    m_exit    = new QPushButton(tr("Click to exit"), this);
    searchline = new QLineEdit(this);
    load        = new QProgressBar(this);
    load->setTextVisible(true);
    load->setMaximum(100);
    load->setMinimum(0);
    load->setValue(0);
    load->setVisible(false);

    searchline->setPlaceholderText(tr("Enter keyword to search"));

    m_nutbam->setStyleSheet(buttonStyle);
    m_nutbam1->setStyleSheet(buttonStyle);
    m_nutbamss->setStyleSheet(buttonStyle);
    sosanh->setStyleSheet(buttonStyle);
    m_export->setStyleSheet(buttonStyle);
    m_exit->setStyleSheet(buttonStyle);

    m_tableWidget = new QTableWidget();
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_tab = new QTabWidget(this);
    languageCombo = new QComboBox(this);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QString qmPath = languageCombo->itemData(index).toString();
        if (!qmPath.isEmpty()) {
            qApp->removeTranslator(m_translator);
            if (m_translator->load(qmPath)) {
                qApp->installTranslator(m_translator);
                updateUiTranslations();
            } else {
                qDebug() << "Failed to load QM file:" << qmPath;
            }
        }
    });

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(languageCombo);
    buttonLayout->addWidget(m_nutbam);
    buttonLayout->addWidget(m_nutbam1);
    buttonLayout->addWidget(m_nutbamss);
    buttonLayout->addWidget(sosanh);
    buttonLayout->addWidget(m_export);
    buttonLayout->addWidget(m_exit);
    buttonLayout->addStretch();

    QVBoxLayout *beside = new QVBoxLayout;
    beside->addWidget(searchline);
    beside->addWidget(m_tab);
    beside->addWidget(load);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(beside, 1);

    setLayout(mainLayout);

    connect(m_nutbam,  &QPushButton::clicked, this, &Cuaso::loadFile);
    connect(m_nutbam1, &QPushButton::clicked, this, &Cuaso::loadFile3);
    connect(sosanh,    &QPushButton::clicked, this, &Cuaso::compare);
    connect(m_export, &QPushButton::clicked, this, &Cuaso::exportComparisonResult);
    connect(m_exit,    &QPushButton::clicked, this, &Cuaso::close);
    connect(searchline, &QLineEdit::textChanged, this, &Cuaso::filterTable);
    setAcceptDrops(true);

    loadAvailableTranslations();

    loadLastSession();
}

void Cuaso::loadAvailableTranslations() {
    QDir qmDir("D:/Code_Qt/Code_Qt");
    QStringList filters;
    filters << "translations_*.qm";
    qmDir.setNameFilters(filters);
    QStringList qmFiles = qmDir.entryList();
    languageCombo->clear();
    for (const QString &qmFile : qmFiles) {
        QString langCode = qmFile;
        langCode.remove("translations_");
        langCode.remove(".qm");
        QString fullPath = qmDir.absoluteFilePath(qmFile);
        languageCombo->addItem(langCode, fullPath);
    }
    if (languageCombo->count() > 0) {
        languageCombo->setCurrentIndex(0);
        QString qmPath = languageCombo->itemData(0).toString();
        if (!qmPath.isEmpty()) {
            if (m_translator->load(qmPath)) {
                qApp->installTranslator(m_translator);
                updateUiTranslations();
            }
        }
    }
}

void Cuaso::filterTable(const QString &text) {
    for (int row = 0; row < m_tableWidget->rowCount(); ++row) {
        bool rowVisible = false;
        for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = m_tableWidget->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive)) {
                rowVisible = true;
                break;
            }
        }
        m_tableWidget->setRowHidden(row, !rowVisible);
    }
}

void Cuaso::loadFile() {
    QString fileTitle = tr("Select Excel file");
    QString fileFilter = tr("Excel Files (*.xlsx)");
    QString newFilePath = QFileDialog::getOpenFileName(this, fileTitle, "", fileFilter);
    if (newFilePath.isEmpty()) {
        qDebug() << tr("Please reselect Excel file 1");
    } else {
        filepath1 = newFilePath;
        m_nutbam->setText(tr("File 1 loaded"));
        QMessageBox::information(this, tr("LOAD FILE"), tr("File 1 successfully loaded"));
        qDebug() << tr("File 1 loaded:") << filepath1;
    }
}

void Cuaso::loadFile3(){
    QString fileTitle = tr("Select Excel file");
    QString fileFilter = tr("Excel Files (*.xlsx)");
    QString newFilePath = QFileDialog::getOpenFileName(this, fileTitle, "", fileFilter);
    if (newFilePath.isEmpty()) {
        qDebug() << tr("Please reselect Excel file 2");
    } else {
        filepath3 = newFilePath;
        m_nutbam1->setText(tr("File 2 loaded"));
        QMessageBox::information(this, tr("LOAD FILE"), tr("File 2 successfully loaded"));
        qDebug() << tr("File 2 loaded:") << filepath3;
    }
}

void Cuaso::compare() {
    if (filepath1.isEmpty() || filepath3.isEmpty()) {
        QMessageBox::critical(this, tr("Reload"), tr("Not enough files"));
        qDebug() << tr("Not enough files to compare!");
        return;
    }
    QXlsx::Document doc1(filepath1);
    QXlsx::Document doc2(filepath3);
    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    if (range1.lastRow() == 0 || range1.lastColumn() == 0 ||
        range2.lastRow() == 0 || range2.lastColumn() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("Excel file is not valid for comparison"));
        return;
    }
    compareSheets(doc1, doc2);
}

void Cuaso::compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2) {
    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());
    int diffCount = 0;
    int totalCells = maxRow * maxCol;

    QTableWidget *compareWidget = new QTableWidget();
    compareWidget->setRowCount(maxRow);
    compareWidget->setColumnCount(maxCol);
    compareWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    compareWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    load->setVisible(true);
    load->setValue(0);
    load->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid grey;"
        "    border-radius: 5px;"
        "    background-color: #F0F0F0;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: green;"
        "    width: 10px;"
        "}"
        );

    QVector<QVector<QVariant>> data1(maxRow, QVector<QVariant>(maxCol));
    QVector<QVector<QVariant>> data2(maxRow, QVector<QVariant>(maxCol));
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            data1[i][j] = doc1.read(i + 1, j + 1);
            data2[i][j] = doc2.read(i + 1, j + 1);
        }
    }
    QVector<QTableWidgetItem*> items;
    for (int i = 0; i < maxRow; i++) {
        for (int j = 0; j < maxCol; j++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if (data1[i][j] != data2[i][j]) {
                diffCount++;
                item->setText(tr("❌ Different"));
                item->setBackground(Qt::darkCyan);
                item->setForeground(Qt::white);
            } else {
                item->setText(data1[i][j].toString());
            }
            items.append(item);
        }
        for (int j = 0; j < maxCol; j++) {
            compareWidget->setItem(i, j, items[i * maxCol + j]);
        }
        int progress = ((i + 1) * 100) / maxRow;
        load->setValue(progress);
        qApp->processEvents();
    }
    load->setValue(100);
    QTimer::singleShot(500, [this]() { load->setVisible(false); });
    QTableWidget *table1 = createTableFromDocument(doc1, maxRow, maxCol);
    QTableWidget *table2 = createTableFromDocument(doc2, maxRow, maxCol);

    double similarityPercent = 100.0 * (totalCells - diffCount) / totalCells;
    QWidget *tab4 = new QWidget();
    QVBoxLayout *layout_report = new QVBoxLayout(tab4);

    QStandardItemModel *report_tab = new QStandardItemModel(tab4);
    report_tab->setColumnCount(2);
    report_tab->setHorizontalHeaderLabels(QStringList() << tr("Component") << tr("Value"));

    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem(QString::number(totalCells)),
        new QStandardItem(tr("Total cells"))
    });
    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem(QString::number(diffCount)),
        new QStandardItem(tr("Different cells"))
    });
    report_tab->appendRow(QList<QStandardItem*>{
        new QStandardItem(QString("%1%").arg(similarityPercent, 0, 'f', 2)),
        new QStandardItem(tr("Similarity percentage"))
    });

    QTableView *table_Report = new QTableView(tab4);
    table_Report->setModel(report_tab);
    table_Report->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_Report->verticalHeader()->setVisible(false);
    table_Report->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_Report->setSelectionMode(QAbstractItemView::NoSelection);

    QPieSeries *series = new QPieSeries(tab4);
    series->append(tr("Matching cells"), totalCells - diffCount);
    series->append(tr("Different cells"), diffCount);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Ratio of matching and different cells"));
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout_report->addWidget(table_Report);
    layout_report->addWidget(chartView);

    if (!first->layout())
        first->setLayout(new QVBoxLayout());
    else
        clearLayout(first->layout());
    first->layout()->addWidget(table1);

    if (!second->layout())
        second->setLayout(new QVBoxLayout());
    else
        clearLayout(second->layout());
    second->layout()->addWidget(table2);

    if (!third->layout())
        third->setLayout(new QVBoxLayout());
    else
        clearLayout(third->layout());
    third->layout()->addWidget(compareWidget);

    m_tab->clear();
    m_tab->addTab(first, tr("Page 1"));
    m_tab->addTab(second, tr("Page 2"));
    m_tab->addTab(third, tr("Compare"));
    m_tab->addTab(tab4, tr("Report"));

    m_tableWidget = compareWidget;
}

QTableWidget* Cuaso::createTableFromDocument(const QXlsx::Document &doc, int maxRow, int maxCol) {
    QTableWidget *table = new QTableWidget();
    table->setRowCount(maxRow);
    table->setColumnCount(maxCol);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    for (int i = 1; i <= maxRow; i++) {
        for (int j = 1; j <= maxCol; j++) {
            QVariant val = doc.read(i, j);
            QTableWidgetItem *item = new QTableWidgetItem(val.toString());
            table->setItem(i - 1, j - 1, item);
        }
    }
    return table;
}

void Cuaso::updateUiTranslations() {
    if (filepath1.isEmpty())
        m_nutbam->setText(tr("Please load Excel file 1"));
    else
        m_nutbam->setText(tr("File 1 loaded"));
    if (filepath3.isEmpty())
        m_nutbam1->setText(tr("Please load Excel file 2"));
    else
        m_nutbam1->setText(tr("File 2 loaded"));
    sosanh->setText(tr("Click to compare"));
    m_exit->setText(tr("Click to exit"));
    searchline->setPlaceholderText(tr("Enter keyword to search"));
    if (m_tab->count() >= 3) {
        m_tab->setTabText(0, tr("Page 1"));
        m_tab->setTabText(1, tr("Page 2"));
        m_tab->setTabText(2, tr("Compare"));
    }
}
void Cuaso::exportComparisonResult() {
    if (!m_tableWidget) {
        QMessageBox::warning(this, tr("Export Failed"), tr("No comparison data to export!"));
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Excel File"), "", tr("Excel Files (*.xlsx)"));
    if (filePath.isEmpty()) {
        return;
    }

    QXlsx::Document xlsx;
    int rowCount = m_tableWidget->rowCount();
    int colCount = m_tableWidget->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = m_tableWidget->item(row, col);
            if (item) {
                xlsx.write(row + 1, col + 1, item->text());
            }
        }
    }

    if (xlsx.saveAs(filePath)) {
        QMessageBox::information(this, tr("Export Successful"), tr("File exported successfully!"));
    } else {
        QMessageBox::critical(this, tr("Export Failed"), tr("Could not save the file!"));
    }
}


void Cuaso::saveLastSession() {
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("filepath1", filepath1);
    settings.setValue("filepath2", filepath2);
    settings.setValue("filepath3", filepath3);
    // Save current language from combobox
    settings.setValue("lastLanguage", languageCombo->currentText());
}

void Cuaso::loadLastSession() {
    QSettings settings("MyCompany", "MyApp");
    filepath1 = settings.value("filepath1").toString();
    filepath2 = settings.value("filepath2").toString();
    filepath3 = settings.value("filepath3").toString();
    QString savedLang = settings.value("lastLanguage").toString();
    if (!filepath1.isEmpty()) {
        m_nutbam->setText(tr("File 1 loaded"));
        qDebug() << "Loaded file 1:" << filepath1;
    }
    if (!savedLang.isEmpty()) {
        int index = languageCombo->findText(savedLang);
        if (index >= 0) {
            languageCombo->setCurrentIndex(index);
        }
    }
    if (!filepath3.isEmpty()) {
        m_nutbam1->setText(tr("File 2 loaded"));
        qDebug() << "Loaded file 2:" << filepath3;
    }
}

Cuaso::~Cuaso() {
    saveLastSession();
}
