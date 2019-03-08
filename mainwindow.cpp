#include "mainwindow.h"
#include<QMenuBar>
#include<QFileDialog>
#include<QDebug>
#include"mainwidget.h"
#include "msedlg.h"

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_filemenu=new QMenu();
    m_filemenu=menuBar()->addMenu(tr("文件"));
    QAction* m_openfile=new QAction(tr("打开文件"));
    QAction* m_savefile=new QAction(tr("保存文件"));
    QAction* m_exit=new QAction(tr("退出程序"));
    m_filemenu->addAction(m_openfile);
    m_filemenu->addAction(m_savefile);
    m_filemenu->addAction(m_exit);
    connect(m_openfile,SIGNAL(triggered(bool)),this,SLOT(slot_onOpenFile()));
    connect(m_savefile,SIGNAL(triggered(bool)),this,SLOT(slot_saveImage()));
    connect(m_exit,SIGNAL(triggered(bool)),this,SLOT(close()));

    m_imageprocessing=new QMenu();
    m_imageprocessing=menuBar()->addMenu(tr("图像处理"));
    QAction* m_eqhist=new QAction(tr("直方图均衡化"));
    QAction* m_gamma=new QAction(tr("Gamma校正"));
    QAction* m_dithering=new QAction(tr("抖动加网"));
    m_imageprocessing->addAction(m_eqhist);
    m_imageprocessing->addAction(m_gamma);
    m_imageprocessing->addAction(m_dithering);
    connect(m_eqhist,SIGNAL(triggered(bool)),this,SLOT(slot_Eqhist()));
    connect(m_gamma,SIGNAL(triggered(bool)),this,SLOT(slot_onGamma()));
    connect(m_dithering,SIGNAL(triggered(bool)),this,SLOT(slot_Dithering()));

    m_qualityrating=new QMenu();
    m_qualityrating=menuBar()->addMenu(tr("质量评价"));
    QAction* m_onmse=new QAction(tr("均方误差"));
    QAction* m_onpsnr=new QAction(tr("峰值信噪比"));
    m_qualityrating->addAction(m_onmse);
    m_qualityrating->addAction(m_onpsnr);
    connect(m_onmse,SIGNAL(triggered(bool)),this,SLOT(slot_onMSE()));
    connect(m_onpsnr,SIGNAL(triggered(bool)),this,SLOT(slot_onPSNR()));

    m_zoom=new QToolButton(this);
    m_zoom->setText("放大");
    m_toolbar=addToolBar("工具");
    m_toolbar->addWidget(m_zoom);
    connect(m_zoom,SIGNAL(clicked(bool)),this,SLOT(slot_zoom()));

    m_narrow=new QToolButton(this);
    m_narrow->setText("缩小");
    m_toolbar=addToolBar("工具");
    m_toolbar->addWidget(m_narrow);
    connect(m_narrow,SIGNAL(clicked(bool)),this,SLOT(slot_narrow()));

    m_binclear=new QToolButton(this);
    m_binclear->setText("清理");
    m_toolbar=addToolBar("工具");
    m_toolbar->addWidget(m_binclear);
    connect(m_binclear,SIGNAL(clicked(bool)),this,SLOT(slot_clear()));

    m_pMainWidget=new MainWidget(this);
    setCentralWidget(m_pMainWidget);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slot_onOpenFile()
{
  m_fileadress=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images(*.png *.bmp *.jpg)"));
  m_scrImg.load(m_fileadress);
  m_mainImg=m_scrImg;
  m_pMainWidget->paintEvent(m_mainImg);
}

void MainWindow::slot_saveImage()
{
    QString savefile=QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images(*.jpg *.png *.bmp)"));
    string fileAsSave=savefile.toStdString();
    Mat SaveImg = QImage2cvMat(m_scrImg);
    imwrite(fileAsSave,SaveImg);
}

