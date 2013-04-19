#include "kinectskeleton.h"

#include <QDebug>
#include <cmath>

#define PI 3.14159265

KinectSkeleton::KinectSkeleton(QWidget *parent, QLabel *label, int width, int height)
	: QWidget(parent)
{
	m_pLabel = label;
	m_iWidth = width;
	m_iHeight = height;
	m_pPixmap = new QPixmap(m_iWidth, m_iHeight);
	m_pPixmap->fill(Qt::black);
	m_pPainter = new QPainter(m_pPixmap);
	m_bRecording = false;
	m_bIsCalibrated = false;
	m_pKinectBVH = new KinectBVH();
}

KinectSkeleton::~KinectSkeleton()
{
	delete m_pPainter;
	delete m_pPixmap;
}

void KinectSkeleton::StartRecording()
{
	if (!m_bRecording) {
		m_bRecording = true;
	}
	else {
		m_bRecording = false;
	}
}

void KinectSkeleton::CalibrateSkeleton(const NUI_SKELETON_DATA &skeleton)
{
	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++) {
		m_pKinectBVH->AddOffset(i, skeleton.SkeletonPositions[i]);
	}

	m_bIsCalibrated = true;
	m_pKinectBVH->CreateBVHFile(QString("test.bvh"));
	m_pKinectBVH->CreateSkeletonInformation();
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

    return QPoint(screenPointX, screenPointY);
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
		m_pPainter->setPen(pen);
		m_pPainter->drawLine(m_Points[bone0], m_Points[bone1]);
    }
    else
    {
		pen.setBrush(Qt::red);
		m_pPainter->setPen(pen);
        m_pPainter->drawLine(m_Points[bone0], m_Points[bone1]);
    }
}

void KinectSkeleton::DrawSkeleton(const NUI_SKELETON_DATA &skel, int windowWidth, int windowHeight)
{
	Clear();

    int i;

	for (i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], windowWidth, windowHeight);

		if(IsRecording() == true && IsCalibrated() == TRUE) {
			NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
			NuiSkeletonCalculateBoneOrientations(&skel, boneOrientations);

			for(int j = 0; j < NUI_SKELETON_POSITION_COUNT; j++){
				m_pKinectBVH->AddMotionFrame(boneOrientations[j].hierarchicalRotation.rotationMatrix);
			}

			/*hip_position[numFrame].x = nuiSkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x;
			hip_position[numFrame].y = nuiSkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y;
			hip_position[numFrame].z = nuiSkeletonFrame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z;*/
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
		QPen pen;
		pen.setWidth(3);
		pen.setBrush(Qt::blue);
		m_pPainter->setPen(pen);
        if (skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_INFERRED)
        {
			m_pPainter->drawEllipse(m_Points[i], g_JointThickness, g_JointThickness);
        }
        else if (skel.eSkeletonPositionTrackingState[i] == NUI_SKELETON_POSITION_TRACKED)
        {
            m_pPainter->drawEllipse(m_Points[i], g_JointThickness, g_JointThickness);
        }
    }

	update();
}

void KinectSkeleton::Clear()
{
	m_pPainter->eraseRect(0, 0, m_iWidth, m_iHeight);
	m_pPixmap->fill(Qt::black);
}

void KinectSkeleton::paintEvent(QPaintEvent *e)
{
	m_pLabel->setPixmap(*m_pPixmap);
}
