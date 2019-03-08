#include "mainwidget.h"
#include "msedlg.h"
#include<QDialog>
#include<QHBoxLayout>  //水平排版
#include<QVBoxLayout>  //垂直排版


MSEDlg::MSEDlg(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout* hlayout=new QHBoxLayout(this);
    m_labmse=new QLabel();
    hlayout->addWidget(m_labmse);
    setLayout(hlayout);
}
