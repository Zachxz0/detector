#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qstringlistmodel.h"
#include "qstringlist.h"
#include "qlabel.h"
#include "qpixmap_item.h"
#include "net_thread.h"
#include <iostream>
#include <QtCore/qmath.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <detectorproxy.h>
#include <QVector>
#include <google/protobuf/text_format.h>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<ITCMsg*>("ITCMsg*");
    findView();
    detector = new DetectorProxy();

    connect(detector,SIGNAL(recWeightMap(int,int,int,int,unsigned char*)),this,SLOT(getWeightMap(int,int,int,int,unsigned char*))) ;
    connect(detector,SIGNAL(recFeature(int,int,int,int,unsigned char*)),this,SLOT(getFeatureMap(int,int,int,int,unsigned char*))) ;
    connect(detector,SIGNAL(recLayerInfo(ITCMsg*)),this,SLOT(getLayerInfo(ITCMsg*))) ;
    connect(detector,SIGNAL(recInput(int,int,int,unsigned char*)),this,SLOT(getInput(int,int,int,unsigned char*))) ;
    connect(detector,SIGNAL(recOutput(ITCMsg*)),this,SLOT(getOutput(ITCMsg*))) ;
    connect(detector,SIGNAL(recDeconv(int,int,int,unsigned char*)),this,SLOT(getDeconv(int,int,int,unsigned char*))) ;

    connect(this,SIGNAL(requestFeature(int,int,bool)),detector->getSendThread(),SLOT(requestFeature(int,int,bool)));
    connect(this,SIGNAL(requestLayerInfo(int)),detector->getSendThread(),SLOT(requestLayerInfo(int)));
    connect(this,SIGNAL(requestWeight(int)),detector->getSendThread(),SLOT(requestWeight(int)));
    connect(this,SIGNAL(requestInput(int)),detector->getSendThread(),SLOT(requestInput(int)));
    connect(this,SIGNAL(requestOutput(int,int)),detector->getSendThread(),SLOT(requestOutput(int,int)));
    connect(this,SIGNAL(requestWeightDiff()),detector->getSendThread(),SLOT(requestWeightDiff()));
    detector->start();
    layer_info_model = new QStringListModel();

    maps = NULL;
    input = NULL;
    weights=NULL;
    map_item = NULL;
    weight_item = NULL;

}

void MainWindow::getWeightMap(int w,int h,int c,int n,unsigned char* image)
{
    if((weights_num!=c*n&&c!=3)||(weights_num!=n&&c==3))
    {
        ui->tw_weight->clear();
        if(this->weights !=NULL){
            delete []this->weights;
            if(c==3)
                this->weights = new QPixmap[n];
            else
                this->weights = new QPixmap[n*c];
        }
    }
    if(this->weights==NULL)
    {
        if(c==3)
            this->weights = new QPixmap[n];
        else
            this->weights = new QPixmap[n*c];
    }
    weights_num = c==3?n:c*n;
    int tw_width = ui->tw_weight->width();
    int tw_height = ui->tw_weight->height();
    int size ,row,num;
    if(c==3){
        size = c*w*h;
        num = n;
        row = (int)qSqrt((double)num);
        if(row>=20)row = 20;
    }else{
        size = w*h;
        num = c*n;
        row = (int)qSqrt((double)(num));
        if(row>=20)row = 20;
    }
    int scale = (tw_width-tw_width*0.01)/row;

    ui->tw_weight->setColumnCount(row);
    ui->tw_weight->setRowCount(((double)num/row)+0.9);
       //以下是设置第一行的所有列的图片
    for(int r=0;r<ui->tw_weight->rowCount();++r){
        for(int col=0; col<ui->tw_weight->columnCount(); col++)
        {
            if(r*row+col>=num)break;
            QImage *tm_img;
            QImage *save_weight;
            if(c==3)
            {
                tm_img = new QImage(image+(r*row+col)*size,w,h,QImage::Format_RGB888);
            }else{
                tm_img = new QImage(image+(r*row+col)*size,w,h,QImage::Format_Grayscale8);
            }
            QPixmap pix = QPixmap::fromImage(*tm_img);
            this->weights[r*row+col] = pix.copy(0,0,w,h);
            QTableWidgetItem *item = new  QTableWidgetItem;
            ui->tw_weight->setColumnWidth(col,scale-scale/30);
            ui->tw_weight->setRowHeight(r,scale-scale/30);
           item->setData(Qt::DisplayRole,QVariant::fromValue<QPixmap>(pix));
            ui->tw_weight->setItem(r,col,item);
            delete tm_img;
        }
    }
    delete[] image;
    image = NULL;
}

