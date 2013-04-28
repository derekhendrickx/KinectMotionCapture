#include "kinectskeleton.h"

#include <QDebug>

#define PI 3.14159265

KinectSkeleton::KinectSkeleton(QWidget *parent, QGraphicsView *graphicsView)
	: QWidget(parent)
{
	m_pGraphicsView = graphicsView;
	m_pGraphicsScene = new QGraphicsScene(this);
	m_pGraphicsView->setScene(m_pGraphicsScene);
	m_bRecording = false;
	m_bIsCalibrated = false;
	m_pKinectBVH = new KinectBVH();
	m_iWidth = 640;
	m_iHeight = 480;
}

KinectSkeleton::~KinectSkeleton()
{
	delete m_pGraphicsView;
	m_pGraphicsView = NULL;
	delete m_pGraphicsScene;
	m_pGraphicsScene = NULL;
}

void KinectSkeleton::StartRecording()
{
	if (!m_bRecording) {
		m_bRecording = true;
	}
	else {
		m_bRecording = false;
		m_pKinectBVH->FillBVHFile();
	}
}

void KinectSkeleton::CalibrateSkeleton(const NUI_SKELETON_DATA &skeleton)
{
	if (m_pKinectBVH->CreateBVHFile(QString("test.bvh")))
	{
		for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
			m_pKinectBVH->AddOffset(i, skeleton.SkeletonPositions[i]);
		}

		m_bIsCalibrated = true;
	}
}

QPointF KinectSkeleton::SkeletonToScreen(Vector4 skeletonPoint, int width, int height)
{
    LONG x, y;
    USHORT depth;

    // calculate the skeleton's position on the screen
    // NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
    NuiTransformSkeletonToDepthImage( skeletonPoint, &x, &y, &depth );

    float screenPointX = static_cast<float>(x * width) / m_iWidth;
    float screenPointY = static_cast<float>(y * height) / m_iHeight;

	//return m_pGraphicsView->mapFromScene(QPoint (x, y));
	return QPoint (x, y);
}

void KinectSkeleton::DrawBone(const NUI_SKELETON_DATA &skel, NUI_SKELETON_POSITION_INDEX bone0, NUI_SKELETON_POSITION_INDEX bone1)
{
    NUI_SKELETON_POSITION_TRACKING_STATE bone0State = skel.eSkeletonPositionTrackingState[bone0];
    NUI_SKELETON_POSITION_TRACKING_STATE bone1State = skel.eSkeletonPositionTrackingState[bone1];

    // If we can't find either of these joints, exit
    if ( bone0State == NUI_SKELETON_POSITION_NOT_TRACKED || bone1State == NUI_SKELETON_POSITION_NOT_TRACKED )
    {
        return;
    }
    
    // Don't draw if both points are inferred
    if ( bone0State == NUI_SKELETON_POSITION_INFERRED && bone1State == NUI_SKELETON_POSITION_INFERRED )
    {
        return;
    }

    // We assume all drawn bones are inferred unless BOTH joints are tracked
	QPen pen;
	pen.setWidth(2);

    if ( bone0State == NUI_SKELETON_POSITION_TRACKED && bone1State == NUI_SKELETON_POSITION_TRACKED )
    {
		pen.setBrush(Qt::green);
		m_pGraphicsScene->addLine(m_Points[bone0].x(), m_Points[bone0].y(), m_Points[bone1].x(), m_Points[bone1].y(), pen);
    }
    else
    {
		pen.setBrush(Qt::red);
        m_pGraphicsScene->addLine(m_Points[bone0].x(), m_Points[bone0].y(), m_Points[bone1].x(), m_Points[bone1].y(), pen);
    }
}

