#include "answerroutesearch.h"
#include "ui_answerroutesearch.h"

AnswerRouteSearch::AnswerRouteSearch(Database* database, QString currentLocation, QString currentTime, QString destination, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnswerRouteSearch)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(800,600);

    this->currentLocation = currentLocation;
    this->currentTime = currentTime;
    this->destination = destination;

    // process call to route search
    recommendedRoute();
}

AnswerRouteSearch::~AnswerRouteSearch()
{
    delete ui;
}

void AnswerRouteSearch::on_Back_clicked()
{
    qDebug("on_AnswerRouteSearch_Back_clicked");
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    this->close();
}

void AnswerRouteSearch::recommendedRoute()
{
    // route search

    // sample output for QLabel
    ui->Text1->setText(currentLocation);
    ui->Text2->setText(currentTime);
    ui->Text3->setText(destination);
}
