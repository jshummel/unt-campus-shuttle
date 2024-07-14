#include "dashboardscreen.h"
#include "./ui_dashboardscreen.h"
#include "routesearch.h"
#include "routeinformation.h"
#include <QDebug>
#include <QPalette>
#include <QPixmap>

DashboardScreen::DashboardScreen(Database* database, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardScreen)
    , database(database)
    , darkModeEnabled(false)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(1280,960);

    // adds background image
    QPixmap bg("_IMAGES/bg.jpg");
    bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bg);
    this->setPalette(palette);
}

DashboardScreen::~DashboardScreen()
{
    delete ui;
}

void DashboardScreen::on_Quit_clicked()
{
    qDebug("on_Quit_clicked");
    qDebug("qApp->exit() called");
    qApp->exit();
}


void DashboardScreen::on_RouteSearch_clicked()
{
    qDebug("on_RouteSearch_clicked");
    RouteSearch *rs = new RouteSearch(database, this);
    rs->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);


}



void DashboardScreen::on_RouteInformation_clicked()
{
    qDebug("on_RouteInformation_clicked");
    RouteInformation *ri = new RouteInformation(database, this);
    ri->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
 }



void DashboardScreen::on_DarkMode_clicked() {
    QPalette palette;
    QPixmap bg("_IMAGES/bg.jpg");
    QImage image = bg.toImage();

    darkModeEnabled = !darkModeEnabled; // Toggle the dark mode state

    if (darkModeEnabled) {
        emit checked(true);

        qInfo() << "Dark mode enabled";
        for (int i = 0; i < image.width(); ++i) {
            for (int j = 0; j < image.height(); ++j) {
                QColor color = image.pixelColor(i, j);
                int hue = color.hue();
                color.setHsv(hue, color.saturation(), color.black(), color.alpha());
                image.setPixelColor(i, j, color);
            }
        }

        palette.setBrush(QPalette::Window, QPixmap::fromImage(image));
    } else {
        qInfo() << "Dark mode disabled";
        palette.setBrush(QPalette::Window, bg);
    }

    this->setPalette(palette);
}
