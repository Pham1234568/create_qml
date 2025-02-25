#include "Cuaso.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "header/xlsxdocument.h"
#include <QHeaderView>
#include "header/xlsxformat.h"
#include <QSpacerItem>
#include <algorithm>
#include <QApplication>  // Dùng cho qApp

// Hàm trợ giúp: Xóa sạch các widget trong layout hiện có
void clearLayout(QLayout *layout) {
    if (!layout)
        return;
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
            item->widget()->deleteLater();
        }
        delete item;
    }
}

Cuaso::Cuaso(QWidget *parent) : QWidget(parent) {
    setMinimumSize(800, 600);

    // Khởi tạo các widget trang cho QTabWidget
    first = new QWidget();
    second = new QWidget();
    third = new QWidget();

    // Tạo các nút chức năng, sử dụng tr() cho hỗ trợ đa ngôn ngữ
    m_nutbam  = new QPushButton(tr("Hãy nạp file excel 1"), this);
    m_nutbam1 = new QPushButton(tr("Hãy nạp file excel 2"), this);
    sosanh    = new QPushButton(tr("Ấn để so sánh"), this);
    m_export  = new QPushButton(tr("Xuất dữ liệu language"), this);
    m_exit    = new QPushButton(tr("Ấn để thoát"), this);

    // Đặt chính sách kích thước cho các nút
    m_nutbam->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_nutbam1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sosanh->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_export->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_exit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Khởi tạo TableWidget để hiển thị kết quả so sánh
    m_tableWidget = new QTableWidget();
    m_tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Khởi tạo QTabWidget và bố trí giao diện
    m_tab = new QTabWidget(this);

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(m_nutbam);
    buttonLayout->addWidget(m_nutbam1);
    buttonLayout->addWidget(sosanh);
    buttonLayout->addWidget(m_export);  // Thêm nút xuất dữ liệu
    buttonLayout->addWidget(m_exit);
    buttonLayout->addStretch();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_tab, 1);

    setLayout(mainLayout);

    // Kết nối các nút với các slot tương ứng
    connect(m_nutbam,  &QPushButton::clicked, this, &Cuaso::loadFile);
    connect(m_nutbam1, &QPushButton::clicked, this, &Cuaso::loadFile2);
    connect(sosanh,    &QPushButton::clicked, this, &Cuaso::compare);
    connect(m_export,  &QPushButton::clicked, this, &Cuaso::exportLanguageData);
    connect(m_exit,    &QPushButton::clicked, this, &Cuaso::close); // Kết nối nút thoát
}

void Cuaso::loadFile(){
    QString newFilePath = QFileDialog::getOpenFileName(this, tr("Chọn file excel"), "", tr("Excel Files (*.xlsx)"));
    if(newFilePath.isEmpty()){
        qDebug() << tr("Hãy chọn lại file excel 1!");
    } else {
        filepath1 = newFilePath;
        m_nutbam->setText(tr("Đã nạp file 1"));
        QMessageBox::information(this, tr("NẠP FILE"), tr("Bạn đã nạp thành công file 1"));
        qDebug() << tr("File 1 được nạp:") << filepath1;
    }
}

void Cuaso::loadFile2(){
    QString newFilePath = QFileDialog::getOpenFileName(this, tr("Chọn file excel"), "", tr("Excel Files (*.xlsx)"));
    if(newFilePath.isEmpty()){
        qDebug() << tr("Hãy chọn lại file excel 2!");
    } else {
        filepath2 = newFilePath;
        m_nutbam1->setText(tr("Đã nạp file 2"));
        QMessageBox::information(this, tr("NẠP FILE"), tr("Bạn đã nạp thành công file 2"));
        qDebug() << tr("File 2 được nạp:") << filepath2;
    }
}

void Cuaso::compare(){
    if(filepath1.isEmpty() || filepath2.isEmpty()){
        QMessageBox::critical(this, tr("Nhập lại"), tr("Chưa có đủ file"));
        qDebug() << tr("Chưa nạp đủ file để so sánh!");
        return;
    }

    QXlsx::Document doc1(filepath1);
    QXlsx::Document doc2(filepath2);

    // Nếu không thể mở file, in thông báo lỗi
    if (!doc1.load() || !doc2.load()){
        QMessageBox::information(this, tr("Nhập lại"), tr("Không thể mở file excel"));
        qDebug() << tr("Lỗi khi mở file!");
        return;
    }

    compareSheets(doc1, doc2);
}

