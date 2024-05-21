#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_addAgentButton_clicked();
    void on_refreshButton_clicked();
    void on_deleteButton_clicked();
    void on_exitButton_clicked();


private:
    Ui::AdminWindow *ui;
    void populateAgentsTable();
};

#endif // ADMINWINDOW_H
