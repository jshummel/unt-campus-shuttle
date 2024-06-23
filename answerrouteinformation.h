#ifndef ANSWERROUTEINFORMATION_H
#define ANSWERROUTEINFORMATION_H

#include <QWidget>
#include "database.h"

namespace Ui {
class AnswerRouteInformation;
}

class AnswerRouteInformation : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerRouteInformation(Database* database,QString routeName, QWidget *parent = nullptr);
    ~AnswerRouteInformation();

    // needs to be implemented
    void routeInformation();

private slots:
    void on_Back_clicked();

private:
    Ui::AnswerRouteInformation *ui;

    QString routeName;
    Database* database;
};

#endif // ANSWERROUTEINFORMATION_H
