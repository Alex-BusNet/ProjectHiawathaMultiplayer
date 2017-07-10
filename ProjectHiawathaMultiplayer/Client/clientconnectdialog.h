#ifndef CLIENTCONNECTDIALOG_H
#define CLIENTCONNECTDIALOG_H

#include "clientoptions.h"
#include <QDialog>
#include "clienthandler.h"

namespace Ui {
class ClientConnectDialog;
}

class ClientConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientConnectDialog(bool fullscreen = false, QWidget *parent = 0);
    ~ClientConnectDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ClientConnectDialog *ui;
    ClientOptions *co;
    bool fullscreen;
};

#endif // CLIENTCONNECTDIALOG_H
