#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> 
#include <QLabel>
#include <QListView>
#include <QTableWidget>
#include "net_thread.h"
#include "QStringListModel"
#include "zoson.pb.h"
#include <detectorproxy.h>
#include <QGraphicsView>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void findView();

    void on_tw_map_clicked(const QModelIndex &index);

    void on_lv_layers_clicked(const QModelIndex &index);

    void getFeatureMap(int,int,int,int,unsigned char*);
    void getDeconv(int w,int h,int c,unsigned char*);
    void getInput(int w,int h,int c,unsigned char*);
    void getLayerInfo(ITCMsg*);
    void getOutput(ITCMsg*);
    void getWeightMap(int w,int h,int c,int n,unsigned char*);

    void on_tw_weight_clicked(const QModelIndex &index);
    void on_actionShowDiff_triggered();
    void on_actionShowWeight_triggered();
    void on_actionShowMap_triggered();
    void on_actionShowLayerInfo_triggered();
    void on_actionShowDeconv_triggered();
    void on_actionShowInput_triggered();

    void on_actionshowlayerLoss_triggered();

signals:
    void requestFeature(int i1,int i2,bool isDiff);
    void requestLayerInfo(int);
    void requestWeight(int);
    void requestInput(int);
    void requestOutput(int,int);
    void requestWeightDiff();
private:
    Ui::MainWindow *ui;
    QTableWidget *tw_map;
    QTableWidget *tw_weight;
    QListView *lv_layers;
    QGraphicsView *im_input;
    QGraphicsView *im_deconv;
    QGraphicsView *im_map;

    NetThread* thread;
    DetectorProxy *detector;

    QStringListModel *layer_info_model;

    VLayerInfos layer_infos;

    QTableWidgetItem *weight_item;
    QTableWidgetItem *map_item;
    QPixmap *input;
    QPixmap *maps;
    int maps_num;
    QPixmap *weights;
    int weights_num;

};

#endif // MAINWINDOW_H
