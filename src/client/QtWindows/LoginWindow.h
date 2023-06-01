#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>
#include <Qt>


#include "LobbyWindow.h"
#include "ActingWindow.h"

using namespace std;

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow, ActingWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    virtual void parseResponse(const std::string message) override;

private slots:
    void on_buttonConnect_clicked();
    void on_buttonExit_clicked();

private:
    Ui::LoginWindow *ui;

    // Abs File related functions
    static string _getProjectAbsPath();
    static string _getCurrFilename();

    // Player related information
    string _host;
    int _port;
    string _name;
    Player* _currPlayer;

    bool checkInputValid() const;

    // Things for main loop
    QTimer* _mainloopTimer;
    void _mainloop();

    // BGM Objects
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
};

#endif // LOGINWINDOW_H
