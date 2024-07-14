#include "answerroutesearch.h"
#include "ui_answerroutesearch.h"
#include "answerrouteinformation.h"

#include <QString>
#include <string>

AnswerRouteSearch::AnswerRouteSearch(Database* database, QString currentLocation, QString currentTime, QString destination, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnswerRouteSearch)
    , database(database)
{
    ui->setupUi(this);
    // removes ability to resize window
    QWidget::setFixedSize(1280,960);

    //ALLOW LABELS TO EXPAND HORIZONTALLY                                           --DOESN'T WORK WTF.
    ui->Text1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->Text2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->Text3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->currentLocation = currentLocation;
    this->currentTime = currentTime;
    this->destination = destination;

    //CONVERT QSTRING VARS TO STRING VARS FOR PROCESSING
    string start = currentLocation.toStdString();
    string time = currentTime.toStdString();
    string end = destination.toStdString();

    // process call to route search
    routeSearch(start, end, time);

    if(recommendedStartRoute.empty()){
        ui->StartRoute->hide();
    } else {
        ui->StartRoute->show();
    }

    if(recommendedEndRoute.empty()){
        ui->EndRoute->hide();
    } else {
        ui->EndRoute->show();
    }

    if(recommendedRoute.empty()){
        ui->RecRoute->hide();
    } else {
        ui->RecRoute->show();
    }


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





int AnswerRouteSearch::convertTimeToInt(const string& timeStr) {
    int timeInt = 0;
    for (char c : timeStr)
        if (c != ';')
            timeInt = timeInt * 10 + (c - '0');
    return timeInt;
}





// Function to find the closest time after a given time
string AnswerRouteSearch::findClosestTimeAfter(const vector<string>& times, const string& givenTime) {
    int givenTimeInt = convertTimeToInt(givenTime);

    for (const auto& time : times) {
        if (time == givenTime) {
            return givenTime;
        }
    }

    int closestTimeInt = INT_MAX; //infinity
    string closestTime;

    for (const auto& time : times) {
        int currentTimeInt = convertTimeToInt(time);
        if (currentTimeInt > givenTimeInt && currentTimeInt < closestTimeInt) {
            closestTimeInt = currentTimeInt;
            closestTime = time;
        }
    }

    return closestTime;
}



//RECOMMENDED ROUTE/SCHEDULE VARIABLES : <STOPNAME, TIME>
vector<pair<string, string>> recommendedSchedule = {};
string recommendedRoute = "";
string recommendedStartRoute = "";
string recommendedEndRoute = "";

void AnswerRouteSearch::routeSearch(const string& start,const string& end,const string& time)
{
    // route search
    qDebug("recommendedRoute() called");
    qDebug() << start;
    qDebug() << time;
    qDebug() << end << "\n";


    //DETERMINE IF TRANSFER IS NEEDED FIRST
    bool transfer = false;
    vector<string> routesWithStartAndEnd;
    vector<string> routesWithStartStop;
    vector<string> routesWithEndStop;

    for (const auto& dbEntry : database->routeDB) {
        string routeName = dbEntry.first;
        unordered_map<string, vector<string>> routeStops = dbEntry.second;

        bool isStartStop = false;
        bool isEndStop = false;

        for (const auto& stopsEntry : routeStops) {
            string stopName = stopsEntry.first;

            if (stopName == start) {
                isStartStop = true;
                routesWithStartStop.push_back(routeName);
            }
            if (stopName == end) {
                isEndStop = true;
                routesWithEndStop.push_back(routeName);
            }
            if (isStartStop && isEndStop) {
                routesWithStartAndEnd.push_back(routeName);
                break;
            }
        }
    }

    if (routesWithStartAndEnd.empty()) {
        qInfo("\tTRANSFER REQUIRED\n");
        ui->Text1->setText("TRANSFER REQUIRED");
        transfer = true;
    }
    else {
        qInfo("\tTRANSFER NOT REQUIRED\n");
        ui->Text1->setText("TRANSFER NOT REQUIRED");
    }



    //IF TRANSFER IS NOT NEEDED
    if (!transfer) {
        //DETERMINE BEST ROUTE FROM POSSIBLE START/END ROUTES WITH NEAREST START TIME TO GIVEN INPUT TIME
        string routeWithBestStartTime = "";
        string bestStartTime = to_string(INT_MAX);

        qDebug("CYCLE THROUGH ROUTES WITH START AND END STOPS");
        for (string& route : routesWithStartAndEnd) {
            unordered_map<string, vector<string>> routeStops = database->routeDB.at(route);

            vector<string> stopTimes = routeStops.at(start);
            string closestTime = findClosestTimeAfter(stopTimes, time);
            string tempRoute = "";
            string tempTime = "";

            qDebug()<< route << " - " << closestTime;
            if (closestTime != "") {
                int int_closestTime = convertTimeToInt(closestTime);
                int int_testTime = convertTimeToInt(bestStartTime);
                if (int_closestTime < int_testTime) {
                    tempTime = bestStartTime;
                    bestStartTime = closestTime;
                    tempRoute = routeWithBestStartTime;
                    routeWithBestStartTime = route;
                }
                qDebug()<< "\tBEST: " << routeWithBestStartTime << " - " << bestStartTime;

                //CHECK IF END IS REACHABLE AT TIME OF START STOP
                stopTimes = routeStops.at(end);
                string closestEndTime = findClosestTimeAfter(stopTimes, closestTime);
                if (closestEndTime == "") {
                    qDebug("WALKED BACK BEST ROUTE CHOICE");
                    //routeWithBestStartTime.clear();
                    routeWithBestStartTime = tempRoute;
                    //bestStartTime = to_string(INT_MAX);
                    bestStartTime = tempTime;
                    qDebug()<< "\tBEST: " << routeWithBestStartTime << " - " << bestStartTime;
                }
            }
        }

        //IF NO ROUTE FOUND WITH GIVEN TIME, BUS SERVICE HAS ENDED FOR TODAY - RECALCULATE USING MIDNIGHT FOR TOMORROW
        if(routeWithBestStartTime == ""){
            qInfo("\t---NO ROUTE FOUND >> TIME CHANGED TO MIDNIGHT FOR TOMORROW PROCESSING---");
            routeSearch(start, end, "00:00");
            return;
        }

        qInfo() << "ROUTE WITH BEST START TIME " << routeWithBestStartTime << "\n";
        recommendedRoute = routeWithBestStartTime;

        //ADD <START, START TIME> TO RECOMMENDED SCHEDULE
        qDebug() << "recommendedSchedule.push_back(" << start << "," << bestStartTime << ")";
        recommendedSchedule.push_back(make_pair(start, bestStartTime));

        //ADD <END, END TIME> TO RECOMMENDED SCHEDULE IF IT EXISTS
        unordered_map<string, vector<string>> routeStops = database->routeDB.at(recommendedRoute);
        vector<string> stopTimes = routeStops.at(end);
        string closestTime = findClosestTimeAfter(stopTimes, bestStartTime);
        if (closestTime.empty()) {
            closestTime = " N/A ";
            qInfo("\t---NO VALID STOP TIMES SERVICING DESTINATION AT THIS TIME---");
            qDebug() << "recommendedSchedule.push_back(N/A,N/A)";
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            ui->Text1->setText("STOP TIME DNE");
            printRoute(recommendedSchedule, recommendedRoute);
            return;
            //PROBABLY A GOOD PLACE TO RETURN IF THIS IS A FUNCTION
        } else {
            qDebug() << "recommendedSchedule.push_back(" << end << "," << closestTime << ")";
            recommendedSchedule.push_back(make_pair(end, closestTime));
        }

        printRoute(recommendedSchedule, recommendedRoute);
    }



    //IF TRANSFER IS NEEDED
    //IF TRANSFER IS NEEDED
    /*
     *
     *
     *
     */
    //IF TRANSFER IS NEEDED
    //IF TRANSFER IS NEEDED



    if (transfer) {
        //DETERMINE BEST ROUTE FROM POSSIBLE START ROUTES WITH NEAREST START TIME TO GIVEN INPUT TIME
        string routeWithBestStartTime = "";
        string bestStartTime = to_string(INT_MAX);
        string departingTransferTime = "";

        qDebug("CYCLE THROUGH ROUTES WITH START STOP");
        for (string& route : routesWithStartStop) {
            unordered_map<string, vector<string>> routeStops = database->routeDB.at(route);

            vector<string> stopTimes = routeStops.at(start);
            string closestTime = findClosestTimeAfter(stopTimes, time);
            string tempRoute = "";
            string tempTime = "";

            qDebug() << route << " - " << closestTime;
            if (closestTime != "") {
                int int_closestTime = convertTimeToInt(closestTime);
                int int_testTime = convertTimeToInt(bestStartTime);
                if (int_closestTime < int_testTime) {
                    tempTime = bestStartTime;
                    bestStartTime = closestTime;
                    tempRoute = routeWithBestStartTime;
                    routeWithBestStartTime = route;
                }
                qDebug() << "\tBEST: " << routeWithBestStartTime << " - " << bestStartTime;

                //CHECK IF TRANSFER IS REACHABLE AT TIME OF START STOP
                stopTimes = routeStops.at("UNION TRANSFER");
                string closestEndTime = findClosestTimeAfter(stopTimes, closestTime);
                if (closestEndTime == "") {
                    qDebug("WALKED BACK BEST ROUTE CHOICE");
                    //routeWithBestStartTime.clear();
                    routeWithBestStartTime = tempRoute;
                    //bestStartTime = to_string(INT_MAX);
                    bestStartTime = tempTime;
                    qDebug() << "\tBEST: " << routeWithBestStartTime << " - " << bestStartTime;
                }
            }
        }

        //IF NO ROUTE FOUND WITH GIVEN TIME, BUS SERVICE HAS ENDED FOR TODAY - RECALCULATE USING MIDNIGHT FOR TOMORROW
        if(routeWithBestStartTime == ""){
            qDebug("\t---NO ROUTE FOUND >> TIME CHANGED TO MIDNIGHT FOR TOMORROW PROCESSING---");
            routeSearch(start, end, "00:00");
            return;
        }

        qInfo() << "ROUTE WITH BEST START TIME " << routeWithBestStartTime;
        recommendedStartRoute = routeWithBestStartTime;

        //ADD <START, START TIME> TO RECOMMENDED SCHEDULE
        qDebug() << "recommendedSchedule.push_back(" << start << "," << bestStartTime << ")";
        recommendedSchedule.push_back(make_pair(start, bestStartTime));

        //ADD <TRANSFER, TRANSFER DEPART TIME> TO RECOMMENDED SCHEDULE IF IT EXISTS
        unordered_map<string, vector<string>> startRouteStops = database->routeDB.at(recommendedStartRoute);
        vector<string> startStopTimes = startRouteStops.at("UNION TRANSFER");
        string closestTransferTime = findClosestTimeAfter(startStopTimes, bestStartTime);
        departingTransferTime = closestTransferTime;
        if (closestTransferTime.empty()) {
            closestTransferTime = " N/A ";
            qInfo("\t---NO VALID UNION TRANSFER DEPARTURE TIMES SERVICING START ROUTE AT THIS TIME---");
            qDebug() << "recommendedSchedule.push_back(N/A,N/A)";
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            ui->Text1->setText("TRANSFER DEPARTURE TIME DNE");
            printTransfer(recommendedSchedule, recommendedStartRoute, recommendedEndRoute);
            return;
            //PROBABLY A GOOD PLACE TO RETURN IF THIS IS A FUNCTION
        } else {
            qDebug() << "recommendedSchedule.push_back(UNION TRANSFER ," << closestTransferTime << ")\n\n\tTRANSFER\n";
            recommendedSchedule.push_back(make_pair("UNION TRANSFER", closestTransferTime));
        }



        /*
         *	TRANSFER BETWEEN ROUTES HAPPENS HERE
         */



        //DETERMINE BEST ROUTE FROM POSSIBLE END ROUTES WITH NEAREST TRANSFER TIME TO DEPARTING TRANSFER TIME
        string routeWithBestTransferTime = "";
        string bestTransferTime = to_string(INT_MAX);
        string closestTime = "";

        qDebug("CYCLE THROUGH ROUTES WITH END STOP");
        for (string& route : routesWithEndStop) {
            unordered_map<string, vector<string>> routeStops = database->routeDB.at(route);

            vector<string> stopTimes = routeStops.at("UNION TRANSFER");
            string tempRoute = "";
            string tempTime = "";
            closestTime = findClosestTimeAfter(stopTimes, departingTransferTime);

            qDebug() << route << " - " << closestTime;
            if (closestTime != "") {
                int int_closestTime = convertTimeToInt(closestTime);
                int int_testTime = convertTimeToInt(bestTransferTime);
                if (int_closestTime < int_testTime) {
                    tempTime = bestTransferTime;
                    bestTransferTime = closestTime;
                    tempRoute = routeWithBestStartTime;
                    routeWithBestTransferTime = route;
                }
                qDebug() << "\tBEST: " << routeWithBestTransferTime << " - " << bestTransferTime;

                //CHECK IF END IS REACHABLE AT TIME OF START STOP
                stopTimes = routeStops.at(end);
                string closestEndTime = findClosestTimeAfter(stopTimes, closestTime);
                if (closestEndTime == "") {
                    qDebug("WALKED BACK BEST ROUTE CHOICE");
                    //routeWithBestStartTime.clear();
                    routeWithBestStartTime = tempRoute;
                    //bestStartTime = to_string(INT_MAX);
                    bestTransferTime = tempTime;
                    qDebug() << "\tBEST: " << routeWithBestTransferTime << " - " << bestTransferTime;
                }
            }
        }
        qInfo()<< "ROUTE WITH BEST TRANSFER TIME " << routeWithBestTransferTime << "\n";
        if(!routeWithBestTransferTime.empty()){
            recommendedEndRoute = routeWithBestTransferTime;
        }

        //ADD <TRANSFER, TRANSFER ARRIVE TIME> TO RECOMMENDED SCHEDULE IF IT EXISTS
        if(bestTransferTime.empty()){
            qInfo("\t---NO VALID UNION TRANSFER ARRIVAL TIMES SERVICING DESTINATION ROUTE AT THIS TIME---");
            qDebug() << "recommendedSchedule.push_back(N/A,N/A)";
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            ui->Text1->setText("TRANSFER ARRIVAL TIME DNE");
            printTransfer(recommendedSchedule, recommendedStartRoute, recommendedEndRoute);
            return;
            //PROBABLY A GOOD PLACE TO RETURN IF THIS IS A FUNCTION
        } else{
            qDebug() << "recommendedSchedule.push_back(UNION TRANSFER ," << bestTransferTime << ")";
            recommendedSchedule.push_back(make_pair("UNION TRANSFER", bestTransferTime));
        }

        //ADD <END, END TIME> TO RECOMMENDED SCHEULE IF IT EXISTS
        unordered_map<string, vector<string>> endRouteStops = database->routeDB.at(recommendedEndRoute);
        vector<string> endStopTimes = endRouteStops.at(end);
        string closestEndTime = findClosestTimeAfter(endStopTimes, bestTransferTime);
        if (closestEndTime.empty()) {
            closestEndTime = " N/A ";
            qInfo("\t---NO VALID STOP TIMES SERVICING DESTINATION AT THIS TIME---");
            qDebug() << "recommendedSchedule.push_back(N/A,N/A)";
            recommendedSchedule.push_back(make_pair("N/A","N/A"));
            ui->Text1->setText("STOP TIME DNE AFTER TRANSFER");
            printTransfer(recommendedSchedule, recommendedStartRoute, recommendedEndRoute);
            return;
            //PROBABLY A GOOD PLACE TO RETURN IF THIS IS A FUNCTION
        } else{
            qDebug() << "recommendedSchedule.push_back(" << end << "," << closestEndTime << ")";
            recommendedSchedule.push_back(make_pair(end, closestEndTime));
        }

        printTransfer(recommendedSchedule, recommendedStartRoute, recommendedEndRoute);
    }
}

void AnswerRouteSearch::printTransfer(const vector<pair<string, string>>& recommendedSchedule, const string& recommendedStartRoute, const string& recommendedEndRoute){
    //PRINT RECOMMENDED SCHEDULE <<<CONSOLE DEBUG>>> AND LOAD QSTRING ARRAY FOR UI OUTPUT
    QString arr[4][2];
    int i = 0;
    int j = 0;
    for (const auto& entry : recommendedSchedule) {
        if (i == 0)
            qInfo() << recommendedStartRoute;
        if (i == 2)
            qInfo() << recommendedEndRoute;
        qInfo() << "\t" << entry.second << " - " << entry.first;
        QString qEntryFirst = QString::fromStdString(entry.first);
        QString qEntrySecond = QString::fromStdString(entry.second);
        arr[i][j] = qEntrySecond;
        arr[i][j+1] = qEntryFirst;
        i++;
    }

    //PRINT RECOMMENDED ROUTE TO UI
    QString routeName = QString::fromStdString(recommendedEndRoute);
    ui->Text3->setText(routeName);
    routeName = QString::fromStdString(recommendedStartRoute);
    ui->Text2->setText(routeName);
    routeName = routeName + QString::fromStdString(" → ");
    routeName = routeName + QString::fromStdString(recommendedEndRoute);
    ui->Title->setText(routeName);
    ui->Text2->setText(ui->Text2->text() + "\n" + arr[0][0] + " - " + arr[0][1] + "\n" + arr[1][0] + " - " + arr[1][1]);
    ui->Text3->setText(ui->Text3->text() + "\n" + arr[2][0] + " - " + arr[2][1] + "\n" + arr[3][0] + " - " + arr[3][1]);

    //SETUP BUTTONS
    ui->StartRoute->setText(QString::fromStdString(recommendedStartRoute));
    ui->EndRoute->setText(QString::fromStdString(recommendedEndRoute));
}


void AnswerRouteSearch::printRoute(const vector<pair<string, string>>& recommendedSchedule, const string& recommendedRoute){
    //PRINT RECOMMENDED SCHEDULE <<<CONSOLE DEBUG>>> AND LOAD QSTRING ARRAY FOR UI OUTPUT
    qInfo() << recommendedRoute;
    QString arr[2][2];
    int i = 0;
    int j = 0;
    for (const auto& entry : recommendedSchedule) {
        qInfo() << "\t" << entry.second << " - " << entry.first;
        QString qEntryFirst = QString::fromStdString(entry.first);
        QString qEntrySecond = QString::fromStdString(entry.second);
        arr[i][j] = qEntrySecond;
        arr[i][j+1] = qEntryFirst;
        i++;
    }

    //PRINT RECOMMENDED ROUTE TO UI
    QString routeName = QString::fromStdString(recommendedRoute);
    ui->Title->setText(routeName);
    ui->Text2->setText(routeName + "\n" + arr[0][0] + " - " + arr[0][1]);
    ui->Text3->setText(routeName + "\n" + arr[1][0] + " - " + arr[1][1]);

    //SETUP BUTTON
    ui->RecRoute->setText(QString::fromStdString(recommendedRoute));
}

void AnswerRouteSearch::on_StartRoute_clicked()
{
    qDebug("on_StartRoute_clicked");
    QString startRoute = QString::fromStdString(recommendedStartRoute);
    qDebug() << startRoute;
    AnswerRouteInformation *ari = new AnswerRouteInformation(database, startRoute, this);
    ari->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    //ui->RouteName->clear();
    //qDebug("RouteName cleared");
}


void AnswerRouteSearch::on_EndRoute_clicked()
{
    qDebug("on_EndRoute_clicked");
    QString endRoute = QString::fromStdString(recommendedEndRoute);
    qDebug() << endRoute;
    AnswerRouteInformation *ari = new AnswerRouteInformation(database, endRoute, this);
    ari->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    //ui->RouteName->clear();
    //qDebug("RouteName cleared");
}


void AnswerRouteSearch::on_RecRoute_clicked()
{
    qDebug("on_RecRoute_clicked");
    QString recRoute = QString::fromStdString(recommendedRoute);
    qDebug() << recRoute;
    AnswerRouteInformation *ari = new AnswerRouteInformation(database, recRoute, this);
    ari->show();
    //tests to make sure db crosses to this window - works as of 06_13v4
    //database->printList(database->allBusStopNames);
    //ui->RouteName->clear();
    //qDebug("RouteName cleared");
}

