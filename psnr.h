#ifndef PSNR_H
#define PSNRDLG_H

#include <QWidget>
#include "mainwidget.h"
#include<QLabel>

class PSNR : public QDialog
{
    Q_OBJECT
public:
    explicit PSNR(QWidget *parent = nullptr);

    QLabel *m_labpsnr;

signals:

public slots:
};

#endif // PSNR_H
