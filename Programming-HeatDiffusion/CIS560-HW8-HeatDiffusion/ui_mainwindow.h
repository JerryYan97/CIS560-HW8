/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    MyGL *mygl;
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *vertPosXSpinBox;
    QDoubleSpinBox *vertPosYSpinBox;
    QDoubleSpinBox *vertPosZSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceBlueSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *faceGreenSpinBox;
    QDoubleSpinBox *faceRedSpinBox;
    QLabel *label_11;
    QPushButton *pushButtonAddVertex;
    QPushButton *pushButtonTriangulate;
    QPushButton *pushButtonSmooth;
    QPushButton *pushButtonExtrude;
    QListWidget *jointsListWidget;
    QLabel *label_12;
    QPushButton *pushButtonSkinning;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_XAxisDegree;
    QLabel *label_YAxisDegree;
    QLabel *label_ZAxisDegree;
    QPushButton *pushButtonXRotation;
    QPushButton *pushButtonYRotation;
    QPushButton *pushButtonZRotation;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1287, 489);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QStringLiteral("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QStringLiteral("mygl"));
        mygl->setGeometry(QRect(11, 11, 618, 432));
        vertsListWidget = new QListWidget(centralWidget);
        vertsListWidget->setObjectName(QStringLiteral("vertsListWidget"));
        vertsListWidget->setGeometry(QRect(640, 10, 111, 261));
        halfEdgesListWidget = new QListWidget(centralWidget);
        halfEdgesListWidget->setObjectName(QStringLiteral("halfEdgesListWidget"));
        halfEdgesListWidget->setGeometry(QRect(770, 10, 111, 261));
        facesListWidget = new QListWidget(centralWidget);
        facesListWidget->setObjectName(QStringLiteral("facesListWidget"));
        facesListWidget->setGeometry(QRect(900, 10, 111, 261));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(640, 280, 111, 16));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(770, 280, 111, 16));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(900, 280, 111, 16));
        label_3->setAlignment(Qt::AlignCenter);
        vertPosXSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosXSpinBox->setObjectName(QStringLiteral("vertPosXSpinBox"));
        vertPosXSpinBox->setGeometry(QRect(740, 320, 62, 22));
        vertPosXSpinBox->setDecimals(4);
        vertPosXSpinBox->setMinimum(-99);
        vertPosYSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosYSpinBox->setObjectName(QStringLiteral("vertPosYSpinBox"));
        vertPosYSpinBox->setGeometry(QRect(820, 320, 62, 22));
        vertPosYSpinBox->setDecimals(4);
        vertPosYSpinBox->setMinimum(-99);
        vertPosZSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosZSpinBox->setObjectName(QStringLiteral("vertPosZSpinBox"));
        vertPosZSpinBox->setGeometry(QRect(900, 320, 62, 22));
        vertPosZSpinBox->setDecimals(4);
        vertPosZSpinBox->setMinimum(-99);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(640, 320, 91, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(740, 340, 61, 16));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(820, 340, 61, 16));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(900, 340, 61, 16));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(900, 390, 61, 16));
        label_8->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(740, 390, 61, 16));
        label_9->setAlignment(Qt::AlignCenter);
        faceBlueSpinBox = new QDoubleSpinBox(centralWidget);
        faceBlueSpinBox->setObjectName(QStringLiteral("faceBlueSpinBox"));
        faceBlueSpinBox->setGeometry(QRect(900, 370, 62, 22));
        faceBlueSpinBox->setMaximum(1);
        faceBlueSpinBox->setSingleStep(0.01);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(640, 370, 71, 21));
        faceGreenSpinBox = new QDoubleSpinBox(centralWidget);
        faceGreenSpinBox->setObjectName(QStringLiteral("faceGreenSpinBox"));
        faceGreenSpinBox->setGeometry(QRect(820, 370, 62, 22));
        faceGreenSpinBox->setMaximum(1);
        faceGreenSpinBox->setSingleStep(0.01);
        faceRedSpinBox = new QDoubleSpinBox(centralWidget);
        faceRedSpinBox->setObjectName(QStringLiteral("faceRedSpinBox"));
        faceRedSpinBox->setGeometry(QRect(740, 370, 62, 22));
        faceRedSpinBox->setMaximum(1);
        faceRedSpinBox->setSingleStep(0.01);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(820, 390, 61, 16));
        label_11->setAlignment(Qt::AlignCenter);
        pushButtonAddVertex = new QPushButton(centralWidget);
        pushButtonAddVertex->setObjectName(QStringLiteral("pushButtonAddVertex"));
        pushButtonAddVertex->setGeometry(QRect(980, 320, 93, 28));
        pushButtonTriangulate = new QPushButton(centralWidget);
        pushButtonTriangulate->setObjectName(QStringLiteral("pushButtonTriangulate"));
        pushButtonTriangulate->setGeometry(QRect(1090, 320, 93, 28));
        pushButtonSmooth = new QPushButton(centralWidget);
        pushButtonSmooth->setObjectName(QStringLiteral("pushButtonSmooth"));
        pushButtonSmooth->setGeometry(QRect(1000, 370, 93, 28));
        pushButtonExtrude = new QPushButton(centralWidget);
        pushButtonExtrude->setObjectName(QStringLiteral("pushButtonExtrude"));
        pushButtonExtrude->setGeometry(QRect(1130, 370, 93, 28));
        jointsListWidget = new QListWidget(centralWidget);
        jointsListWidget->setObjectName(QStringLiteral("jointsListWidget"));
        jointsListWidget->setGeometry(QRect(1040, 10, 111, 261));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(1040, 280, 111, 16));
        label_12->setAlignment(Qt::AlignCenter);
        pushButtonSkinning = new QPushButton(centralWidget);
        pushButtonSkinning->setObjectName(QStringLiteral("pushButtonSkinning"));
        pushButtonSkinning->setGeometry(QRect(1190, 320, 93, 28));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(640, 420, 81, 21));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(740, 440, 61, 16));
        label_14->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(810, 440, 61, 16));
        label_15->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(880, 440, 61, 16));
        label_16->setAlignment(Qt::AlignCenter);
        label_XAxisDegree = new QLabel(centralWidget);
        label_XAxisDegree->setObjectName(QStringLiteral("label_XAxisDegree"));
        label_XAxisDegree->setGeometry(QRect(740, 420, 61, 16));
        label_XAxisDegree->setFrameShape(QFrame::WinPanel);
        label_XAxisDegree->setFrameShadow(QFrame::Plain);
        label_XAxisDegree->setTextFormat(Qt::PlainText);
        label_XAxisDegree->setAlignment(Qt::AlignCenter);
        label_YAxisDegree = new QLabel(centralWidget);
        label_YAxisDegree->setObjectName(QStringLiteral("label_YAxisDegree"));
        label_YAxisDegree->setGeometry(QRect(810, 420, 61, 16));
        label_YAxisDegree->setFrameShape(QFrame::WinPanel);
        label_YAxisDegree->setAlignment(Qt::AlignCenter);
        label_ZAxisDegree = new QLabel(centralWidget);
        label_ZAxisDegree->setObjectName(QStringLiteral("label_ZAxisDegree"));
        label_ZAxisDegree->setGeometry(QRect(880, 420, 61, 16));
        label_ZAxisDegree->setFrameShape(QFrame::WinPanel);
        label_ZAxisDegree->setAlignment(Qt::AlignCenter);
        pushButtonXRotation = new QPushButton(centralWidget);
        pushButtonXRotation->setObjectName(QStringLiteral("pushButtonXRotation"));
        pushButtonXRotation->setGeometry(QRect(950, 420, 93, 28));
        pushButtonYRotation = new QPushButton(centralWidget);
        pushButtonYRotation->setObjectName(QStringLiteral("pushButtonYRotation"));
        pushButtonYRotation->setGeometry(QRect(1060, 420, 93, 28));
        pushButtonZRotation = new QPushButton(centralWidget);
        pushButtonZRotation->setObjectName(QStringLiteral("pushButtonZRotation"));
        pushButtonZRotation->setGeometry(QRect(1170, 420, 93, 28));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1287, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "OpenGLDemo", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionCamera_Controls->setText(QApplication::translate("MainWindow", "Camera Controls", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Vertices", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Half-Edges", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Faces", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Vertex Position", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "X", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Y", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Z", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Blue", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "Red", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Face Color", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Green", Q_NULLPTR));
        pushButtonAddVertex->setText(QApplication::translate("MainWindow", "Split this Edge", Q_NULLPTR));
        pushButtonTriangulate->setText(QApplication::translate("MainWindow", "Triangulate", Q_NULLPTR));
        pushButtonSmooth->setText(QApplication::translate("MainWindow", "Smooth", Q_NULLPTR));
        pushButtonExtrude->setText(QApplication::translate("MainWindow", "Extrude", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Joints", Q_NULLPTR));
        pushButtonSkinning->setText(QApplication::translate("MainWindow", "Skinning", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "Local Rotation", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "X Axis", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "Y Axis", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "Z Axis", Q_NULLPTR));
        label_XAxisDegree->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_YAxisDegree->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label_ZAxisDegree->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        pushButtonXRotation->setText(QApplication::translate("MainWindow", "X Rotation", Q_NULLPTR));
        pushButtonYRotation->setText(QApplication::translate("MainWindow", "Y Rotation", Q_NULLPTR));
        pushButtonZRotation->setText(QApplication::translate("MainWindow", "Z Rotation", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
