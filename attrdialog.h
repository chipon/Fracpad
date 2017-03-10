#ifndef ATTRDIALOG_H
#define ATTRDIALOG_H

#include <QDialog>

namespace Ui {
class AttrDialog;
}

class AttrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AttrDialog(QWidget *parent = 0);
    ~AttrDialog();
    QString getValue();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AttrDialog *ui;
    QString value;
};

#endif // ATTRDIALOG_H