void MainWindow::getFeatureMap(int w,int h,int c,int n,unsigned char* map)
{
    if(c!=3)
    {
        int size = w*h;
        if(maps_num!=n*c){
            ui->tw_map->clear();
            if(maps!=NULL){
                delete []maps;
                maps = new QPixmap[c*n];
            }
        }
        if(maps == NULL)maps = new QPixmap[c*n];
        maps_num = n*c;
        int tw_width = ui->tw_map->width();
        int tw_height = ui->tw_map->height();
        int row;
        row = (int)qSqrt((double)c);

        int scale = (float)tw_width/row;
        ui->tw_map->setColumnCount(row);
        ui->tw_map->setRowCount(row);

           //以下是设置第一行的所有列的图片
        for(int r=0;r<ui->tw_map->rowCount();++r){
            for(int col=0; col<ui->tw_map->columnCount(); col++)
            {
                if(r*row+col>=c)break;
                QImage* tm_img;
                tm_img = new QImage(map+w*h*(r*row+col),w,h,QImage::Format_Grayscale8);
                QPixmap pix = QPixmap::fromImage(*tm_img);
                maps[r*row+col] = pix.copy(0,0,w,h);
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tw_map->setColumnWidth(col,scale);
                ui->tw_map->setRowHeight(r,scale);
                item->setData(Qt::DisplayRole,QVariant::fromValue<QPixmap>(pix));
                ui->tw_map->setItem(r,col, item);
                delete tm_img;
            }
        }

    }else if(c==3){
        int size = w*h*c;
        if(maps_num!=n){
            ui->tw_map->clear();
            if(maps!=NULL){
                delete []maps;
                maps = new QPixmap[n];
            }
        }
        if(maps == NULL)maps = new QPixmap[n];
        maps_num = n;
        int tw_width = ui->tw_map->width();
        int tw_height = ui->tw_map->height();
        int row;
        row = (int)qSqrt((double)n);

        int scale = (float)tw_width/row;
        ui->tw_map->setColumnCount(row);
        ui->tw_map->setRowCount(row);

           //以下是设置第一行的所有列的图片
        for(int r=0;r<ui->tw_map->rowCount();++r){
            for(int col=0; col<ui->tw_map->columnCount(); col++)
            {
                if(r*row+col>=n)break;
                QImage* tm_img;
                tm_img = new QImage(map+w*h*(r*row+col)*3,w,h,QImage::Format_RGB888);
                QPixmap pix = QPixmap::fromImage(*tm_img);
                maps[r*row+col] = pix.copy(0,0,w,h);
                QTableWidgetItem *item = new QTableWidgetItem;
                ui->tw_map->setColumnWidth(col,scale);
                ui->tw_map->setRowHeight(r,scale);
                item->setData(Qt::DisplayRole,QVariant::fromValue<QPixmap>(pix));
                ui->tw_map->setItem(r,col, item);
                delete tm_img;
            }
        }
    }
    delete map;
    map = NULL;
}

void MainWindow::getDeconv(int w,int h,int c,unsigned char* image)
{
    delete im_deconv->scene();
    QImage *tm_img = NULL;
    if(c==1)
    {
        tm_img = new QImage(image,w,h,QImage::Format_Grayscale8);
    }else if(c==3){
        tm_img = new QImage(image,w,h,QImage::Format_RGB888);
    }else{
        delete[] image;
        return;
    }
    QPixmap pix = QPixmap::fromImage(*tm_img);
    QPixmap sc_pix = pix.scaled(im_deconv->width(),im_deconv->height());
    QGraphicsScene *scene = new QGraphicsScene;
    im_deconv->setScene(scene);
    scene->addPixmap(sc_pix);
    im_deconv->setSceneRect(0,0,im_deconv->width()-5,im_deconv->height()-5);
    delete tm_img;
    delete[] image;
}

