#include <mainwindow.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qputenv("CF_DATADIR", "/Users/omergoktas/Desktop/datadir");

    MainWindow w;
    w.show();

    return a.exec();
}
