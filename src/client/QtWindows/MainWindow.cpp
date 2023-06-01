#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

#include "PopupWindow.h"
#include "../../common/Cell/Avenue.hpp"
#include "../../common/Cell/Railway.hpp"
#include "../../common/Cell/Cell.hpp"
#include "../../common/network/responses/ServerResponse.h"
#include "../ClientCommunicationToolbox/ClientCommunicationToolbox.h"
#include "../HelperFunctions.cpp"
#include "ui_PopupWindow.h"

#include <iostream>
#include <QPixmap>
#include <filesystem>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _needUpdate = false;

    // Init display arrays
    _initializeLocArr();
    _initBalanceArr();
    _initNameArr();

    // Set BGM
//    string projectRootPath = _getProjectAbsPath();
//    QString mainWindowBGMPath = QString::fromStdString(projectRootPath + "BGM/MainWindowBGM.mp3");
//    this->mediaPlayer = new QMediaPlayer;
//    this->audioOutput = new QAudioOutput();
//    this->mediaPlayer->setAudioOutput(audioOutput);
//    this->audioOutput->setVolume(0);
//    mediaPlayer->setSource(QUrl::fromLocalFile(mainWindowBGMPath));
//    mediaPlayer->play();

    // Update UI
    _updateUI();


}

MainWindow::MainWindow(Player *currPlayer, Game *game, QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->currPlayer->setText("Your name is: " + QString::fromStdString(currPlayer->getName()));

    // Set variables
    this->currPlayer = currPlayer;
    this->game = game;

    // Init display arrays and color array
    _initializeLocArr();
    _initBalanceArr();
    _initNameArr();
    _initColorArr();

    // Setting up mainloop
    _recievedMessage = "";
    _mainloopTimer = new QTimer{this};
    connect(_mainloopTimer, &QTimer::timeout, this, &MainWindow::_mainloop);
    _mainloopTimer->start(20);


    // setup and play BGM
    string projectRootPath = _getProjectAbsPath();
    QString mainWindowBGMPath = QString::fromStdString(projectRootPath + "BGM/MainWindowBGM.mp3");
    this->mediaPlayer = new QMediaPlayer;
    this->audioOutput = new QAudioOutput();
    this->mediaPlayer->setAudioOutput(audioOutput);
    this->audioOutput->setVolume(20);
    mediaPlayer->setSource(QUrl::fromLocalFile(mainWindowBGMPath));
    mediaPlayer->play();

    // Update UI
    _updateUI();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete game;
}




// _mainloop used to handle ui
void MainWindow::_mainloop() {
    // Decode the response

    if (_needUpdate && !_windowClosed){
        _needUpdate = false;

        try{
            rapidjson::Document json = rapidjson::Document(rapidjson::kObjectType);
            json.Parse(_recievedMessage.c_str());

            ServerResponse* res = ServerResponse::fromJson(json);
            Game* recievedGame = res->Process();
            Game* oldGame = this->game;

            if (recievedGame != nullptr){
                game = recievedGame;
                currPlayer = game->getPlayerFromId(currPlayer->getId());
                string currDelta = game->getDelta();
                showMessage(currDelta);
                _updateUI();
                delete oldGame;
            }else{
                cerr << "Game is nulltpr, some bugs happens" << endl;
            }
        }catch (std::exception e){
            QMessageBox::information(this, "SON parsing error", QString::fromStdString("Failed to parse message from server:\n" + _recievedMessage + "\n" + (std::string) e.what()));
        }
    }
}



// update UI and its helper function
void MainWindow::_updateUI() {
    // Clear current things in UI
    _clearUI();

    // Update game state and the player location
    _updateGameState();
    _updatePlayerLoc();
}

