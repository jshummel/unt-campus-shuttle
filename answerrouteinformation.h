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

    void routeInformation(const string& routeName);

    void setImage(const QString& imgMap, const QString& imgTime);

    QString routeName;

private slots:
    void on_Back_clicked();

private:
    Ui::AnswerRouteInformation *ui;

    Database* database;
};

#endif // ANSWERROUTEINFORMATION_H
