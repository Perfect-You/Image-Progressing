#ifndef PSNRDLG_H
#define PSNRDLG_H

#include <QWidget>
#include "mainwidget.h"
#include<QDialog>
#include<QLineEdit>
#include<QLabel>

class PSNRDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PSNRDlg(QWidget *parent = nullptr);
    QLabel *m_labpsnr;
signals:

public slots:
};

#endif // PSNRDLG_H
