#ifndef MSEDLG_H
#define MSEDLG_H

#include <QWidget>
#include "mainwidget.h"
#include<QDialog>
#include<QLineEdit>
#include<QLabel>

class MSEDlg : public QDialog
{
    Q_OBJECT
public:
    explicit MSEDlg(QWidget *parent = nullptr);

    QLabel *m_labmse;
signals:

public slots:
};

#endif // MSEDLG_H
