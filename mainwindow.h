#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMenu>
#include<QToolBar>
#include<QToolButton>
#include<QMenuBar>
#include "mainwidget.h"
#include "gammadlg.h"
#include "msedlg.h"
#include "psnrdlg.h"

#include<QFileDialog>
#include<QImage>
#include<QString>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    QString m_fileadress;
    QImage m_scrImg;
    QImage m_chaImg;
    QImage m_chaImg1;
    QImage m_chaImg2;
    QImage m_mainImg;
    Mat SaveImg;
    QImage m_grayImg;
    double n_Gamma;
    double m_mse;
    double m_psnr;

    //文件菜单栏
    QMenu *m_filemenu;
    QAction* m_openfile;
    QAction* m_savefile;
    QAction* m_exit;

    //图像处理菜单栏
    QMenu *m_imageprocessing;
    QAction* m_eqhist;
    QAction* m_gamma;
    QAction* m_dithering;

    //质量评价
    QMenu *m_qualityrating;
    QAction* m_onmse;
    QAction* m_onpsnr;

    //工具条
    QToolBar *m_toolbar;
    QToolButton *m_binclear;
    QToolButton *m_zoom;
    QToolButton *m_narrow;

    MainWidget* m_pMainWidget;
    GammaDlg* m_pGammaDlg;
    MSEDlg* m_pmseDlg;
    PSNRDlg* m_psnrDlg;
    Mat QImage2cvMat(QImage image);
    QImage cvMat2QImage(const Mat& mat);
    QImage Eqhist(QImage image);
    QImage Gamma(QImage image);
    QImage Dithering(QImage image);
    double onMSE(QImage Befimg, QImage Aftimg);
    double onPSNR(QImage Befimg, QImage Aftimg);

private slots:            //槽函数
    void slot_onOpenFile();
    void slot_saveImage();    
    void slot_Eqhist();
    void slot_onGamma();
    void slot_Dithering();
    void slot_onMSE();
    void slot_onPSNR();
    void slot_clear();
    void slot_narrow();
    void slot_zoom();

};

#endif // MAINWINDOW_H
