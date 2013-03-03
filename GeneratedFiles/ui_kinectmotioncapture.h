/********************************************************************************
** Form generated from reading UI file 'kinectmotioncapture.ui'
**
** Created by: Qt User Interface Compiler version 5.0.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KINECTMOTIONCAPTURE_H
#define UI_KINECTMOTIONCAPTURE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectMotionCaptureClass
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *video;
    QLabel *skeleton;
    QPushButton *pushButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KinectMotionCaptureClass)
    {
        if (KinectMotionCaptureClass->objectName().isEmpty())
            KinectMotionCaptureClass->setObjectName(QStringLiteral("KinectMotionCaptureClass"));
        KinectMotionCaptureClass->resize(768, 528);
        centralWidget = new QWidget(KinectMotionCaptureClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 741, 484));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        video = new QLabel(verticalLayoutWidget);
        video->setObjectName(QStringLiteral("video"));
        video->setMinimumSize(QSize(320, 240));
        video->setMaximumSize(QSize(320, 240));

        horizontalLayout->addWidget(video);

        skeleton = new QLabel(verticalLayoutWidget);
        skeleton->setObjectName(QStringLiteral("skeleton"));
        skeleton->setMinimumSize(QSize(320, 240));
        skeleton->setMaximumSize(QSize(320, 240));

        horizontalLayout->addWidget(skeleton);


        verticalLayout->addLayout(horizontalLayout);

        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout->addWidget(pushButton);

        KinectMotionCaptureClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(KinectMotionCaptureClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        KinectMotionCaptureClass->setStatusBar(statusBar);

        retranslateUi(KinectMotionCaptureClass);

        QMetaObject::connectSlotsByName(KinectMotionCaptureClass);
    } // setupUi

    void retranslateUi(QMainWindow *KinectMotionCaptureClass)
    {
        KinectMotionCaptureClass->setWindowTitle(QApplication::translate("KinectMotionCaptureClass", "Kinect Motion Capture", 0));
        video->setText(QString());
        skeleton->setText(QString());
        pushButton->setText(QApplication::translate("KinectMotionCaptureClass", "Record", 0));
    } // retranslateUi

};

namespace Ui {
    class KinectMotionCaptureClass: public Ui_KinectMotionCaptureClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTMOTIONCAPTURE_H
