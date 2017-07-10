/* ServerManager is the server-side GameManager.
 * This class is responsible for the following actions:
 *      - Turn Controller
 *      - AI turn processing
 *      - Start Turn
 *      - End Turn
 *      - Map Generation
 *      - Civ selection for civs listed as 'Random'
 *
 */

#include "servermanager.h"
#include <ui_servermanager.h>"

ServerManager::ServerManager(QWidget *parent, int mapX, int mapY, int numAI) :
    QWidget(parent)/*,
    ui(new Ui::ServerManager) */
{
    this->setWindowTitle("Project Hiawatha Server");
}