void MainWindow::_updateGameState() {
    vector<Player*> playerVec = game->getPlayers();


    const Player* playingPlayer = game->getCurrentPlayer();
    string playingPlayerName = playingPlayer->getName();
    int round = game->getRoundNumber();

    cerr << "Current Round: " << round << endl;

    QString roundDisplay = QString::fromStdString("Round: ") + QString::number(round);

    // Update meta data
    ui->label_round->setText(roundDisplay);
    ui->label_trun->setText(QString::fromStdString(playingPlayerName) + "'s turn");

    // Update balance and player name loc
    for(int i = 0; i < playerVec.size(); i++){

        // Update color
        _playerNameLabelArr[i]->setStyleSheet(_colorArr[i]);

        // Update text
        _playerNameLabelArr[i]->setText(QString::fromStdString(playerVec[i]->getName()));
        _playerBalanceLabelArr[i]->setText("$" + QString::number(playerVec[i]->getBalance()));
    }

    // Update out of jail free card
    if(currPlayer->getNumGetOutOfJailFreeCard() > 0){
        ui->pushButton_jailFreeCard->show();
        ui->pushButton_jailFreeCard->setText(QString::fromStdString("Free out jail: " + to_string(currPlayer->getNumGetOutOfJailFreeCard())));
    }else{
        ui->pushButton_jailFreeCard->hide();
    }
}

void MainWindow::_updatePlayerLoc() {
    vector<Player*> playerVec = game->getPlayers();
    for (int j = 0; j < playerVec.size(); j++){
        const Player* curr = playerVec[j];
        const int i = curr->getPosition();
        if (i != 10){
            playerLocArr[i][j]->setStyleSheet(_colorArr[j]);
        }else{
            if (curr->getTurnsLeftInJail() != 0){
                playerLocArr[i][j]->setStyleSheet(_colorArr[j]);
            }else{
                _playerOutJailLabelArr[j]->setStyleSheet(_colorArr[j]);
            }
        }

    }
}

void MainWindow::_clearUI() {
    QPixmap empty;
    for(int i = 0; i < 40; i++){
        for(int j = 0; j < 6; j++){
            playerLocArr[i][j]->setPixmap(empty);
            playerLocArr[i][j]->setStyleSheet("background-color : transparent;");
        }
    }
    for(int i = 0; i < 6; i++){
        _playerBalanceLabelArr[i]->setText("");
        _playerNameLabelArr[i]->setText("");
        _playerOutJailLabelArr[i]->setPixmap(empty);
        _playerOutJailLabelArr[i]->setStyleSheet("background-color : transparent;");
    }

    if(currPlayer->getNumGetOutOfJailFreeCard() > 0){
        ui->pushButton_jailFreeCard->show();
    }else{
        ui->pushButton_jailFreeCard->hide();
    }
}


void MainWindow::parseResponse(const std::string message) {

#ifdef PRINT_NETWORK_MESSAGES
    cout << "Recieved response: " << message << endl;
#endif
    _recievedMessage = message;
    _needUpdate = true;
}


// Functional button clicked functions

void MainWindow::on_pushButton_jailFreeCard_clicked()
{
    bool success = ClientCommunicationToolbox::useGetOutOfJailFree(game->getId(), currPlayer->getId());
    if (success){
        cout << "successful use get out of jail free card" << endl;
    }
}

void MainWindow::on_buttonRollDice_clicked() {
    bool success = ClientCommunicationToolbox::rollDiceRequest(game->getId(), currPlayer->getId());
    if (success){
        cout << "successful send roll dice request" << endl;
    }
}

void MainWindow::on_buttonNextTurn_clicked() {
    bool success = ClientCommunicationToolbox::endTurn(game->getId(), currPlayer->getId());
    if (success){
        cout << "successful send end turn request" << endl;
    }
}


// On board button clicked functions

