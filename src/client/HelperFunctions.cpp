//
// Created by Ricky on 2023/5/9.
//

#include <QMessageBox>
#include <QString>



static void showError(const std::string& errorMessage, const std::string& errTitle = "Error"){
    QMessageBox::information(nullptr, "Error", QString::fromStdString(errorMessage));
}

static void showMessage(const std::string& msg){
    QMessageBox::information(nullptr, "Message", QString::fromStdString(msg));
}
