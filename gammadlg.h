#ifndef GAMMADLG_H
#define GAMMADLG_H

#include <QWidget>
#include<QDialog>
#include<QLineEdit>
#include<QPushButton>

class GammaDlg : public QDialog
{
    Q_OBJECT
public:
    explicit GammaDlg(QWidget *parent = nullptr);
    QLineEdit *putGamma;
    QPushButton *pushbutton;
    QPushButton *m_btnOK;
    double nGamma;


signals:

public slots:
    void onButtonGamma();
};

#endif // GAMMADLG_H
