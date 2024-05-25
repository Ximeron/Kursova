#ifndef VERIFICATIONDIALOG_H
#define VERIFICATIONDIALOG_H

#include <QDialog>

namespace Ui
{
class VerificationDialog;
}

class VerificationDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit VerificationDialog(QWidget* parent = nullptr);
        ~VerificationDialog();
        double getCommission() const;


    private slots:
        void on_buttonBox_accepted();
        void on_buttonBox_rejected();



    private:
        Ui::VerificationDialog* ui;
        double commission;

};

#endif // VERIFICATIONDIALOG_H
