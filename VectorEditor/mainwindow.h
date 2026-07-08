#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onShapeChanged(int index);
    void onChooseOutlineColor();
    void onChooseFillColor();
    void onEraseToggled(bool checked);
    void onSaveShapes();
    void onLoadShapes();
    void onShowStats();

private:
    Ui::MainWindow *ui;
    DrawingScene *m_scene;
};

#endif // MAINWINDOW_H