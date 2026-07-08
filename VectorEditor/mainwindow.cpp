#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new DrawingScene(this);
    m_scene->setSceneRect(0, 0, 2000, 2000);
    ui->graphicsView->setScene(m_scene);

    ui->comboBoxShape->addItem("Линия");
    ui->comboBoxShape->addItem("Прямоугольник");
    ui->comboBoxShape->addItem("Эллипс");
    ui->comboBoxShape->setCurrentIndex(2);

    connect(ui->comboBoxShape, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onShapeChanged);
    connect(ui->btnOutlineColor, &QPushButton::clicked, this, &MainWindow::onChooseOutlineColor);
    connect(ui->btnFillColor, &QPushButton::clicked, this, &MainWindow::onChooseFillColor);
    connect(ui->btnErase, &QPushButton::toggled, this, &MainWindow::onEraseToggled);
    connect(ui->btnSave, &QPushButton::clicked, this, &MainWindow::onSaveShapes);
    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadShapes);
    connect(ui->btnStats, &QPushButton::clicked, this, &MainWindow::onShowStats);

    onShapeChanged(ui->comboBoxShape->currentIndex());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onShapeChanged(int index)
{
    ui->btnErase->setChecked(false);
    m_scene->setShapeType(static_cast<DrawingScene::ShapeType>(index));
}

void MainWindow::onEraseToggled(bool checked)
{
    m_scene->setEraseMode(checked);
}

void MainWindow::onChooseOutlineColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Выбрать цвет контура");
    if (color.isValid())
        m_scene->setOutlineColor(color);
}

void MainWindow::onChooseFillColor()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Выбрать цвет заливки");
    if (color.isValid())
        m_scene->setFillColor(color);
}

void MainWindow::onSaveShapes()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить фигуры", QString(), "Text files (*.txt)");
    if (!fileName.isEmpty())
        m_scene->saveToFile(fileName);
}

void MainWindow::onLoadShapes()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Загрузить фигуры", QString(), "Text files (*.txt)");
    if (!fileName.isEmpty())
        m_scene->loadFromFile(fileName);
}
void MainWindow::onShowStats()
{
    DrawingScene::ShapeStats stats = m_scene->calculateStats();

    QString text = QString(
                       "Линий: %1\nПрямоугольников: %2\nЭллипсов: %3\n"
                       "Всего фигур: %4\nСуммарная площадь: %5"
                       ).arg(stats.lineCount)
                       .arg(stats.rectangleCount)
                       .arg(stats.ellipseCount)
                       .arg(stats.totalCount)
                       .arg(stats.totalArea, 0, 'f', 2);

    QMessageBox::information(this, "Статистика фигур", text);
}