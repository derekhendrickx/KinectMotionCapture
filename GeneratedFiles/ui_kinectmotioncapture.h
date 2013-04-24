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
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KinectMotionCaptureClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *record;
    QGraphicsView *skeleton;

    void setupUi(QMainWindow *KinectMotionCaptureClass)
    {
        if (KinectMotionCaptureClass->objectName().isEmpty())
            KinectMotionCaptureClass->setObjectName(QStringLiteral("KinectMotionCaptureClass"));
        KinectMotionCaptureClass->resize(768, 528);
        centralWidget = new QWidget(KinectMotionCaptureClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        record = new QPushButton(centralWidget);
        record->setObjectName(QStringLiteral("record"));

        verticalLayout->addWidget(record);

        skeleton = new QGraphicsView(centralWidget);
        skeleton->setObjectName(QStringLiteral("skeleton"));
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        skeleton->setBackgroundBrush(brush);

        verticalLayout->addWidget(skeleton);

        KinectMotionCaptureClass->setCentralWidget(centralWidget);

        retranslateUi(KinectMotionCaptureClass);

        QMetaObject::connectSlotsByName(KinectMotionCaptureClass);
    } // setupUi

    void retranslateUi(QMainWindow *KinectMotionCaptureClass)
    {
        KinectMotionCaptureClass->setWindowTitle(QApplication::translate("KinectMotionCaptureClass", "Kinect Motion Capture", 0));
        record->setText(QApplication::translate("KinectMotionCaptureClass", "Record", 0));
    } // retranslateUi

};

namespace Ui {
    class KinectMotionCaptureClass: public Ui_KinectMotionCaptureClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KINECTMOTIONCAPTURE_H
