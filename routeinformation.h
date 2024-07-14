#ifndef ROUTEINFORMATION_H
#define ROUTEINFORMATION_H

#include <QWidget>
#include "database.h"

namespace Ui {
class RouteInformation;
}

class RouteInformation : public QWidget
{
    Q_OBJECT

public:
    explicit RouteInformation(Database* database, QWidget *parent = nullptr);
    ~RouteInformation();

private slots:
    void on_Back_clicked();

    void on_Submit_clicked();

private:
    Ui::RouteInformation *ui;

    Database* database;
};

#endif // ROUTEINFORMATION_H