void MainWindow::getInput(int w,int h,int c,unsigned char* image)//ITCMsg *itcmsg)
{
    delete im_input->scene();
    QImage* tm_img = new QImage(image,w,h,QImage::Format_RGB888);
    QPixmap pix = QPixmap::fromImage(*tm_img);
    QPixmap sc_pix = pix.scaled(im_input->width(),im_input->height());
    QGraphicsScene *scene = new QGraphicsScene;
    im_input->setScene(scene);
    scene->addPixmap(sc_pix);
    im_input->setSceneRect(0,0,im_input->width()-5,im_input->height()-5);
    delete tm_img;
    delete[] image;
}

void MainWindow::getLayerInfo(ITCMsg *msg)
{
    VLayerInfos *infos = (VLayerInfos*)(msg->msg);
    layer_infos.CopyFrom(*infos);
    QStringList list;
    for(int i=0;i<infos->infos_size();++i)
    {
        list <<(infos->infos().Get(i).name()).c_str();
    }
    layer_info_model->setStringList(list);
    lv_layers->setModel(layer_info_model);
    delete msg;
}

void MainWindow::getOutput(ITCMsg *)
{

}


void MainWindow::findView()
{
    lv_layers = ui->lv_layers;
    im_input = ui->gv_input;
    im_deconv = ui->gv_deconv;
    im_map = ui->gv_map;
    tw_weight = ui->tw_weight;
    tw_map = ui->tw_map;

    ui->tw_weight->setItemDelegate(new QPixmapItemdele());
    ui->tw_weight->horizontalHeader()->setVisible(false);
    ui->tw_weight->verticalHeader()->setVisible(false);
    ui->tw_weight->setSelectionMode(ui->tw_weight->NoSelection);

    ui->tw_map->setItemDelegate(new QPixmapItemdele());
    ui->tw_map->horizontalHeader()->setVisible(false);
    ui->tw_map->verticalHeader()->setVisible(false);
    ui->tw_map->setSelectionMode(ui->tw_map->NoSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete[] weights;
    delete[] maps;
}

void MainWindow::on_tw_map_clicked(const QModelIndex &index)
{
    delete im_map->scene();
    QPixmap &pix = maps[index.row()*tw_map->columnCount()+index.column()];
    QPixmap pix2 = pix.scaled(im_map->width(),im_map->height());
    QGraphicsScene *scene = new QGraphicsScene;
    im_map->setScene(scene);
    scene->addPixmap(pix2);
    im_map->setSceneRect(0,0,im_map->width()-5,im_map->height()-5);
    emit requestOutput(-1,index.row()*tw_map->rowCount()+index.column());
}

void MainWindow::on_lv_layers_clicked(const QModelIndex &index)
{
    emit requestFeature(index.row(),0,false);
}

void MainWindow::on_tw_weight_clicked(const QModelIndex &index)
{
    QPixmap pix = weights[index.row()*tw_weight->columnCount()+index.column()];
    QPixmap pix2 = pix.scaled(im_map->width(),im_map->height());
    QGraphicsScene *scene = new QGraphicsScene;
    im_map->setScene(scene);
    scene->addPixmap(pix2);
    im_map->setSceneRect(0,0,im_map->width()-5,im_map->height()-5);
}

void MainWindow::on_actionShowDiff_triggered()
{
    emit requestFeature(-1,0,true);
}

void MainWindow::on_actionShowWeight_triggered()
{
    emit requestWeight(0);
}

void MainWindow::on_actionShowMap_triggered()
{

}

void MainWindow::on_actionShowLayerInfo_triggered()
{
    emit requestLayerInfo(0);
}

void MainWindow::on_actionShowDeconv_triggered()
{

}

void MainWindow::on_actionShowInput_triggered()
{
    emit requestInput(0);
}

void MainWindow::on_actionshowlayerLoss_triggered()
{
    emit requestWeightDiff();
}
