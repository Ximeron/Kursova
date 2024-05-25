#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui
{
class EmployeeWindow;
}

class EmployeeWindow : public QWidget
{
        Q_OBJECT

    public:
        explicit EmployeeWindow(QWidget* parent = nullptr);
        ~EmployeeWindow();
    private slots:
        void on_applyButton_clicked();
        void on_comboBoxEmployers_currentIndexChanged(int index);
        void on_exitButton_clicked();




    private:
        Ui::EmployeeWindow* ui;
        void loadEmployers();
        void loadVacancies(int employerId);


};

#endif // EMPLOYEEWINDOW_H
