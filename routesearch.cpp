#include "routesearch.h"
#include "ui_routesearch.h"
#include "answerroutesearch.h"

RouteSearch::RouteSearch(Database* database, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RouteSearch)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(800,600);

    // adds and aligns placeholder text to QLineEdit
    ui->CurrentLocation->setPlaceholderText("Current Location");
    ui->CurrentLocation->setAlignment(Qt::AlignHCenter);
    ui->CurrentTime->setPlaceholderText("Current Time");
    ui->CurrentTime->setAlignment(Qt::AlignHCenter);
    ui->Destination->setPlaceholderText("Destination");
    ui->Destination->setAlignment(Qt::AlignHCenter);
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
    currentLocation = ui->CurrentLocation->text();
    currentTime = ui->CurrentTime->text();
    destination = ui->Destination->text();
    AnswerRouteSearch *ars = new AnswerRouteSearch(database, currentLocation, currentTime, destination, this);
    ars->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    ui->CurrentLocation->clear();
    ui->CurrentTime->clear();
    ui->Destination->clear();
}
