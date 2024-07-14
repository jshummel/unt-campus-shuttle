#ifndef ROUTESEARCH_H
#define ROUTESEARCH_H

#include <QWidget>
#include "database.h"

namespace Ui {
class RouteSearch;
}

class RouteSearch : public QWidget
{
    Q_OBJECT

public:
    explicit RouteSearch(Database* database, QWidget *parent = nullptr);
    ~RouteSearch();

    string getCurrentTime();

    bool isValidTime(const QString& timeStr);

private slots:
    void on_Back_clicked();

    void on_Submit_clicked();

    void on_TimeToggle_clicked(bool checked);
 private:
    Ui::RouteSearch *ui;

    Database* database;
};

#endif // ROUTESEARCH_H
