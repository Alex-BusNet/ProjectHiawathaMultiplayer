#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>

QString MainStyle = "QPushButton { background-color: #4899C8; border: 1px solid black; border-radius: 6px; font: 10px; max-width: 201px; min-width: 75; min-height: 23px; max-height: 61px; } QPushButton:pressed { background-color: #77adcb; }";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifdef __APPLE__
    QDir bin(QCoreApplication::applicationDirPath());
    bin.cdUp();
    bin.cdUp();
    bin.cdUp();
    QDir::setCurrent(bin.absolutePath());
#endif

    sOptions = NULL;
    cOptions = NULL;
    about = NULL;
    fullscreen = false;
    bkgnd = new QPixmap("Assets/Menu/Background/dom_hiawatha.png");
    buttonBorder = new QPixmap("Assets/Menu/mainMenuBackground.png");
    ui->mainBackground->setPixmap(*bkgnd);
    ui->menuButtonBorder->setPixmap(*buttonBorder);
    this->setStyleSheet(MainStyle);
}

MainWindow::~MainWindow()
{
    if(sOptions != NULL)
        delete sOptions;

    if(bkgnd != NULL)
        delete bkgnd;

    if(buttonBorder != NULL)
        delete buttonBorder;

    if(ui != NULL)
        delete ui;

    if(cOptions != NULL)
        delete cOptions;

    if(about != NULL)
        delete about;
}

void MainWindow::on_client_clicked()
{   
    if(cOptions != NULL)
        delete cOptions;

    cOptions = new ClientOptions(0,fullscreen);
    cOptions->show();
}


void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_fullscreen_clicked()
{
    this->fullscreen = !(this->fullscreen);
}

void MainWindow::on_controls_clicked()
{
    if(about != NULL)
    {
        delete about;
    }

    about = new About();
    about->show();
}

void MainWindow::on_server_clicked()
{
    if(sOptions != NULL)
    {
        delete sOptions;
    }

    sOptions = new ServerOptions();
    sOptions->show();
}
