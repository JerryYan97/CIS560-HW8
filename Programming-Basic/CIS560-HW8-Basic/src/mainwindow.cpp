#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include "jsonreader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    // Disable every pinBox
    ui->faceBlueSpinBox->setEnabled(false);
    ui->faceGreenSpinBox->setEnabled(false);
    ui->faceRedSpinBox->setEnabled(false);
    ui->vertPosXSpinBox->setEnabled(false);
    ui->vertPosYSpinBox->setEnabled(false);
    ui->vertPosZSpinBox->setEnabled(false);
    ui->pushButtonAddVertex->setEnabled(false);
    ui->pushButtonTriangulate->setEnabled(false);

    // Wake up related pinBox when a vertexListWedgetItem is selected
    // Then, we can change the value of the vertices by changing the values through pinbox
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_freeVerticesPinboxes(QListWidgetItem*)));

    // Wake up related pinBox when a faceListWedgetItem is selected
    // Then, we can change the value of the faces by changing the values through pinbox
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_freeFacesPinboxes(QListWidgetItem*)));

    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_slientAllPinboxes(QListWidgetItem*)));

    // Connect slot and signal for pinboxes
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_VertexPositionXValue(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_VertexPositionYValue(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_VertexPositionZValue(double)));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_FaceColorRedValue(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_FaceColorGreenValue(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slot_FaceColorBlueValue(double)));

    // Connect slot and signal for highlight
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_highlightVertex(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_highlightFace(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_highlightHalfEdge(QListWidgetItem*)));
    connect(ui->jointsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_highlightJoint(QListWidgetItem*)));
    connect(ui->jointsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slot_jointItemOtherActions(QListWidgetItem*)));

    // Connect slot and signal for Visual Debugging Tools
    connect(ui->mygl, SIGNAL(SendNextHEOfHE(QListWidgetItem*)), this, SLOT(slot_highlightHalfEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(SendSYMHEOfHE(QListWidgetItem*)), this, SLOT(slot_highlightHalfEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(SendVertexOfHE(QListWidgetItem*)), this, SLOT(slot_highlightVertex(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(SendFaceOfHE(QListWidgetItem*)), this, SLOT(slot_highlightFace(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(SendHEOfFace(QListWidgetItem*)), this, SLOT(slot_highlightHalfEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(SendHEOfVertex(QListWidgetItem*)), this, SLOT(slot_highlightHalfEdge(QListWidgetItem*)));

    // Connect slot and signal for split edge and triangulate face
    connect(ui->pushButtonAddVertex, SIGNAL(clicked(bool)), this, SLOT(slot_splitCubeEdge(bool)));
    connect(ui->pushButtonTriangulate, SIGNAL(clicked(bool)), this, SLOT(slot_triangulateCubeFace(bool)));

    // Connect slot and signal for smooth option
    connect(ui->pushButtonSmooth, SIGNAL(clicked(bool)), this, SLOT(slot_smoothMesh(bool)));

    // Put the mesh into the tree_widget.
    connect(ui->mygl, SIGNAL(SendMesh(Mesh&)), this, SLOT(slot_addMeshToListWidget(Mesh&)));

    // Put the skeleton into the tree_widget.
    connect(ui->mygl, SIGNAL(SendSkeleton(Skeleton&)), this, SLOT(slot_addSkeletonToListWidget(Skeleton&)));

    // Connect slot and signal for extrude face
    connect(ui->pushButtonExtrude, SIGNAL(clicked(bool)), this, SLOT(slot_extrudeFace(bool)));

    // Connect slot and signal for load obj
    //connect(ui->pushButtonLoadScene, SIGNAL(clicked(bool)), this, SLOT(on_actionLoad_Scene_triggered(bool)));
    connect(ui->mygl, SIGNAL(DrawCow(bool)), this, SLOT(on_actionLoad_Scene_triggered(bool)));

    // Connect slot and signal for highlighting vertex
    connect(ui->mygl, SIGNAL(HighlightVert(QListWidgetItem*)), this, SLOT(slot_highlightVertex(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(HighlightFace(QListWidgetItem*)), this, SLOT(slot_highlightFace(QListWidgetItem*)));

    // Connect slot and signal for load skeleton
    connect(ui->mygl, SIGNAL(DrawSkeleton(bool)), this, SLOT(on_actionLoad_Skeleton_triggered(bool)));
    connect(ui->pushButtonSkinning, SIGNAL(clicked(bool)), this, SLOT(slot_onSkinningButtonClicked(bool)));

    connect(ui->pushButtonXRotation, SIGNAL(clicked(bool)), this, SLOT(slot_onRotationXButtonClicked(bool)));
    connect(ui->pushButtonYRotation, SIGNAL(clicked(bool)), this, SLOT(slot_onRotationYButtonClicked(bool)));
    connect(ui->pushButtonZRotation, SIGNAL(clicked(bool)), this, SLOT(slot_onRotationZButtonClicked(bool)));

    // Slient all button
    ui->pushButtonXRotation->setEnabled(false);
    ui->pushButtonYRotation->setEnabled(false);
    ui->pushButtonZRotation->setEnabled(false);

    mActivatedJoint = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::slot_addMeshToListWidget(Mesh &iMesh)
{
    for(std::vector<std::unique_ptr<Face>>::iterator ptr = iMesh.mFaceList.begin();
        ptr != iMesh.mFaceList.end();
        ptr++)
    {
        ui->facesListWidget->addItem(ptr->get());
    }

    for(std::vector<std::unique_ptr<HalfEdge>>::iterator ptr = iMesh.mHalfEdgeList.begin();
        ptr != iMesh.mHalfEdgeList.end();
        ptr++)
    {
        ui->halfEdgesListWidget->addItem(ptr->get());
    }

    for(std::vector<std::unique_ptr<Vertex>>::iterator ptr = iMesh.mVertexList.begin();
        ptr != iMesh.mVertexList.end();
        ptr++)
    {
        ui->vertsListWidget->addItem(ptr->get());
    }
}

void MainWindow::slot_freeVerticesPinboxes(QListWidgetItem* i)
{
    ui->faceBlueSpinBox->setEnabled(false);
    ui->faceGreenSpinBox->setEnabled(false);
    ui->faceRedSpinBox->setEnabled(false);
    ui->vertPosXSpinBox->setEnabled(true);
    ui->vertPosYSpinBox->setEnabled(true);
    ui->vertPosZSpinBox->setEnabled(true);

    Vertex* vPtr = dynamic_cast<Vertex*>(i);
    ui->vertPosXSpinBox->setValue(vPtr->mPos.x);
    ui->vertPosYSpinBox->setValue(vPtr->mPos.y);
    ui->vertPosZSpinBox->setValue(vPtr->mPos.z);
}

void MainWindow::slot_freeFacesPinboxes(QListWidgetItem* i)
{
    ui->faceBlueSpinBox->setEnabled(true);
    ui->faceGreenSpinBox->setEnabled(true);
    ui->faceRedSpinBox->setEnabled(true);
    ui->vertPosXSpinBox->setEnabled(false);
    ui->vertPosYSpinBox->setEnabled(false);
    ui->vertPosZSpinBox->setEnabled(false);

    ui->pushButtonAddVertex->setEnabled(false);
    ui->pushButtonTriangulate->setEnabled(true);

    Face* fPtr = dynamic_cast<Face*>(i);
    ui->faceBlueSpinBox->setValue(fPtr->mColor.z);
    ui->faceGreenSpinBox->setValue(fPtr->mColor.y);
    ui->faceRedSpinBox->setValue(fPtr->mColor.x);
}

void MainWindow::slot_slientAllPinboxes(QListWidgetItem *i)
{
    ui->faceBlueSpinBox->setEnabled(false);
    ui->faceGreenSpinBox->setEnabled(false);
    ui->faceRedSpinBox->setEnabled(false);
    ui->vertPosXSpinBox->setEnabled(false);
    ui->vertPosYSpinBox->setEnabled(false);
    ui->vertPosZSpinBox->setEnabled(false);

    ui->pushButtonAddVertex->setEnabled(true);
    ui->pushButtonTriangulate->setEnabled(false);
}

void MainWindow::slot_VertexPositionXValue(double i)
{
    QListWidgetItem* curr =  ui->vertsListWidget->currentItem();
    Vertex* vPtr = dynamic_cast<Vertex*>(curr);
    vPtr->mPos.x = i;
    ui->mygl->AdjustMeshPlanar(ui->mygl->m_Cube);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_VertexPositionYValue(double i)
{
    QListWidgetItem* curr =  ui->vertsListWidget->currentItem();
    Vertex* vPtr = dynamic_cast<Vertex*>(curr);
    vPtr->mPos.y = i;
    ui->mygl->AdjustMeshPlanar(ui->mygl->m_Cube);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_VertexPositionZValue(double i)
{
    QListWidgetItem* curr =  ui->vertsListWidget->currentItem();
    Vertex* vPtr = dynamic_cast<Vertex*>(curr);
    vPtr->mPos.z = i;
    ui->mygl->AdjustMeshPlanar(ui->mygl->m_Cube);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_FaceColorRedValue(double i)
{
    QListWidgetItem* curr =  ui->facesListWidget->currentItem();
    Face* fPtr = dynamic_cast<Face*>(curr);
    fPtr->mColor.x = i;
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_FaceColorGreenValue(double i)
{
    QListWidgetItem* curr =  ui->facesListWidget->currentItem();
    Face* fPtr = dynamic_cast<Face*>(curr);
    fPtr->mColor.y = i;
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_FaceColorBlueValue(double i)
{
    QListWidgetItem* curr =  ui->facesListWidget->currentItem();
    Face* fPtr = dynamic_cast<Face*>(curr);
    fPtr->mColor.z = i;
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_highlightVertex(QListWidgetItem *i)
{
    ui->pushButtonXRotation->setEnabled(false);
    ui->pushButtonYRotation->setEnabled(false);
    ui->pushButtonZRotation->setEnabled(false);
    ui->mygl->DisplayClear();
    ui->mygl->m_skeleton.ClearSelected();
    Vertex* vPtr = dynamic_cast<Vertex*>(i);
    for(unsigned int i = 0; i < vPtr->mJointInfluVec.size(); i++)
    {
        JointInfluence jInflu = vPtr->mJointInfluVec.at(i);
        jInflu.tarJoint->mSelected = true;
    }

    ui->mygl->setFocus();
    ui->mygl->m_vertDisplay.updateVertex(vPtr);
    ui->mygl->update();
}

void MainWindow::slot_highlightFace(QListWidgetItem *i)
{
    ui->pushButtonXRotation->setEnabled(false);
    ui->pushButtonYRotation->setEnabled(false);
    ui->pushButtonZRotation->setEnabled(false);
    ui->mygl->DisplayClear();
    Face* fPtr = dynamic_cast<Face*>(i);
    ui->mygl->setFocus();
    ui->mygl->m_faceDisplay.updateFace(fPtr);
    ui->mygl->update();
}

void MainWindow::slot_highlightHalfEdge(QListWidgetItem *i)
{
    ui->pushButtonXRotation->setEnabled(false);
    ui->pushButtonYRotation->setEnabled(false);
    ui->pushButtonZRotation->setEnabled(false);
    ui->mygl->DisplayClear();
    HalfEdge* hePtr = dynamic_cast<HalfEdge*>(i);
    ui->mygl->setFocus();
    ui->mygl->m_halfEdgeDisplay.updateHalfEdge(hePtr);
    ui->mygl->update();
}

void MainWindow::slot_highlightJoint(QListWidgetItem *i)
{
    ui->mygl->DisplayClear();
    Joint* jPtr = dynamic_cast<Joint*>(i);
    ui->mygl->m_skeleton.ClearSelected();
    jPtr->mSelected = true;
    ui->mygl->setFocus();
    ui->mygl->update();
}

void MainWindow::slot_splitCubeEdge(bool i)
{

    ui->mygl->SplitHE(ui->mygl->m_halfEdgeDisplay.representedHalfEdge, ui->mygl->m_Cube);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_triangulateCubeFace(bool i)
{
    ui->mygl->TriangulateFace(ui->mygl->m_faceDisplay.representedFace, ui->mygl->m_Cube);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_smoothMesh(bool i)
{
    ui->mygl->m_Cube.Catmull_Clark_Subdivide();
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::slot_extrudeFace(bool i)
{
    float dis = 3.f;
    QList<QListWidgetItem*> itemList = ui->facesListWidget->selectedItems();
    if(itemList.size() != 0){
        QListWidgetItem* tempI = itemList.at(0);
        Face* f = dynamic_cast<Face*>(tempI);
        ui->mygl->m_Cube.ExtrudeFace(f, dis);
        ui->mygl->setFocus();
        ui->mygl->m_Cube.destroy();
        ui->mygl->m_Cube.create();
        ui->mygl->update();
        emit ui->mygl->SendMesh(ui->mygl->m_Cube);
    }
}

MPolygon MainWindow::LoadOBJ(const QString &file, const QString &polyName)
{
    MPolygon p(polyName);
    QString filepath = file;
    std::vector<tinyobj::shape_t> shapes; std::vector<tinyobj::material_t> materials;
    std::string errors = tinyobj::LoadObj(shapes, materials, filepath.toStdString().c_str());
    std::cout << errors << std::endl;
    if(errors.size() == 0)
    {
        int min_idx = 0;
        //Read the information from the vector of shape_ts
        for(unsigned int i = 0; i < shapes.size(); i++)
        {
            std::vector<glm::vec4> pos, nor;
            std::vector<glm::vec2> uv;
            std::vector<float> &positions = shapes[i].mesh.positions;
            std::vector<float> &normals = shapes[i].mesh.normals;
            std::vector<float> &uvs = shapes[i].mesh.texcoords;
            for(unsigned int j = 0; j < positions.size()/3; j++)
            {
                pos.push_back(glm::vec4(positions[j*3], positions[j*3+1], positions[j*3+2],1));
            }
            for(unsigned int j = 0; j < normals.size()/3; j++)
            {
                nor.push_back(glm::vec4(normals[j*3], normals[j*3+1], normals[j*3+2],0));
            }
            for(unsigned int j = 0; j < uvs.size()/2; j++)
            {
                uv.push_back(glm::vec2(uvs[j*2], uvs[j*2+1]));
            }
            for(unsigned int j = 0; j < pos.size(); j++)
            {
                p.AddPVertex(PVertex(pos[j], glm::vec3(255,255,255), nor[j], uv[j]));
            }

            std::vector<unsigned int> indices = shapes[i].mesh.indices;
            for(unsigned int j = 0; j < indices.size(); j += 3)
            {
                Triangle t;
                t.m_indices[0] = indices[j] + min_idx;
                t.m_indices[1] = indices[j+1] + min_idx;
                t.m_indices[2] = indices[j+2] + min_idx;
                p.AddTriangle(t);
            }

            min_idx += pos.size();
        }
    }
    else
    {
        //An error loading the OBJ occurred!
        std::cout << errors << std::endl;
    }
    return p;
}

void MainWindow::on_actionLoad_Scene_triggered(bool ib)
{
    std::vector<int> iVec;
    std::vector<MPolygon> polygons;

    /*QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"),
                                                    QDir::currentPath().append(QString("../..")),
                                                    QString("*.obj"));*/
    QString filename = QString("../../jsons/cow.obj");
    QString name = QString("helloWorld");
    MPolygon p = LoadOBJ(filename, name);

    ui->mygl->m_Cube.Clear();

    ui->mygl->m_Cube.PolygonToMesh(p);

    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
    emit ui->mygl->SendMesh(ui->mygl->m_Cube);
}

void MainWindow::on_actionLoad_Skeleton_triggered(bool i)
{
    /*QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"),
                                                    QDir::currentPath().append(QString("../..")),
                                                    QString("*.json"));*/
    QString filename = QString("../../jsons/cow_skeleton.json");
    std::cout << "filename:" << filename.toStdString() << std::endl;
    JSONReader mJR;
    mJR.ParseToSkeleton(filename, ui->mygl->m_skeleton);
    emit ui->mygl->SendSkeleton(ui->mygl->m_skeleton);
}

void MainWindow::slot_addSkeletonToListWidget(Skeleton &i)
{
    for(std::vector<std::unique_ptr<Joint>>::iterator ptr = i.mJointsList.begin(); ptr != i.mJointsList.end(); ptr++)
    {
        ui->jointsListWidget->addItem(ptr->get());
    }
}

void MainWindow::slot_onSkinningButtonClicked(bool i)
{
    ui->mygl->Skinning(ui->mygl->m_Cube, ui->mygl->m_skeleton);
    ui->mygl->setFocus();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_jointItemOtherActions(QListWidgetItem* i)
{
    ui->pushButtonXRotation->setEnabled(true);
    ui->pushButtonYRotation->setEnabled(true);
    ui->pushButtonZRotation->setEnabled(true);
    // Display the Degree values in the Tag
    Joint* jPtr = dynamic_cast<Joint*>(i);
    mActivatedJoint = jPtr;
    ui->label_XAxisDegree->setText(QString::number(jPtr->mXAxisDegree));
    ui->label_YAxisDegree->setText(QString::number(jPtr->mYAxisDegree));
    ui->label_ZAxisDegree->setText(QString::number(jPtr->mZAxisDegree));
}

void MainWindow::slot_onRotationXButtonClicked(bool i)
{
    mActivatedJoint->mXAxisDegree += 5;
    ui->label_XAxisDegree->setText(QString::number(mActivatedJoint->mXAxisDegree));
    mActivatedJoint->UpdateOrientation();
    ui->mygl->setFocus();
    ui->mygl->m_skeleton.destroy();
    ui->mygl->m_skeleton.create();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_onRotationYButtonClicked(bool i)
{
    mActivatedJoint->mYAxisDegree += 5;
    ui->label_YAxisDegree->setText(QString::number(mActivatedJoint->mYAxisDegree));
    mActivatedJoint->UpdateOrientation();
    ui->mygl->setFocus();
    ui->mygl->m_skeleton.destroy();
    ui->mygl->m_skeleton.create();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}

void MainWindow::slot_onRotationZButtonClicked(bool i)
{
    mActivatedJoint->mZAxisDegree += 5;
    ui->label_ZAxisDegree->setText(QString::number(mActivatedJoint->mZAxisDegree));
    mActivatedJoint->UpdateOrientation();
    ui->mygl->setFocus();
    ui->mygl->m_skeleton.destroy();
    ui->mygl->m_skeleton.create();
    ui->mygl->m_Cube.destroy();
    ui->mygl->m_Cube.create();
    ui->mygl->update();
}
