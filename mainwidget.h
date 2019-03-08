#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QLabel>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void paintEvent(QImage img);
    QLabel *m_labgamma;
    QLabel *m_labimage;

signals:
protected:



public slots:
};

#endif // MAINWIDGET_H
