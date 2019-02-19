#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openGLWidget->context();

    connect(ui->pushButton, SIGNAL (released()), this, SLOT (handleButton1()));
    connect(ui->pushButton_2, SIGNAL (released()), this, SLOT (handleButton2()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
//    if(arg1 == "Naive 1")
//    {
//        OGLWidget::_NAIVE1ALGO = true;
//        OGLWidget::_NAIVE2ALGO = false;
//    }
//    else if(arg1 == "Naive 2")
//    {
//        OGLWidget::_NAIVE1ALGO = false;
//        OGLWidget::_NAIVE2ALGO = true;
//    }
//    else if(arg1 == "Bresenham")
//    {
//        OGLWidget::_NAIVE1ALGO = false;
//        OGLWidget::_NAIVE2ALGO = false;
//    }
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    cout << "CALLED" << endl;

    if(arg1 == "AiAgent - Polícia")
    {
        OGLWidget::_AIAGENT_OPTION = true;
        OGLWidget::_AIAGENT_TYPE = COP;
        OGLWidget::_TILE_OPTION = false;
    }
    else if(arg1 == "AiAgent - Bandido")
    {
        OGLWidget::_AIAGENT_OPTION = true;
        OGLWidget::_AIAGENT_TYPE = BANDIT;
        OGLWidget::_TILE_OPTION = false;
    }
    else if(arg1 == "Tile - Parede")
    {
        OGLWidget::_AIAGENT_OPTION = false;
        OGLWidget::_TILE_OPTION = true;
        OGLWidget::_TILE_TYPE = WALL;
    }
}

void MainWindow::handleButton1()
{
    OGLWidget::_CLEAR_AIAGENTS = true;
}

void MainWindow::handleButton2()
{
    OGLWidget::_CLEAR_FIELD = true;
}
