#include "answerrouteinformation.h"
#include "ui_answerrouteinformation.h"

AnswerRouteInformation::AnswerRouteInformation(Database* database, QString routeName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnswerRouteInformation)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(1280,960);

    this->routeName = routeName;

    // sample output for QLabel
    ui->Title->setText(routeName);

    // process call to route information
    routeInformation(routeName.toStdString());

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

void AnswerRouteInformation::routeInformation(const string& routeName)
{
    string dPath = "_IMAGES";
    string imgMap;
    string imgTime;
    try{
        for (const auto& file : filesystem::directory_iterator(dPath)) {
            if (file.is_regular_file()) {
                string filename = file.path().filename().string();

                // Check for map and time images based on routeName
                if (filename == routeName + "-MAP.jpg") {
                    imgMap = file.path().string();
                    qDebug() << "Found -MAP";
                } else if (filename == routeName + "-TIME.jpg") {
                    imgTime = file.path().string();
                    qDebug() << "Found -TIME";
                }
            }
        }
    } catch (const exception& ex) {
        qInfo() << ex.what();
        return;
    }

    setImage(QString::fromStdString(imgMap), QString::fromStdString(imgTime));
}

void AnswerRouteInformation::setImage(const QString& imgMap, const QString& imgTime) {

    QPixmap routeMap(imgMap);
    routeMap = routeMap.scaled(ui->Map->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    ui->Map->setPixmap(routeMap);
    ui->Map->setMask(routeMap.mask());

    QPixmap routeTimetable(imgTime);
    routeTimetable = routeTimetable.scaled(ui->Timetable->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    ui->Timetable->setPixmap(routeTimetable);
    ui->Timetable->setMask(routeTimetable.mask());
}

