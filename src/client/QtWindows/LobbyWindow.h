#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QDialog>
#include <QAudioOutput>
#include <QMediaPlayer>

#include<vector>
#include "../../common/game_state/Game.h"
#include "../../common/game_state/player/Player.hpp"
#include "ActingWindow.h"

namespace Ui {
class LobbyWindow;
}

class LobbyWindow : public QDialog, public ActingWindow
{
    Q_OBJECT

public:
    explicit LobbyWindow(Game *gamestate, QWidget *parent = nullptr, QString name = "", QString ip = "",
                         QString port = "", Player *currPlayer = nullptr);
    ~LobbyWindow();

    virtual void parseResponse(const std::string message) override;

private slots:
    void on_buttonExit_clicked();
    void on_buttonStartGame_clicked();


private:
    Game* game;
    Player* _currPlayer;
    QString name;
    QString ip;
    QString port;
    std::vector<QString> playerLs;
    Ui::LobbyWindow *ui;


    // Abs file related functions
    static string _getProjectAbsPath();
    static string _getCurrFilename();

    void _updatePlayerLs();
    void _setupWindow();

    // Variable to handle mainloop for input, working with ActingWindow::_needUpdate
    QTimer* _mainloopTimer;
    void _mainloop();

    // BGM objects
    QMediaPlayer* _mediaPlayer;
    QAudioOutput* _audioOutput;

};

#endif // LOBBYWINDOW_H