void MainWindow::on_pushButton_cell1_clicked()
{
    string picturePath = getAbsAvePlotPath("1.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[1];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell3_clicked()
{
    string picturePath = getAbsAvePlotPath("3.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[3];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell5_clicked()
{
    string picturePath = getAbsAvePlotPath("railway1.png");
    Railway *myRailway = (Railway*) game->getCells()[5];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
                                                 );
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell6_clicked()
{
    string picturePath = getAbsAvePlotPath("6.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[6];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell8_clicked()
{
    string picturePath = getAbsAvePlotPath("8.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[8];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell9_clicked()
{
    string picturePath = getAbsAvePlotPath("9.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[9];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell11_clicked()
{
    string picturePath = getAbsAvePlotPath("11.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[11];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}

void MainWindow::on_pushButton_cell12_clicked() {
    string picturePath = getAbsAvePlotPath("12.png");
    Utility *myRailway = (Utility*) game->getCells()[12];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
    );
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();

    currPopWindow->show();
}


void MainWindow::on_pushButton_cell13_clicked()
{
    string picturePath = getAbsAvePlotPath("13.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[13];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell14_clicked()
{
    string picturePath = getAbsAvePlotPath("14.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[14];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell15_clicked()
{
    string picturePath = getAbsAvePlotPath("railway2.png");
    Railway *myRailway = (Railway*) game->getCells()[15];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
                                                 );
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell16_clicked()
{
    string picturePath = getAbsAvePlotPath("16.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[16];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell18_clicked()
{
    string picturePath = getAbsAvePlotPath("18.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[18];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell19_clicked()
{
    string picturePath = getAbsAvePlotPath("19.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[19];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell21_clicked()
{
    string picturePath = getAbsAvePlotPath("21.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[21];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell23_clicked()
{
    string picturePath = getAbsAvePlotPath("23.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[23];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell24_clicked()
{
    string picturePath = getAbsAvePlotPath("24.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[24];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell25_clicked()
{
    string picturePath = getAbsAvePlotPath("railway3.png");
    Railway *myRailway = (Railway*) game->getCells()[25];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
                                                 );
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell26_clicked()
{
    string picturePath = getAbsAvePlotPath("26.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[26];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell27_clicked()
{
    string picturePath = getAbsAvePlotPath("27.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[27];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}

void MainWindow::on_pushButton_cell28_clicked() {
    string picturePath = getAbsAvePlotPath("water.png");
    Utility *myRailway = (Utility*) game->getCells()[28];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
    );
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();

    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));

    currPopWindow->show();
}


void MainWindow::on_pushButton_cell29_clicked()
{    string picturePath = getAbsAvePlotPath("29.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[29];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell31_clicked()
{
    string picturePath = getAbsAvePlotPath("31.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[31];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell32_clicked()
{
    string picturePath = getAbsAvePlotPath("32.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[32];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell34_clicked()
{
    string picturePath = getAbsAvePlotPath("34.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[34];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell35_clicked()
{
    string picturePath = getAbsAvePlotPath("railway4.png");
    Railway *myRailway = (Railway*) game->getCells()[35];
    QString avenueName = QString::fromStdString(myRailway->getName());
    std::string ownerID = myRailway->getOwnerId();
    int price = myRailway->getPrice();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price
                                                 );
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myRailway->getId()));
    currPopWindow->ui->buttonAddBuilding->hide();
    currPopWindow->ui->buttonRemoveBuilding->hide();
    currPopWindow->ui->aveNumBuilding_input->hide();
    currPopWindow->ui->numBuildings->hide();
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell37_clicked()
{
    string picturePath = getAbsAvePlotPath("37.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[37];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::on_pushButton_cell39_clicked()
{
    string picturePath = getAbsAvePlotPath("39.png");
    Avenue *myAvenue = (Avenue*) game->getCells()[39];
    QString avenueName = QString::fromStdString(myAvenue->getName());
    std::string ownerID = myAvenue->getOwnerId();
    int price = myAvenue->getPrice();
    int numOfBuilding = myAvenue->getCurrentNumBuildings();
    QString ownerName;

    if (ownerID.length() > 0){
        Player *myPlayer = game->getPlayerFromId(ownerID);
        ownerName = QString::fromStdString(myPlayer->getName());
    }
    else {
        ownerName = "NO OWNER";
    }

    PopupWindow* currPopWindow = new PopupWindow(QString::fromStdString(picturePath),
                                                 avenueName,
                                                 ownerName,
                                                 price,
                                                 numOfBuilding);
    currPopWindow->setPlayerID(currPlayer->getId());
    currPopWindow->setGameID(game->getId());
    currPopWindow->setCellID(to_string(myAvenue->getId()));
    currPopWindow->show();
}


void MainWindow::_initNameArr(){
    this->_playerNameLabelArr = new QLabel*[6];
    _playerNameLabelArr[0] = ui->player1_name;
    _playerNameLabelArr[1] = ui->player2_name;
    _playerNameLabelArr[2] = ui->player3_name;
    _playerNameLabelArr[3] = ui->player4_name;
    _playerNameLabelArr[4] = ui->player5_name;
    _playerNameLabelArr[5] = ui->player6_name;
}

void MainWindow::_initBalanceArr() {
    this->_playerBalanceLabelArr = new QLabel*[6];
    _playerBalanceLabelArr[0] = ui->player1_balance;
    _playerBalanceLabelArr[1] = ui->player2_balance;
    _playerBalanceLabelArr[2] = ui->player3_balance;
    _playerBalanceLabelArr[3] = ui->player4_balance;
    _playerBalanceLabelArr[4] = ui->player5_balance;
    _playerBalanceLabelArr[5] = ui->player6_balance;
}

void MainWindow::_initColorArr() {
    _colorArr[0] = "background-color : red; border: 2px solid black;";
    _colorArr[1] = "background-color : green; border: 2px solid black;";
    _colorArr[2] = "background-color : blue; border: 2px solid black;";
    _colorArr[3] = "background-color : yellow; border: 2px solid black;";
    _colorArr[4] = "background-color : orange; border: 2px solid black;";
    _colorArr[5] = "background-color : purple; border: 2px solid black;";
}

void MainWindow::_initializeLocArr() {
    this->playerLocArr = new QLabel**[40];
    for (int i = 0; i < 40; i++){
        this->playerLocArr[i] = new QLabel*[6];
    }

    _playerOutJailLabelArr = new QLabel*[6];
    _playerOutJailLabelArr[0] = ui->label_outJail_1;
    _playerOutJailLabelArr[1] = ui->label_outJail_2;
    _playerOutJailLabelArr[2] = ui->label_outJail_3;
    _playerOutJailLabelArr[3] = ui->label_outJail_4;
    _playerOutJailLabelArr[4] = ui->label_outJail_5;
    _playerOutJailLabelArr[5] = ui->label_outJail_6;

    playerLocArr[0][0] = ui->label_cell0_player1;
    playerLocArr[0][1] = ui->label_cell0_player2;
    playerLocArr[0][2] = ui->label_cell0_player3;
    playerLocArr[0][3] = ui->label_cell0_player4;
    playerLocArr[0][4] = ui->label_cell0_player5;
    playerLocArr[0][5] = ui->label_cell0_player6;
    playerLocArr[1][0] = ui->label_cell1_player1;
    playerLocArr[1][1] = ui->label_cell1_player2;
    playerLocArr[1][2] = ui->label_cell1_player3;
    playerLocArr[1][3] = ui->label_cell1_player4;
    playerLocArr[1][4] = ui->label_cell1_player5;
    playerLocArr[1][5] = ui->label_cell1_player6;
    playerLocArr[2][0] = ui->label_cell2_player1;
    playerLocArr[2][1] = ui->label_cell2_player2;
    playerLocArr[2][2] = ui->label_cell2_player3;
    playerLocArr[2][3] = ui->label_cell2_player4;
    playerLocArr[2][4] = ui->label_cell2_player5;
    playerLocArr[2][5] = ui->label_cell2_player6;
    playerLocArr[3][0] = ui->label_cell3_player1;
    playerLocArr[3][1] = ui->label_cell3_player2;
    playerLocArr[3][2] = ui->label_cell3_player3;
    playerLocArr[3][3] = ui->label_cell3_player4;
    playerLocArr[3][4] = ui->label_cell3_player5;
    playerLocArr[3][5] = ui->label_cell3_player6;
    playerLocArr[4][0] = ui->label_cell4_player1;
    playerLocArr[4][1] = ui->label_cell4_player2;
    playerLocArr[4][2] = ui->label_cell4_player3;
    playerLocArr[4][3] = ui->label_cell4_player4;
    playerLocArr[4][4] = ui->label_cell4_player5;
    playerLocArr[4][5] = ui->label_cell4_player6;
    playerLocArr[5][0] = ui->label_cell5_player1;
    playerLocArr[5][1] = ui->label_cell5_player2;
    playerLocArr[5][2] = ui->label_cell5_player3;
    playerLocArr[5][3] = ui->label_cell5_player4;
    playerLocArr[5][4] = ui->label_cell5_player5;
    playerLocArr[5][5] = ui->label_cell5_player6;
    playerLocArr[6][0] = ui->label_cell6_player1;
    playerLocArr[6][1] = ui->label_cell6_player2;
    playerLocArr[6][2] = ui->label_cell6_player3;
    playerLocArr[6][3] = ui->label_cell6_player4;
    playerLocArr[6][4] = ui->label_cell6_player5;
    playerLocArr[6][5] = ui->label_cell6_player6;
    playerLocArr[7][0] = ui->label_cell7_player1;
    playerLocArr[7][1] = ui->label_cell7_player2;
    playerLocArr[7][2] = ui->label_cell7_player3;
    playerLocArr[7][3] = ui->label_cell7_player4;
    playerLocArr[7][4] = ui->label_cell7_player5;
    playerLocArr[7][5] = ui->label_cell7_player6;
    playerLocArr[8][0] = ui->label_cell8_player1;
    playerLocArr[8][1] = ui->label_cell8_player2;
    playerLocArr[8][2] = ui->label_cell8_player3;
    playerLocArr[8][3] = ui->label_cell8_player4;
    playerLocArr[8][4] = ui->label_cell8_player5;
    playerLocArr[8][5] = ui->label_cell8_player6;
    playerLocArr[9][0] = ui->label_cell9_player1;
    playerLocArr[9][1] = ui->label_cell9_player2;
    playerLocArr[9][2] = ui->label_cell9_player3;
    playerLocArr[9][3] = ui->label_cell9_player4;
    playerLocArr[9][4] = ui->label_cell9_player5;
    playerLocArr[9][5] = ui->label_cell9_player6;
    playerLocArr[10][0] = ui->label_cell10_player1;
    playerLocArr[10][1] = ui->label_cell10_player2;
    playerLocArr[10][2] = ui->label_cell10_player3;
    playerLocArr[10][3] = ui->label_cell10_player4;
    playerLocArr[10][4] = ui->label_cell10_player5;
    playerLocArr[10][5] = ui->label_cell10_player6;
    playerLocArr[11][0] = ui->label_cell11_player1;
    playerLocArr[11][1] = ui->label_cell11_player2;
    playerLocArr[11][2] = ui->label_cell11_player3;
    playerLocArr[11][3] = ui->label_cell11_player4;
    playerLocArr[11][4] = ui->label_cell11_player5;
    playerLocArr[11][5] = ui->label_cell11_player6;
    playerLocArr[12][0] = ui->label_cell12_player1;
    playerLocArr[12][1] = ui->label_cell12_player2;
    playerLocArr[12][2] = ui->label_cell12_player3;
    playerLocArr[12][3] = ui->label_cell12_player4;
    playerLocArr[12][4] = ui->label_cell12_player5;
    playerLocArr[12][5] = ui->label_cell12_player6;
    playerLocArr[13][0] = ui->label_cell13_player1;
    playerLocArr[13][1] = ui->label_cell13_player2;
    playerLocArr[13][2] = ui->label_cell13_player3;
    playerLocArr[13][3] = ui->label_cell13_player4;
    playerLocArr[13][4] = ui->label_cell13_player5;
    playerLocArr[13][5] = ui->label_cell13_player6;
    playerLocArr[14][0] = ui->label_cell14_player1;
    playerLocArr[14][1] = ui->label_cell14_player2;
    playerLocArr[14][2] = ui->label_cell14_player3;
    playerLocArr[14][3] = ui->label_cell14_player4;
    playerLocArr[14][4] = ui->label_cell14_player5;
    playerLocArr[14][5] = ui->label_cell14_player6;
    playerLocArr[15][0] = ui->label_cell15_player1;
    playerLocArr[15][1] = ui->label_cell15_player2;
    playerLocArr[15][2] = ui->label_cell15_player3;
    playerLocArr[15][3] = ui->label_cell15_player4;
    playerLocArr[15][4] = ui->label_cell15_player5;
    playerLocArr[15][5] = ui->label_cell15_player6;
    playerLocArr[16][0] = ui->label_cell16_player1;
    playerLocArr[16][1] = ui->label_cell16_player2;
    playerLocArr[16][2] = ui->label_cell16_player3;
    playerLocArr[16][3] = ui->label_cell16_player4;
    playerLocArr[16][4] = ui->label_cell16_player5;
    playerLocArr[16][5] = ui->label_cell16_player6;
    playerLocArr[17][0] = ui->label_cell17_player1;
    playerLocArr[17][1] = ui->label_cell17_player2;
    playerLocArr[17][2] = ui->label_cell17_player3;
    playerLocArr[17][3] = ui->label_cell17_player4;
    playerLocArr[17][4] = ui->label_cell17_player5;
    playerLocArr[17][5] = ui->label_cell17_player6;
    playerLocArr[18][0] = ui->label_cell18_player1;
    playerLocArr[18][1] = ui->label_cell18_player2;
    playerLocArr[18][2] = ui->label_cell18_player3;
    playerLocArr[18][3] = ui->label_cell18_player4;
    playerLocArr[18][4] = ui->label_cell18_player5;
    playerLocArr[18][5] = ui->label_cell18_player6;
    playerLocArr[19][0] = ui->label_cell19_player1;
    playerLocArr[19][1] = ui->label_cell19_player2;
    playerLocArr[19][2] = ui->label_cell19_player3;
    playerLocArr[19][3] = ui->label_cell19_player4;
    playerLocArr[19][4] = ui->label_cell19_player5;
    playerLocArr[19][5] = ui->label_cell19_player6;
    playerLocArr[20][0] = ui->label_cell20_player1;
    playerLocArr[20][1] = ui->label_cell20_player2;
    playerLocArr[20][2] = ui->label_cell20_player3;
    playerLocArr[20][3] = ui->label_cell20_player4;
    playerLocArr[20][4] = ui->label_cell20_player5;
    playerLocArr[20][5] = ui->label_cell20_player6;
    playerLocArr[21][0] = ui->label_cell21_player1;
    playerLocArr[21][1] = ui->label_cell21_player2;
    playerLocArr[21][2] = ui->label_cell21_player3;
    playerLocArr[21][3] = ui->label_cell21_player4;
    playerLocArr[21][4] = ui->label_cell21_player5;
    playerLocArr[21][5] = ui->label_cell21_player6;
    playerLocArr[22][0] = ui->label_cell22_player1;
    playerLocArr[22][1] = ui->label_cell22_player2;
    playerLocArr[22][2] = ui->label_cell22_player3;
    playerLocArr[22][3] = ui->label_cell22_player4;
    playerLocArr[22][4] = ui->label_cell22_player5;
    playerLocArr[22][5] = ui->label_cell22_player6;
    playerLocArr[23][0] = ui->label_cell23_player1;
    playerLocArr[23][1] = ui->label_cell23_player2;
    playerLocArr[23][2] = ui->label_cell23_player3;
    playerLocArr[23][3] = ui->label_cell23_player4;
    playerLocArr[23][4] = ui->label_cell23_player5;
    playerLocArr[23][5] = ui->label_cell23_player6;
    playerLocArr[24][0] = ui->label_cell24_player1;
    playerLocArr[24][1] = ui->label_cell24_player2;
    playerLocArr[24][2] = ui->label_cell24_player3;
    playerLocArr[24][3] = ui->label_cell24_player4;
    playerLocArr[24][4] = ui->label_cell24_player5;
    playerLocArr[24][5] = ui->label_cell24_player6;
    playerLocArr[25][0] = ui->label_cell25_player1;
    playerLocArr[25][1] = ui->label_cell25_player2;
    playerLocArr[25][2] = ui->label_cell25_player3;
    playerLocArr[25][3] = ui->label_cell25_player4;
    playerLocArr[25][4] = ui->label_cell25_player5;
    playerLocArr[25][5] = ui->label_cell25_player6;
    playerLocArr[26][0] = ui->label_cell26_player1;
    playerLocArr[26][1] = ui->label_cell26_player2;
    playerLocArr[26][2] = ui->label_cell26_player3;
    playerLocArr[26][3] = ui->label_cell26_player4;
    playerLocArr[26][4] = ui->label_cell26_player5;
    playerLocArr[26][5] = ui->label_cell26_player6;
    playerLocArr[27][0] = ui->label_cell27_player1;
    playerLocArr[27][1] = ui->label_cell27_player2;
    playerLocArr[27][2] = ui->label_cell27_player3;
    playerLocArr[27][3] = ui->label_cell27_player4;
    playerLocArr[27][4] = ui->label_cell27_player5;
    playerLocArr[27][5] = ui->label_cell27_player6;
    playerLocArr[28][0] = ui->label_cell28_player1;
    playerLocArr[28][1] = ui->label_cell28_player2;
    playerLocArr[28][2] = ui->label_cell28_player3;
    playerLocArr[28][3] = ui->label_cell28_player4;
    playerLocArr[28][4] = ui->label_cell28_player5;
    playerLocArr[28][5] = ui->label_cell28_player6;
    playerLocArr[29][0] = ui->label_cell29_player1;
    playerLocArr[29][1] = ui->label_cell29_player2;
    playerLocArr[29][2] = ui->label_cell29_player3;
    playerLocArr[29][3] = ui->label_cell29_player4;
    playerLocArr[29][4] = ui->label_cell29_player5;
    playerLocArr[29][5] = ui->label_cell29_player6;
    playerLocArr[30][0] = ui->label_cell30_player1;
    playerLocArr[30][1] = ui->label_cell30_player2;
    playerLocArr[30][2] = ui->label_cell30_player3;
    playerLocArr[30][3] = ui->label_cell30_player4;
    playerLocArr[30][4] = ui->label_cell30_player5;
    playerLocArr[30][5] = ui->label_cell30_player6;
    playerLocArr[31][0] = ui->label_cell31_player1;
    playerLocArr[31][1] = ui->label_cell31_player2;
    playerLocArr[31][2] = ui->label_cell31_player3;
    playerLocArr[31][3] = ui->label_cell31_player4;
    playerLocArr[31][4] = ui->label_cell31_player5;
    playerLocArr[31][5] = ui->label_cell31_player6;
    playerLocArr[32][0] = ui->label_cell32_player1;
    playerLocArr[32][1] = ui->label_cell32_player2;
    playerLocArr[32][2] = ui->label_cell32_player3;
    playerLocArr[32][3] = ui->label_cell32_player4;
    playerLocArr[32][4] = ui->label_cell32_player5;
    playerLocArr[32][5] = ui->label_cell32_player6;
    playerLocArr[33][0] = ui->label_cell33_player1;
    playerLocArr[33][1] = ui->label_cell33_player2;
    playerLocArr[33][2] = ui->label_cell33_player3;
    playerLocArr[33][3] = ui->label_cell33_player4;
    playerLocArr[33][4] = ui->label_cell33_player5;
    playerLocArr[33][5] = ui->label_cell33_player6;
    playerLocArr[34][0] = ui->label_cell34_player1;
    playerLocArr[34][1] = ui->label_cell34_player2;
    playerLocArr[34][2] = ui->label_cell34_player3;
    playerLocArr[34][3] = ui->label_cell34_player4;
    playerLocArr[34][4] = ui->label_cell34_player5;
    playerLocArr[34][5] = ui->label_cell34_player6;
    playerLocArr[35][0] = ui->label_cell35_player1;
    playerLocArr[35][1] = ui->label_cell35_player2;
    playerLocArr[35][2] = ui->label_cell35_player3;
    playerLocArr[35][3] = ui->label_cell35_player4;
    playerLocArr[35][4] = ui->label_cell35_player5;
    playerLocArr[35][5] = ui->label_cell35_player6;
    playerLocArr[36][0] = ui->label_cell36_player1;
    playerLocArr[36][1] = ui->label_cell36_player2;
    playerLocArr[36][2] = ui->label_cell36_player3;
    playerLocArr[36][3] = ui->label_cell36_player4;
    playerLocArr[36][4] = ui->label_cell36_player5;
    playerLocArr[36][5] = ui->label_cell36_player6;
    playerLocArr[37][0] = ui->label_cell37_player1;
    playerLocArr[37][1] = ui->label_cell37_player2;
    playerLocArr[37][2] = ui->label_cell37_player3;
    playerLocArr[37][3] = ui->label_cell37_player4;
    playerLocArr[37][4] = ui->label_cell37_player5;
    playerLocArr[37][5] = ui->label_cell37_player6;
    playerLocArr[38][0] = ui->label_cell38_player1;
    playerLocArr[38][1] = ui->label_cell38_player2;
    playerLocArr[38][2] = ui->label_cell38_player3;
    playerLocArr[38][3] = ui->label_cell38_player4;
    playerLocArr[38][4] = ui->label_cell38_player5;
    playerLocArr[38][5] = ui->label_cell38_player6;
    playerLocArr[39][0] = ui->label_cell39_player1;
    playerLocArr[39][1] = ui->label_cell39_player2;
    playerLocArr[39][2] = ui->label_cell39_player3;
    playerLocArr[39][3] = ui->label_cell39_player4;
    playerLocArr[39][4] = ui->label_cell39_player5;
    playerLocArr[39][5] = ui->label_cell39_player6;

}

std::string MainWindow::getAbsAvePlotPath(std::string avePlotName) {
    std::string relativePath = "../../../plots/Ave Plots/" + avePlotName;
    std::string dirPath = __FILE__;
    string currFileName = _getCurrFilename();
    dirPath = dirPath.substr(0, dirPath.size() - currFileName.size());
    std::string absolutePath = dirPath + relativePath;
    return absolutePath;
}

string MainWindow::_getProjectAbsPath() {
    std::string relativePath = "../../../";
    std::string dirPath = __FILE__;
    string currFileName = _getCurrFilename();
    dirPath = dirPath.substr(0, dirPath.size() - currFileName.size());
    std::string absolutePath = dirPath + relativePath;
    return absolutePath;
}

string MainWindow::_getCurrFilename() {
    const char* path = __FILE__;
    const char* filename = std::strrchr(path, '/');
    if (filename) {
        ++filename;
    } else {
        filename = path;
    }
    return filename;
}








