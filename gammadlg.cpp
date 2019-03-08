#include "gammadlg.h"
#include<QWidget>
#include<QDialog>
#include<QHBoxLayout>
#include<QString>

GammaDlg::GammaDlg(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout* hlayout= new QHBoxLayout();

    putGamma=new QLineEdit;
    m_btnOK =new QPushButton(tr("确认"));
    connect(m_btnOK,SIGNAL(clicked(bool)),this,SLOT(onButtonGamma()));

    hlayout->addWidget(putGamma);
    hlayout->addWidget(m_btnOK);

    setLayout(hlayout);
}

void GammaDlg::onButtonGamma()
{
    QString str;
    str=putGamma->text();
    nGamma=str.toDouble();

    QDialog:accept();
}
