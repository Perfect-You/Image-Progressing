#include "mainwidget.h"
#include<QHBoxLayout>  //水平排版
#include<QVBoxLayout>  //垂直排版

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* hlayout=new QHBoxLayout(this);
    m_labimage=new QLabel(tr("This is QT1"));
    hlayout->addWidget(m_labimage);
    setLayout(hlayout);
}

MainWidget::~MainWidget()
{

}

void MainWidget::paintEvent(QImage img)
{
    m_labimage->resize(QSize(img.width(),img.height()));
    QPixmap pix;
    pix=pix.fromImage(img);
    m_labimage->setPixmap(pix);
}
