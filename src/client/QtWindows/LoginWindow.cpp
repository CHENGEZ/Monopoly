#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QString>
#include <QMessageBox>
#include <iostream>
#include <string>
#include <QTimer>

#include "sockpp/tcp_connector.h"
#include "MainWindow.h"
#include "../../common/default.conf"
#include "../../common/network/responses/ServerResponse.h"
#include "../ClientCommunicationToolbox/ClientNetworkManager.h"
#include "../ClientCommunicationToolbox/ClientCommunicationToolbox.h"

using namespace std;

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->lineEdit_serverAddress->setText(QString::fromStdString(default_server_host));
    ui->lineEdit_serverPort->setText(QString::number(default_port));

    string projectRootPath = _getProjectAbsPath();
    QString mainWindowBGMPath = QString::fromStdString(projectRootPath + "BGM/LoginWindowBGM.mp3");

    this->mediaPlayer = new QMediaPlayer;
    this->audioOutput = new QAudioOutput();
    this->mediaPlayer->setAudioOutput(audioOutput);
    this->audioOutput->setVolume(20);
    mediaPlayer->setSource(QUrl::fromLocalFile(mainWindowBGMPath));
    mediaPlayer->play();

    _recievedMessage = "";

    _mainloopTimer = new QTimer{this};
    connect(_mainloopTimer, &QTimer::timeout, this, &LoginWindow::_mainloop);
    _mainloopTimer->start(20);

}

LoginWindow::~LoginWindow()
{
//    std::cout << "aaa" << std::endl;
    delete ui;
}

void LoginWindow::on_buttonConnect_clicked(){
    QString serverAddress = ui->lineEdit_serverAddress->text();
    QString serverPort = ui->lineEdit_serverPort->text();
    QString playerName = ui->lineEdit_playerName->text();
    
//    QMessageBox::information(this, "login", QString("you have typed: \nServer Address: %1\n"
//                                                    "Server Port: %2\nPlayer Name: %3").
//                                            arg(serverAddress).arg(serverPort).arg(playerName));
    std::string strServerAddress = serverAddress.toStdString();
    std::string strServerPort = serverPort.toStdString();
    std::string strPlayerName = playerName.toStdString();

    // Check if there is something empty
    if(strServerAddress.empty() || strPlayerName.empty() || strPlayerName.empty()){
        QMessageBox::information(this, "Error", "Please input all the boxes");
        return;
    }
    this->_host = strServerAddress;
    this->_name = strPlayerName;
    try{
        this->_port = std::stoi(strServerPort);
    }catch (const std::invalid_argument& e){
        QMessageBox::information(this, "Port Error", "Please enter valid int port");
        return;
    }catch (const std::out_of_range& e){
        QMessageBox::information(this, "Port Error", "Port number out of range (0~65535)");
        return;
    }

    if(this->_port > 65535){
        QMessageBox::information(this, "Port Error", "Port number out of range (0~65535)");
        return;
    }

    ClientNetworkManager::setActingWindow(this);
    _currPlayer = ClientCommunicationToolbox::connectToServer(_host, _port, _name);
}

void LoginWindow::on_buttonExit_clicked() {
    qApp->exit();
}

void LoginWindow::parseResponse(const std::string message) {

#ifdef PRINT_NETWORK_MESSAGES
    cout << "Recieved response: " << message << endl;
#endif
    _recievedMessage = message;
    _needUpdate = true;
}

bool LoginWindow::checkInputValid() const {
    if (this->_name.empty() || this->_host.empty() || this->_port > 65535){
        return false;
    }
    return true;
}

string LoginWindow::_getCurrFilename() {
    const char* path = __FILE__;
    const char* filename = std::strrchr(path, '/');
    if (filename) {
        ++filename;
    } else {
        filename = path;
    }
    return filename;
}

string LoginWindow::_getProjectAbsPath() {
    std::string relativePath = "../../../";
    std::string dirPath = __FILE__;
    string currFileName = _getCurrFilename();
    dirPath = dirPath.substr(0, dirPath.size() - currFileName.size());
    std::string absolutePath = dirPath + relativePath;
    return absolutePath;
}

void LoginWindow::_mainloop() {

    if(_needUpdate && !_windowClosed){
        _needUpdate = false;
        try{
            rapidjson::Document json = rapidjson::Document (rapidjson::kObjectType);
            json.Parse(_recievedMessage.c_str());

            ServerResponse* res = ServerResponse::fromJson(json);
            Game* game = res->Process();

            // enter lobby
            if (game != nullptr){
                LobbyWindow* lobbyWindow = new LobbyWindow(game,
                                                           nullptr,
                                                           QString::fromStdString(_name),
                                                           QString::fromStdString(_host),
                                                           QString::number(_port), _currPlayer);
                ClientNetworkManager::setActingWindow(lobbyWindow);
                lobbyWindow->show();
                this->close();
                mediaPlayer->stop();
                _windowClosed = true;
            }else{
                QMessageBox::information(this, "Fatal Error", "Connection failed, to ensure every other players can play normally, reboot the client.");
                qApp->exit();
            }
        } catch (std::exception e){
            QMessageBox::information(this, "SON parsing error", QString::fromStdString("Failed to parse message from server:\n" + _recievedMessage + "\n" + (std::string) e.what()));
        }
    }
}
