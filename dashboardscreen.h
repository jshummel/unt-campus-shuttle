#ifndef DASHBOARDSCREEN_H
#define DASHBOARDSCREEN_H

#include <QMainWindow>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DashboardScreen;
}
QT_END_NAMESPACE

class DashboardScreen : public QMainWindow
{
    Q_OBJECT

public:
    DashboardScreen(Database* database, QWidget *parent = nullptr);
    ~DashboardScreen();

private slots:
    void on_Quit_clicked();

    void on_RouteSearch_clicked();

    void on_RouteInformation_clicked();

private:
    Ui::DashboardScreen *ui;
    Database* database;
};
#endif // DASHBOARDSCREEN_H
