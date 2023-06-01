#include "LobbyWindow.h"
#include "ui_LobbyWindow.h"
#include "../../common/network/responses/ServerResponse.h"
#include "../ClientCommunicationToolbox/ClientCommunicationToolbox.h"
#include "../ClientCommunicationToolbox/ClientNetworkManager.h"
#include "MainWindow.h"

#include <QMessageBox>
#include <iostream>
#include <QTimer>

using namespace std;

LobbyWindow::LobbyWindow(Game *gamestate, QWidget *parent, QString name, QString ip, QString port, Player *currPlayer) :
    QDialog(parent),
    ui(new Ui::LobbyWindow)
{
    this->game = gamestate;
    this->name = name;
    this->ip = ip;
    this->port = port;
    this->_currPlayer = currPlayer;

    ui->setupUi(this);
    this->_setupWindow();

    // Setting up mainloop
    _recievedMessage = "";
    _mainloopTimer = new QTimer{this};
    connect(_mainloopTimer, &QTimer::timeout, this, &LobbyWindow::_mainloop);
    _mainloopTimer->start(20);

    // Setting up media player
    string projectRootPath = _getProjectAbsPath();
    QString lobbyWindowBGMPath = QString::fromStdString(projectRootPath + "BGM/LobyWindowBGM.mp3");
    this->_mediaPlayer = new QMediaPlayer;
    this->_audioOutput = new QAudioOutput();
    this->_mediaPlayer->setAudioOutput(_audioOutput);
    this->_audioOutput->setVolume(20);
    _mediaPlayer->setSource(QUrl::fromLocalFile(lobbyWindowBGMPath));
    _mediaPlayer->play();
}

void LobbyWindow::_mainloop() {

    //This does not include start game process
    if (_needUpdate && !_windowClosed){
        _needUpdate = false;

        try{
            rapidjson::Document json = rapidjson::Document (rapidjson::kObjectType);
            json.Parse(_recievedMessage.c_str());

            ServerResponse* res = ServerResponse::fromJson(json);
            Game* recievedGame = res->Process();

            if (recievedGame != nullptr){
                delete game;
                game = recievedGame;
                if(recievedGame->isStarted()){
                    cerr << "Game is started" << endl;
                    MainWindow* mainWindow = new MainWindow(_currPlayer, game, nullptr);
                    ClientNetworkManager::setActingWindow(mainWindow);
                    mainWindow->show();
                    this->close();
                    _windowClosed = true;
                    _mediaPlayer->stop();
                }else{
                    // The game have not started, update current window
                    cout << "trying update the player slots" << endl;
                    this->_setupWindow();
                }

            }
        }catch (std::exception e){
            QMessageBox::information(this, "SON parsing error", QString::fromStdString("Failed to parse message from server:\n" + _recievedMessage + "\n" + (std::string) e.what()));
        }
    }
}

LobbyWindow::~LobbyWindow()
{
    delete ui;
}

void LobbyWindow::_updatePlayerLs() {
    std::vector<Player*> currPlayerLs = game->getPlayers();
    for (Player* curr : currPlayerLs){
        QString currPlayerName = QString::fromStdString(curr->getName());
        int inPlayerVec = std::count(playerLs.begin(), playerLs.end(), currPlayerName);
        if(inPlayerVec == 0){
            playerLs.push_back(currPlayerName);
        }
    }
}

void LobbyWindow::_setupWindow()  {
    ui->label_ip->setText(QString("IP Address: %1").arg(ip));
    ui->label_name->setText(QString("Player Name: %1").arg(name));
    ui->label_port->setText(QString("Port Number: %1").arg(port));

    ui->listWidget->clear();
    _updatePlayerLs();

    for(const QString& currPlayerName : playerLs){
        ui->listWidget->addItem(currPlayerName);
    }
}

void LobbyWindow::parseResponse(const std::string message) {
#ifdef PRINT_NETWORK_MESSAGES
    cout << "Recieved response: " << message << endl;
#endif
    _recievedMessage = message;
    _needUpdate = true;
}

void LobbyWindow::on_buttonExit_clicked() {
    qApp->exit();
}

void LobbyWindow::on_buttonStartGame_clicked() {
    ClientCommunicationToolbox::startGame(game, _currPlayer);
}






// Things to get abs path for the game, used in
string LobbyWindow::_getCurrFilename() {
    const char* path = __FILE__;
    const char* filename = std::strrchr(path, '/');
    if (filename) {
        ++filename;
    } else {
        filename = path;
    }
    return filename;
}

string LobbyWindow::_getProjectAbsPath() {
    std::string relativePath = "../../../";
    std::string dirPath = __FILE__;
    string currFileName = _getCurrFilename();
    dirPath = dirPath.substr(0, dirPath.size() - currFileName.size());
    std::string absolutePath = dirPath + relativePath;
    return absolutePath;
}