void KinectSkeleton::DrawSkeleton(const NUI_SKELETON_DATA &skel, int windowWidth, int windowHeight)
{
	m_pGraphicsScene->clear();
	/*m_pGraphicsScene = new QGraphicsScene(this);
	m_pGraphicsView->setScene(m_pGraphicsScene);*/

	/*QBrush brush(Qt::blue);
	QPen pen;
	m_pGraphicsScene->addRect(50, 50, 50, 50, pen, brush);

	NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
	NuiSkeletonCalculateBoneOrientations(&skel, boneOrientations);
	Matrix4 transform = boneOrientations[NUI_SKELETON_POSITION_HAND_RIGHT].absoluteRotation.rotationMatrix;

	QTransform rotation = QTransform(transform.M11, transform.M12, transform.M13, transform.M21, transform.M22, transform.M23, transform.M31, transform.M32, transform.M33);
	m_pGraphicsView->setTransform(rotation, true);*/

    int i;

	for (i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], m_pGraphicsView->width(), m_pGraphicsView->height());

		if(IsRecording() == true && IsCalibrated() == TRUE) {
			NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
			NuiSkeletonCalculateBoneOrientations(&skel, boneOrientations);

			m_pKinectBVH->AddPosition(skel.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER]);

			// Matrices Multiplication
			Matrix4 hipCenter = boneOrientations[NUI_SKELETON_POSITION_SPINE].absoluteRotation.rotationMatrix;
			Matrix4 spine = boneOrientations[NUI_SKELETON_POSITION_SPINE].absoluteRotation.rotationMatrix * hipCenter;
			Matrix4 shoulderCenter = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_CENTER].absoluteRotation.rotationMatrix * spine;
			Matrix4 head = boneOrientations[NUI_SKELETON_POSITION_HEAD].absoluteRotation.rotationMatrix * shoulderCenter;
			Matrix4 shoulderLeft = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_LEFT].absoluteRotation.rotationMatrix * shoulderCenter;
			Matrix4 elbowLeft = boneOrientations[NUI_SKELETON_POSITION_ELBOW_LEFT].absoluteRotation.rotationMatrix * shoulderLeft;
			Matrix4 wristLeft = boneOrientations[NUI_SKELETON_POSITION_WRIST_LEFT].absoluteRotation.rotationMatrix * elbowLeft;
			Matrix4 handLeft = boneOrientations[NUI_SKELETON_POSITION_WRIST_LEFT].absoluteRotation.rotationMatrix * wristLeft;
			Matrix4 shoulderRight = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_RIGHT].absoluteRotation.rotationMatrix * shoulderCenter;
			Matrix4 elbowRight = boneOrientations[NUI_SKELETON_POSITION_ELBOW_RIGHT].absoluteRotation.rotationMatrix * shoulderRight;
			Matrix4 wristRight = boneOrientations[NUI_SKELETON_POSITION_WRIST_RIGHT].absoluteRotation.rotationMatrix * elbowRight;
			Matrix4 handRight = boneOrientations[NUI_SKELETON_POSITION_WRIST_RIGHT].absoluteRotation.rotationMatrix * wristRight;
			Matrix4 hipLeft = boneOrientations[NUI_SKELETON_POSITION_HIP_LEFT].absoluteRotation.rotationMatrix * hipCenter;
			Matrix4 kneeLeft = boneOrientations[NUI_SKELETON_POSITION_KNEE_LEFT].absoluteRotation.rotationMatrix * hipLeft;
			Matrix4 ankleLeft = boneOrientations[NUI_SKELETON_POSITION_ANKLE_LEFT].absoluteRotation.rotationMatrix * kneeLeft;
			Matrix4 footLeft = boneOrientations[NUI_SKELETON_POSITION_FOOT_LEFT].absoluteRotation.rotationMatrix * ankleLeft;
			Matrix4 hipRight = boneOrientations[NUI_SKELETON_POSITION_HIP_RIGHT].absoluteRotation.rotationMatrix * hipCenter;
			Matrix4 kneeRight = boneOrientations[NUI_SKELETON_POSITION_KNEE_RIGHT].absoluteRotation.rotationMatrix * hipRight;
			Matrix4 ankleRight = boneOrientations[NUI_SKELETON_POSITION_ANKLE_RIGHT].absoluteRotation.rotationMatrix * kneeRight;
			Matrix4 footRight = boneOrientations[NUI_SKELETON_POSITION_FOOT_RIGHT].absoluteRotation.rotationMatrix * ankleRight;

			m_pKinectBVH->AddMotionFrame(hipCenter);
			m_pKinectBVH->AddMotionFrame(spine);
			m_pKinectBVH->AddMotionFrame(shoulderCenter);
			m_pKinectBVH->AddMotionFrame(head);
			m_pKinectBVH->AddMotionFrame(shoulderLeft);
			m_pKinectBVH->AddMotionFrame(elbowLeft);
			m_pKinectBVH->AddMotionFrame(wristLeft);
			m_pKinectBVH->AddMotionFrame(handLeft);
			m_pKinectBVH->AddMotionFrame(shoulderRight);
			m_pKinectBVH->AddMotionFrame(elbowRight);
			m_pKinectBVH->AddMotionFrame(wristRight);
			m_pKinectBVH->AddMotionFrame(handRight);
			m_pKinectBVH->AddMotionFrame(hipLeft);
			m_pKinectBVH->AddMotionFrame(kneeLeft);
			m_pKinectBVH->AddMotionFrame(ankleLeft);
			m_pKinectBVH->AddMotionFrame(footLeft);
			m_pKinectBVH->AddMotionFrame(hipRight);
			m_pKinectBVH->AddMotionFrame(kneeRight);
			m_pKinectBVH->AddMotionFrame(ankleRight);
			m_pKinectBVH->AddMotionFrame(footRight);

			//for (int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++) {
			//	m_pKinectBVH->AddMotionFrame(boneOrientations[j].absoluteRotation.rotationMatrix);
			//	//m_pKinectBVH->AddQuaternion(boneOrientations[j].hierarchicalRotation.rotationQuaternion);
			//}
			m_pKinectBVH->IncrementNbFrames();
		}
	}

    // Render Torso
    DrawBone(skel, NUI_SKELETON_POSITION_HEAD, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_CENTER, NUI_SKELETON_POSITION_SPINE);
    DrawBone(skel, NUI_SKELETON_POSITION_SPINE, NUI_SKELETON_POSITION_HIP_CENTER);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_CENTER, NUI_SKELETON_POSITION_HIP_RIGHT);

    // Left Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_LEFT, NUI_SKELETON_POSITION_ELBOW_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_LEFT, NUI_SKELETON_POSITION_WRIST_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_LEFT, NUI_SKELETON_POSITION_HAND_LEFT);

    // Right Arm
    DrawBone(skel, NUI_SKELETON_POSITION_SHOULDER_RIGHT, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_ELBOW_RIGHT, NUI_SKELETON_POSITION_WRIST_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_WRIST_RIGHT, NUI_SKELETON_POSITION_HAND_RIGHT);

    // Left Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_LEFT, NUI_SKELETON_POSITION_KNEE_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_LEFT, NUI_SKELETON_POSITION_ANKLE_LEFT);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_LEFT, NUI_SKELETON_POSITION_FOOT_LEFT);

    // Right Leg
    DrawBone(skel, NUI_SKELETON_POSITION_HIP_RIGHT, NUI_SKELETON_POSITION_KNEE_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_KNEE_RIGHT, NUI_SKELETON_POSITION_ANKLE_RIGHT);
    DrawBone(skel, NUI_SKELETON_POSITION_ANKLE_RIGHT, NUI_SKELETON_POSITION_FOOT_RIGHT);
    
    // Draw the joints in a different color
    for (i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
    {
		QBrush brush(Qt::blue);
		QPen pen;
		pen.setWidth(3);
        if (skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED)
        {
			m_pGraphicsScene->addEllipse(m_Points[i].x(), m_Points[i].y(), g_JointThickness, g_JointThickness, pen, brush);
        }
        else if (skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_TRACKED)
        {
            m_pGraphicsScene->addEllipse(m_Points[i].x(), m_Points[i].y(), g_JointThickness, g_JointThickness, pen, brush);
        }
    }
}