void Cuaso::compareSheets(const QXlsx::Document &doc1, const QXlsx::Document &doc2) {
    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());

    // Cập nhật m_tableWidget cho trang so sánh
    m_tableWidget->clear();
    m_tableWidget->setRowCount(maxRow);
    m_tableWidget->setColumnCount(maxCol);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    for (int i = 1; i <= maxRow; i++) {
        for (int j = 1; j <= maxCol; j++) {
            QVariant val1 = doc1.read(i, j);
            QVariant val2 = doc2.read(i, j);
            QTableWidgetItem *item = new QTableWidgetItem();
            if (val1 != val2) {
                // Hiển thị điểm khác biệt với nền đỏ
                item->setText(tr("❌ Khác"));
                item->setBackground(Qt::red);
                item->setForeground(Qt::white);
            } else {
                item->setText(val1.toString());
            }
            m_tableWidget->setItem(i - 1, j - 1, item);
        }
    }

    // Tạo bảng cho file 1 và file 2
    QTableWidget *table1 = createTableFromDocument(doc1, maxRow, maxCol);
    QTableWidget *table2 = createTableFromDocument(doc2, maxRow, maxCol);

    // Cập nhật tab Trang 1
    if (first->layout()) {
        clearLayout(first->layout());
    } else {
        first->setLayout(new QVBoxLayout());
    }
    first->layout()->addWidget(table1);

    // Cập nhật tab Trang 2
    if (second->layout()) {
        clearLayout(second->layout());
    } else {
        second->setLayout(new QVBoxLayout());
    }
    second->layout()->addWidget(table2);

    // Cập nhật tab So sánh
    if (third->layout()) {
        clearLayout(third->layout());
    } else {
        third->setLayout(new QVBoxLayout());
    }
    // Đảm bảo m_tableWidget được chuyển sang tab này
    m_tableWidget->setParent(nullptr);
    third->layout()->addWidget(m_tableWidget);

    // Cập nhật các tab trong QTabWidget
    m_tab->clear();
    m_tab->addTab(first, tr("Trang 1"));
    m_tab->addTab(second, tr("Trang 2"));
    m_tab->addTab(third, tr("So sánh"));

    adjustSize();
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

void Cuaso::exportLanguageData() {
    if(filepath1.isEmpty() || filepath2.isEmpty()){
        QMessageBox::warning(this, tr("Xuất dữ liệu"), tr("Chưa nạp đủ file excel để xuất dữ liệu!"));
        return;
    }

    QXlsx::Document doc1(filepath1);
    QXlsx::Document doc2(filepath2);

    if (!doc1.load() || !doc2.load()){
        QMessageBox::critical(this, tr("Xuất dữ liệu"), tr("Không thể mở file excel!"));
        return;
    }

    // Xác định kích thước tổng hợp từ hai file
    QXlsx::CellRange range1 = doc1.dimension();
    QXlsx::CellRange range2 = doc2.dimension();
    int maxRow = std::max(range1.lastRow(), range2.lastRow());
    int maxCol = std::max(range1.lastColumn(), range2.lastColumn());

    // Tạo một document mới để ghi dữ liệu kết hợp: nếu có điểm khác, dùng giá trị từ file 2 (đã cập nhật)
    QXlsx::Document exportDoc;
    for (int i = 1; i <= maxRow; i++){
        for (int j = 1; j <= maxCol; j++){
            QVariant val1 = doc1.read(i, j);
            QVariant val2 = doc2.read(i, j);
            QVariant newVal = (val1 != val2) ? val2 : val1;
            exportDoc.write(i, j, newVal);
        }
    }

    QString exportPath = QFileDialog::getSaveFileName(this, tr("Xuất file ngôn ngữ"), "", tr("Excel Files (*.xlsx)"));
    if(exportPath.isEmpty()){
        return;
    }

    if(exportDoc.saveAs(exportPath))
        QMessageBox::information(this, tr("Xuất dữ liệu"), tr("Xuất dữ liệu ngôn ngữ thành công!"));
    else
        QMessageBox::critical(this, tr("Xuất dữ liệu"), tr("Xuất dữ liệu ngôn ngữ thất bại!"));
}
