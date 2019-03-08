#include "psnrdlg.h"
#include "mainwidget.h"
#include<QDialog>
#include<QHBoxLayout>  //水平排版
#include<QVBoxLayout>  //垂直排版

PSNRDlg::PSNR(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout* hlayout=new QHBoxLayout(this);
    m_labpsnr=new QLabel();
    hlayout->addWidget(m_labpsnr);
    setLayout(hlayout);
}
