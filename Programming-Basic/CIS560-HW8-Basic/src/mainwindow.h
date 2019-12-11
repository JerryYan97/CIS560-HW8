#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QImageWriter>
#include <QDebug>
#include <QImage>
#include <QGraphicsScene>
#include "mygl.h"
#include "polygon.h"
#include "tiny_obj_loader.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Joint* mActivatedJoint;
    ~MainWindow();

signals:
    void sign_HighLightRelatedJoints(QListWidgetItem* i);
public slots:
    void slot_extrudeFace(bool i);

    void slot_addMeshToListWidget(Mesh& iMesh);

    void slot_freeVerticesPinboxes(QListWidgetItem* i);
    void slot_freeFacesPinboxes(QListWidgetItem* i);
    void slot_slientAllPinboxes(QListWidgetItem* i);

    void slot_VertexPositionXValue(double i);
    void slot_VertexPositionYValue(double i);
    void slot_VertexPositionZValue(double i);

    void slot_FaceColorRedValue(double i);
    void slot_FaceColorGreenValue(double i);
    void slot_FaceColorBlueValue(double i);

    void slot_highlightVertex(QListWidgetItem* i);
    void slot_highlightFace(QListWidgetItem* i);
    void slot_highlightHalfEdge(QListWidgetItem* i);
    void slot_highlightJoint(QListWidgetItem* i);

    void slot_splitCubeEdge(bool i);
    void slot_triangulateCubeFace(bool i);

    void slot_smoothMesh(bool i);

    void on_actionLoad_Scene_triggered(bool i);

    void on_actionLoad_Skeleton_triggered(bool i);

    void on_actionCamera_Controls_triggered();

    void slot_addSkeletonToListWidget(Skeleton& i);

    void slot_onSkinningButtonClicked(bool i);

    void slot_jointItemOtherActions(QListWidgetItem* i);

    void slot_onRotationXButtonClicked(bool i);
    void slot_onRotationYButtonClicked(bool i);
    void slot_onRotationZButtonClicked(bool i);
private slots:
    void on_actionQuit_triggered();

private:
    MPolygon LoadOBJ(const QString &file, const QString &polyName);
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
