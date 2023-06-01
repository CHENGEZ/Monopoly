#include "QtWindows/LoginWindow.h"
#include <QApplication>
#include "QtWindows/MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
//    qputenv("QT_SCALE_FACTOR", "0.5");
//    qputenv("QT_USE_PHYSICAL_DPI", "0.1");


    QApplication a(argc, argv);
    LoginWindow w;
    w.show();
    return a.exec();
}
