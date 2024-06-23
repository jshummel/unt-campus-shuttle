#include "routeinformation.h"
#include "ui_routeinformation.h"
#include "answerrouteinformation.h"

RouteInformation::RouteInformation(Database* database, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RouteInformation)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(800,600);

    // adds and aligns placeholder text to QLineEdit
    ui->RouteName->setPlaceholderText("Route Name");
    ui->RouteName->setAlignment(Qt::AlignHCenter);
}

RouteInformation::~RouteInformation()
{
    delete ui;
}

void RouteInformation::on_Back_clicked()
{
    qDebug("on_RouteInformation_Back_clicked");
    this->close();
}

QString routeName;

void RouteInformation::on_Submit_clicked()
{
    qDebug("on_RouteInformation_Submit_clicked");
    routeName = ui->RouteName->text();
    AnswerRouteInformation *ari = new AnswerRouteInformation(database, routeName, this);
    ari->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    ui->RouteName->clear();
    qDebug("RouteName cleared");
}
