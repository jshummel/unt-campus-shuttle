#ifndef ANSWERROUTESEARCH_H
#define ANSWERROUTESEARCH_H

#include <QWidget>
#include "database.h"

namespace Ui {
class AnswerRouteSearch;
}

class AnswerRouteSearch : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerRouteSearch(Database* database, QString currentLocation, QString currentTime, QString destination, QWidget *parent = nullptr);
    ~AnswerRouteSearch();

    void routeSearch(const string& start,const string& end,const string& time);

    int convertTimeToInt(const string& timeStr);

    string findClosestTimeAfter(const vector<string>& times, const string& givenTime);

    void printTransfer(const vector<pair<string, string>>& recommendedSchedule, const string& recommendedStartRoute, const string& recommendedEndRoute);

    void printRoute(const vector<pair<string, string>>& recommendedSchedule, const string& recommendedRoute);

private slots:
    void on_Back_clicked();

    void on_StartRoute_clicked();

    void on_EndRoute_clicked();

    void on_RecRoute_clicked();

private:
    Ui::AnswerRouteSearch *ui;

    QString currentLocation, currentTime, destination;

    Database* database;

    vector<pair<string, string>> recommendedSchedule;

    string recommendedRoute;

    string recommendedStartRoute;

    string recommendedEndRoute;
};

#endif // ANSWERROUTESEARCH_H
