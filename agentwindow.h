#ifndef AGENTWINDOW_H
#define AGENTWINDOW_H

#include <QWidget>

namespace Ui
{
class AgentWindow;
}

class AgentWindow : public QWidget
{
        Q_OBJECT

    public:
        explicit AgentWindow(QWidget* parent = nullptr);
        ~AgentWindow();

    private slots:
        void on_addEmployerButton_clicked();
        void on_addEmployeeButton_clicked();
        void on_refreshEmpls_clicked();
        void on_deleteEmplee_clicked();
        void on_deleteEmpler_clicked();
        void on_verifyActButton_clicked();
        void on_exitButton_clicked();
        void on_deleteAct_clicked();
        void loadActs();

    private:
        Ui::AgentWindow* ui;
        void populateEmplsTable();


};

#endif // AGENTWINDOW_H
