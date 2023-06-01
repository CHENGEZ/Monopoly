#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include "../../common/game_state/Game.h"
#include "ActingWindow.h"


using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ActingWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(Player* currPlayer, Game* game, QWidget* parent = nullptr);
    ~MainWindow();

    virtual void parseResponse(const std::string message) override;

private slots:

    // Functional button clicked function
    void on_buttonNextTurn_clicked();
    void on_buttonRollDice_clicked();

    // All the on-board button clicked function
    void on_pushButton_cell1_clicked();
    void on_pushButton_cell3_clicked();
    void on_pushButton_cell5_clicked();
    void on_pushButton_cell6_clicked();
    void on_pushButton_cell8_clicked();
    void on_pushButton_cell9_clicked();
    void on_pushButton_cell11_clicked();
    void on_pushButton_cell12_clicked();
    void on_pushButton_cell13_clicked();
    void on_pushButton_cell14_clicked();
    void on_pushButton_cell16_clicked();
    void on_pushButton_cell18_clicked();
    void on_pushButton_cell19_clicked();
    void on_pushButton_cell21_clicked();
    void on_pushButton_cell23_clicked();
    void on_pushButton_cell24_clicked();
    void on_pushButton_cell25_clicked();
    void on_pushButton_cell26_clicked();
    void on_pushButton_cell27_clicked();
    void on_pushButton_cell28_clicked();
    void on_pushButton_cell29_clicked();
    void on_pushButton_cell31_clicked();
    void on_pushButton_cell32_clicked();
    void on_pushButton_cell34_clicked();
    void on_pushButton_cell35_clicked();
    void on_pushButton_cell37_clicked();
    void on_pushButton_cell39_clicked();

    void on_pushButton_cell15_clicked();

    void on_pushButton_jailFreeCard_clicked();

private:
    Ui::MainWindow *ui;

    // Function to get abs path for a given plot name
    static std::string getAbsAvePlotPath(std::string avePlotName);

    // Abs file related functions
    static string _getProjectAbsPath();
    static string _getCurrFilename();

    // Initialize main_board slots
    void _initializeLocArr();
    void _initNameArr();
    void _initBalanceArr();
    void _initColorArr();

    // Game state information
    Game* game;

    // Indicate who is playing in this client terminal
    Player* currPlayer;

    // Location array for players
    QLabel*** playerLocArr;
    QLabel** _playerNameLabelArr;
    QLabel** _playerBalanceLabelArr;
    QLabel** _playerOutJailLabelArr;

    // Color array for players
    QString _colorArr[6];

    // BGM Objects
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

    // Variable to handle mainloop
    QTimer* _mainloopTimer;
    void _mainloop();

    // update ui and its helper function
    void _updateUI();
    void _updateGameState();
    void _updatePlayerLoc();
    void _clearUI();

};

#endif // MAINWINDOW_H
