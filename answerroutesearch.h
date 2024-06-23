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

    // needs to be implemented
    void recommendedRoute();

private slots:
    void on_Back_clicked();

private:
    Ui::AnswerRouteSearch *ui;

    QString currentLocation, currentTime, destination;
    Database* database;
};

#endif // ANSWERROUTESEARCH_H