Mat MainWindow::QImage2cvMat(QImage image)
{
    Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage MainWindow::cvMat2QImage(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::slot_Eqhist()
{
    m_chaImg=Eqhist(m_scrImg);
    m_mainImg=m_chaImg;
    m_pMainWidget->paintEvent(m_mainImg);
}

QImage MainWindow::Eqhist(QImage image)
{
    Mat mat=QImage2cvMat(image);
    Mat Grayimage;
    cvtColor(mat,Grayimage,CV_RGB2GRAY);
    m_grayImg=cvMat2QImage(Grayimage);
    Mat resultImg;
    equalizeHist(Grayimage,resultImg);

    image=cvMat2QImage(resultImg);
    return image;
}

void MainWindow::slot_onGamma()
{
    m_pGammaDlg=new GammaDlg();
    m_pGammaDlg->exec();
   if(m_pGammaDlg->exec()==QDialog::Accepted)
    {
       n_Gamma=m_pGammaDlg->nGamma;
    }
    m_chaImg1=Gamma(m_scrImg);
    m_mainImg=m_chaImg1;
    m_pMainWidget->paintEvent(m_mainImg);
}

QImage MainWindow::Gamma(QImage image)
{
    Mat mat=QImage2cvMat(image);
    Mat Grayimage;
    cvtColor(mat,Grayimage,CV_RGB2GRAY);
    m_grayImg=cvMat2QImage(Grayimage);
    Mat resultImg(Grayimage.size(),CV_8UC1);
    for(int i=0;i<Grayimage.rows;i++)
    {
        for(int j=0;j<Grayimage.cols;j++)
        {
            resultImg.at<uchar>(i,j)=
                    255*pow(Grayimage.at<uchar>(i,j)/255.0,1.0/n_Gamma);
        }
    }
    image=cvMat2QImage(resultImg);
    return image;
}

void MainWindow::slot_Dithering()
{
    m_chaImg2=Dithering(m_scrImg);
    m_mainImg=m_chaImg2;
    m_pMainWidget->paintEvent(m_mainImg);
}

QImage MainWindow::Dithering(QImage img)
{
    cv::Mat image=QImage2cvMat(img);
    Mat Grayimage;
    cvtColor(image,Grayimage,CV_RGB2GRAY);
    m_grayImg=cvMat2QImage(Grayimage);
    Mat resultImg(Grayimage.size(),CV_8UC1);

    int Matrix[4][4]={{0,8,2,10},{4,12,6,14},{3,11,1,9},{7,15,5,13}};
    int m,n;
    for(int i=0;i<Grayimage.rows;i++)
    {
        m=i%4;
        for(int j=0;j<Grayimage.cols;j++)
        {
            n=j%4;
            if(Grayimage.at<uchar>(i,j)/255.0>=Matrix[m][n]/15.0)
                resultImg.at<uchar>(i,j)=255;
            else
                resultImg.at<uchar>(i,j)=0;
        }
    }
    //m_ditheringImg=cvMat2QImage(resultImg);
    img=cvMat2QImage(resultImg);
    return img;
}

void MainWindow::slot_onMSE()
{
    m_mse=onMSE(m_scrImg,m_mainImg);
    m_pmseDlg=new MSEDlg();
    m_pmseDlg->m_labmse->setText(QString::number(m_mse));
    m_pmseDlg->exec();

}


double MainWindow::onMSE(QImage Befimg, QImage Aftimg)
{
    Mat Befmat = QImage2cvMat(Befimg);
    Mat Aftmat = QImage2cvMat(Aftimg);

    double NumD,NumS=0;
    for(int i = 0; i < Befmat.rows; i++)
        {
            // 每列
            for(int j = 0; j < Befmat.cols; j++)
            {
                 NumD=(double)Befmat.at<uchar>(i,j)-(double)Aftmat.at<uchar>(i,j);
                 NumS += NumD*NumD;
            }
        }
    double m_vMSE;
    m_vMSE=NumS/Befmat.rows/Befmat.cols;
    return m_vMSE;
}

void MainWindow::slot_onPSNR()
{
    m_psnr=onPSNR(m_scrImg,m_mainImg);
    m_psnrDlg=new PSNRDlg();
    m_psnrDlg->m_labpsnr->setText(QString::number(m_psnr));
    m_psnrDlg->exec();
}

//PSNR评估
double MainWindow::onPSNR(QImage Befimg, QImage Aftimg)
{
    onMSE(Befimg,Aftimg);
    double m_vPSNR;
    m_vPSNR=10.0 * log10((255*255)/m_mse);
    return m_vPSNR;
}

void MainWindow::slot_clear()
{
    m_pMainWidget->m_labimage->clear();
    m_fileadress="";
}

void MainWindow::slot_narrow()
{
    QMatrix martrix;
    martrix.scale(0.9,0.9);
    m_mainImg=m_mainImg.transformed(martrix);
    m_scrImg=m_scrImg.transformed(martrix);
    m_pMainWidget->m_labimage->setPixmap(QPixmap::fromImage(m_mainImg));
}

void MainWindow::slot_zoom()
{
    QMatrix martrix;
    martrix.scale(1.1,1.1);
    m_mainImg=m_mainImg.transformed(martrix);
    m_scrImg=m_scrImg.transformed(martrix);
    m_pMainWidget->m_labimage->setPixmap(QPixmap::fromImage(m_mainImg));
}
