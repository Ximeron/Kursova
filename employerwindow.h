#ifndef EMPLOYERWINDOW_H
#define EMPLOYERWINDOW_H

#include "qtableview.h"
#include <QWidget>
#include <QSqlTableModel>

namespace Ui
{
class EmployerWindow;
}

class EmployerWindow : public QWidget
{
        Q_OBJECT

    public:
        explicit EmployerWindow(QWidget* parent = nullptr);
        ~EmployerWindow();
    private slots:
        void on_addVacancyButton_clicked();
        void on_moveToActsButton_clicked();
        void on_deleteVacancyButton_clicked();

        void on_tabWidget_tabBarClicked(int index);

    private:
        Ui::EmployerWindow* ui;
        void loadApplications();
        void setupTableWidget();
        bool vacancyExists(const QString& vacancyName, int employerId);
        QSqlDatabase db;
        QSqlTableModel *model;
        void vacLoad();

};

#endif // EMPLOYERWINDOW_H
