#include "answerrouteinformation.h"
#include "ui_answerrouteinformation.h"

AnswerRouteInformation::AnswerRouteInformation(Database* database, QString routeName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnswerRouteInformation)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(800,600);

    this->routeName = routeName;

    // sample output for QLabel
    ui->Title->setText(routeName);

    // process call to route information
    routeInformation();
}

AnswerRouteInformation::~AnswerRouteInformation()
{
    delete ui;
}

void AnswerRouteInformation::on_Back_clicked()
{
    qDebug("on_AnswerRouteInformation_Back_clicked");
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    this->close();
}

void AnswerRouteInformation::routeInformation()
{
    // route search
}
