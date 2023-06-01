#include "PopupWindow.h"
#include "ui_PopupWindow.h"
#include "QMessageBox"

#include "../ClientCommunicationToolbox/ClientCommunicationToolbox.h"

PopupWindow::PopupWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PopupWindow)
{
    ui->setupUi(this);


}

PopupWindow::~PopupWindow()
{
    delete ui;
}



PopupWindow::PopupWindow(const QString picturePath,
                         const QString aveName,
                         const QString ownerName,
                         const int price,
                         const int numBuilding, QWidget *parent) : QDialog(parent),ui(new Ui::PopupWindow)
{
    ui->setupUi(this);

//    QString picPathprefix = "://../../../plots/Ave Plots/";
//    QString picSuffix = ".png";

//    QString picPath = picPathprefix + QString::number(pictureNumber) + picSuffix;
    QPixmap pixmap(picturePath);
    ui->avePlot->setPixmap(pixmap);
    ui->avePlot->show();

    QFont font;
    font.setWeight(QFont::Bold);
    font.setPointSize(24);
    this->_price = price;
    this->aveName = aveName;
    this->ownerName = ownerName;
    this->_numBuilding = numBuilding;


    ui->aveName_input->setFont(font);
    ui->aveOwner_input->setFont(font);
    ui->aveNumBuilding_input->setFont(font);
    ui->avePrice_input->setFont(font);

    ui->aveName_input->setText(this->aveName);
    ui->aveOwner_input->setText(this->ownerName);
    ui->aveNumBuilding_input->setText(QString::number(this->_numBuilding));
    ui->avePrice_input->setText("$" + QString::number(this->_price));


}

void PopupWindow::on_buttonBuy_clicked()
{
//    QMessageBox::information(this, "popup", QString("Buy Property"));
    bool success = ClientCommunicationToolbox::buyProperty(_gameID, _playerID, _cellID);
    if (success) cout << "successfully buy the property" << endl;
    this->close();
}


void PopupWindow::on_buttonSell_clicked()
{
    bool success = ClientCommunicationToolbox::sellProperty(_gameID, _playerID, _cellID);
    if (success) cout << "successfully buy the property" << endl;
    this->close();
}


void PopupWindow::on_buttonAddBuilding_clicked()
{
    bool success = ClientCommunicationToolbox::buyBuilding(_gameID, _playerID, _cellID);
    if (success) cout << "successfully buy the building" << endl;
    this->close();
}


void PopupWindow::on_buttonRemoveBuilding_clicked()
{
    bool success = ClientCommunicationToolbox::sellBuilding(_gameID, _playerID, _cellID);
    if (success) cout << "successfully sell the building" << endl;
    this->close();
}

void PopupWindow::setCellID(std::string cellID) {_cellID = cellID;}
void PopupWindow::setGameID(std::string gameID) {_gameID = gameID;}
void PopupWindow::setPlayerID(std::string palyerID) {_playerID = palyerID;}

