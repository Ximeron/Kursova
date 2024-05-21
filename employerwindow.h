#ifndef EMPLOYERWINDOW_H
#define EMPLOYERWINDOW_H

#include <QWidget>

namespace Ui {
class EmployerWindow;
}

class EmployerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EmployerWindow(QWidget *parent = nullptr);
    ~EmployerWindow();
private slots:
    void on_addVacancyButton_clicked();
    void on_moveToActsButton_clicked();

private:
    Ui::EmployerWindow *ui;
    void loadApplications();
    void setupTableWidget();


};

#endif // EMPLOYERWINDOW_H
