#include "dashboardscreen.h"
#include <QApplication>
#include "database.h"

int main(int argc, char *argv[])
{
    Database* databases = new Database();
    //tests to make sure db initializes - works as of 06_13v4
    //databases->printList(databases->allBusStopNames);

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    DashboardScreen dashboard(databases);
    dashboard.show();
    return a.exec();
}
