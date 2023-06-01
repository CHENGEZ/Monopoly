#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QDialog>
#include <QPixmap>
#include <iostream>


using namespace std;


namespace Ui {
    class PopupWindow;
}

class PopupWindow : public QDialog
{
Q_OBJECT

public:
    explicit PopupWindow(QWidget *parent = nullptr);
    PopupWindow(QString picturePath,
                QString aveName,
                QString ownerName,
                int price,
                int numBuilding = 0,
                QWidget* parent = nullptr);
    Ui::PopupWindow *ui;
    ~PopupWindow();

    void setGameID(string gameID);
    void setPlayerID(string palyerID);
    void setCellID(string cellID);

private slots:
    void on_buttonBuy_clicked();

    void on_buttonSell_clicked();

    void on_buttonAddBuilding_clicked();

    void on_buttonRemoveBuilding_clicked();

private:
    int _price;
    string _gameID;
    string _playerID;
    string _cellID;
    int _numBuilding;
    QString aveName;
    QString ownerName;
};

#endif // POPUPWINDOW_H
