#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Client/clientoptions.h"
#include "Server/serveroptions.h"
#include "Common/about.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_client_clicked();

    void on_exit_clicked();

    void on_fullscreen_clicked();

    void on_controls_clicked();

    void on_server_clicked();

private:
    Ui::MainWindow *ui;
    ServerOptions *sOptions;
    ClientOptions *cOptions;
    About *about;
    bool fullscreen;
    QPixmap *bkgnd, *buttonBorder;
};

#endif // MAINWINDOW_H
