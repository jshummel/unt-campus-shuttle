#include "routesearch.h"
#include "ui_routesearch.h"
#include "answerroutesearch.h"

#include <QStringList>
#include <chrono>
#include <iomanip>
#include <sstream>


string RouteSearch::getCurrentTime(){
    // Get the current time using chrono
    auto now = chrono::system_clock::now();

    // Convert to time_t to get the current time in seconds since epoch
    time_t now_time = chrono::system_clock::to_time_t(now);

    // Convert to struct tm in local timezone
    tm time_info = *localtime(&now_time);

    // Format the time as HH:MM
    ostringstream time_stream;
    time_stream << put_time(&time_info, "%H:%M");

    return time_stream.str();
}

bool RouteSearch::isValidTime(const QString& timeStr){
    // Time format expected: HH:MM
    QRegularExpression regex("^([0-1]?[0-9]|2[0-3]):([0-5][0-9])$");
    QRegularExpressionMatch match = regex.match(timeStr);
    return match.hasMatch();
}


RouteSearch::RouteSearch(Database* database, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RouteSearch)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(1280,960);

    QStringList locations = {
        "14 FIFTY ONE",
        "BONNIE BRAE AT UNIVERSITY",
        "DISCOVERY PARK(MAIN)",
        "EAGLE AT BERNARD ST.",
        "EB EAGLE AT BERNARD ST.",
        "ESLC",
        "FOUTS TRANSFER",
        "GAB",
        "HICKORT AT AVE. H",
        "KERR HALL/BLB",
        "MEDPARK STATION",
        "NTX DENTON",
        "OAK AT AVE. G",
        "POHL RECREATION CENTER",
        "THE ARBORS",
        "THE ARBORS/UNIVERSITY PLACE",
        "THE ARCH",
        "THE FORUM/REDPOINT",
        "THE FORUM/REPUBLIC",
        "THE GATEWAY AT DENTON",
        "THE LEONARD",
        "THE OAKS",
        "THE RETREAT",
        "THE RIDGE",
        "THE VENUE AT BERNARD ST.",
        "THE VENUE AT LINDSEY ST.",
        "UNION TRANSFER",
        "UNIVERSITY PLACE",
        "UNIVERSITY UPTOWN",
        "VICTORY HALL",
        "WB OAK AT FRY",
        "WB OAK AT I-35",
        "WB OAK AT THOMAS"
    };

    ui->CurrentLocation->setPlaceholderText("Current Location");
    ui->CurrentLocation->addItems(locations);

    ui->Destination->setPlaceholderText("Destination");
    ui->Destination->addItems(locations);

    ui->CurrentTime->setPlaceholderText("Current Time");
    //ui->CurrentTime->setAlignment(Qt::AlignHCenter);



}

RouteSearch::~RouteSearch()
{
    delete ui;
}

void RouteSearch::on_Back_clicked()
{
    qDebug("on_RouteSearch_Back_clicked");
    this->close();
}

QString currentLocation, currentTime, destination;

void RouteSearch::on_Submit_clicked()
{
    qDebug("on_RouteSearch_Submit_clicked");
    bool validLocation = false;
    bool validDestination = false;
    bool validTime= false;


    currentLocation = ui->CurrentLocation->currentText();
    currentTime = ui->CurrentTime->text();
    destination = ui->Destination->currentText();

    //DEBUG OUTPUT
    // qDebug() << currentLocation;
    // qDebug() << currentTime;
    // qDebug() << destination;

    if(!currentLocation.isEmpty()){
        validLocation = true;
    } else{
        qDebug() << "Invalid current location";
    }

    if(!destination.isEmpty()){
        validDestination = true;
    } else {
        qDebug() << "Invalid destination";
    }

    if(isValidTime(currentTime)){
        validTime = true;
    } else {
        qDebug() << "Invalid time format or out of range";
    }


    // VALIDATE CURRENTTIME FORMAT (HH:MM)
    if (validTime && validLocation && validDestination) {
        AnswerRouteSearch *ars = new AnswerRouteSearch(database, currentLocation, currentTime, destination, this);
        ars->show();
        //sync dark mode switch

    }

    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    // ui->CurrentLocation->clear();
    // ui->CurrentTime->clear();
    // ui->Destination->clear();
}


void RouteSearch::on_TimeToggle_clicked(bool checked)
{
    if (checked){
        ui->TimeToggle->setText("Current Time: ON");
        ui->TimeToggle->setStyleSheet(" background-color: rgb(90,152,101); border: 5px solid; border-color: rgb(0, 0, 0); border-radius: 20px; font: 30pt 'Oswald'; color: white; ");
        QString time = QString::fromStdString(getCurrentTime());
        ui->CurrentTime->setText(time);
    } else {
        ui->TimeToggle->setText("Current Time: OFF");
        ui->TimeToggle->setStyleSheet(" background-color: rgb(39,100,54); border: 1px solid; border-color: rgb(0, 0, 0);  border-radius: 20px; font: 30pt 'Oswald'; color: white; ");
        ui->CurrentTime->clear();
        ui->CurrentTime->setPlaceholderText("Current Time");
    }
}

