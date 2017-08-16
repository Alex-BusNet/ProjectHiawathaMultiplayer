/* The ClientConnectDialog is used by
 * the player to enter their username and
 * host IP address.
 *
 * If the connection to the host completes,
 * then the ClientOptions screen opens.
 */

#include "clientconnectdialog.h"
#include "ui_clientconnectdialog.h"

ClientConnectDialog::ClientConnectDialog(bool fullscreen, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientConnectDialog)
{
    ui->setupUi(this);

    this->setFixedSize(this->width(), this->height());

    co = NULL;

    ui->hostIP->setText("127.0.0.1");
    ui->username->setText("Sparta");
    ui->errorLabel->setVisible(false);
    this->fullscreen = fullscreen;
}

ClientConnectDialog::~ClientConnectDialog()
{
    delete ui;
}

void ClientConnectDialog::on_buttonBox_accepted()
{
    ClientHandler::instance()->SetHost(ui->hostIP->text(), 5300);
    ClientHandler::instance()->SetSocket(new QTcpSocket());
    ClientHandler::instance()->SetUser(ui->username->text());

    if(ClientHandler::instance()->connectToHost())
    {
        if(co != NULL)
            delete co;

        co = new ClientOptions(0, ui->username->text(), fullscreen);
        co->show();
        ui->errorLabel->setVisible(false);
    }
    else
    {
        ui->errorLabel->setVisible(true);
        this->show();
    }
}

void ClientConnectDialog::on_buttonBox_rejected()
{
    this->close();
}
