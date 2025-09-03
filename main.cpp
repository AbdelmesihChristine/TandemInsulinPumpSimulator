#include "MainWindow.h"
#include <QApplication>

/**
 * @brief The entry point of the Qt application.
 * Creates a QApplication object and shows the MainWindow.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create and show our main window, which holds the pump UI.
    MainWindow w;
    w.show();

    // Start Qt's event loop.
    return a.exec();
}
