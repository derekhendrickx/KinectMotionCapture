#include "kinectskeleton.h"

#include <QDebug>

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

bool KinectSkeleton::CalibrateSkeleton(const NUI_SKELETON_DATA &skeleton)
{
	// Torso
	Vector4 hipCenter = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	Vector4 spine = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE];
	Vector4 shoulderCenter = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER];
	Vector4 head = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD];

	// Left Arm
	Vector4 shoulderLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT];
	Vector4 elbowLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT];
	Vector4 wristLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
	Vector4 handLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT];

	// Right Arm
	Vector4 shoulderRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT];
	Vector4 elbowRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT];
	Vector4 wristRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	Vector4 handRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];

	// Left Leg
	Vector4 hipLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT];
	Vector4 kneeLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT];
	Vector4 ankleLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT];
	Vector4 footLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT];

	// Right Leg
	Vector4 hipRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT];
	Vector4 kneeRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT];
	Vector4 ankleRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT];
	Vector4 footRight = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT];

	bool torso = false, leftArm = false, rightArm = false, leftLeg = false, rightLeg = false;

	// Torso
	qDebug() << "HIP CENTER (" << hipCenter.x << ", " << hipCenter.y << ", " << hipCenter.z << ")" << endl;
	qDebug() << "SPINE (" << spine.x << ", " << spine.y << ", " << spine.z << ")" << endl;
	if (hipCenter.x - 0.01f == spine.x  - 0.01f || hipCenter.x + 0.01f == spine.x  + 0.01f)
	{
		qDebug() << "OK" << endl;
		if (hipCenter.x == shoulderCenter.x && hipCenter.z == shoulderCenter.z)
		{
			if (hipCenter.x == head.x && hipCenter.z == head.z)
			{
				m_pKinectBVH->AddOffset(hipCenter);
				m_pKinectBVH->AddOffset(spine);
				m_pKinectBVH->AddOffset(shoulderCenter);
				m_pKinectBVH->AddOffset(head);
				torso = true;
				OutputDebugString(L"TORSO CALIBRATED");
			}
		}
	}

	// Left Arm
	if (torso)
	{
		if (shoulderCenter.y == shoulderLeft.y && shoulderCenter.z == shoulderLeft.z)
		{
			if (shoulderCenter.y == elbowLeft.y && shoulderCenter.z == elbowLeft.z)
			{
				if (shoulderCenter.y == wristLeft.y && shoulderCenter.z == wristLeft.z)
				{
					if (shoulderCenter.y == handLeft.y && shoulderCenter.z == handLeft.z)
					{
						m_pKinectBVH->AddOffset(shoulderLeft);
						m_pKinectBVH->AddOffset(elbowLeft);
						m_pKinectBVH->AddOffset(wristLeft);
						m_pKinectBVH->AddOffset(handLeft);
						leftArm = true;
					}
				}
			}
		}
	}

    // Right Arm
	if (torso && leftArm)
	{
		if (shoulderCenter.y == shoulderRight.y && shoulderCenter.z == shoulderRight.z)
		{
			if (shoulderCenter.y == elbowRight.y && shoulderCenter.z == elbowRight.z)
			{
				if (shoulderCenter.y == wristRight.y && shoulderCenter.z == wristRight.z)
				{
					if (shoulderCenter.y == handRight.y && shoulderCenter.z == handRight.z)
					{
						m_pKinectBVH->AddOffset(shoulderRight);
						m_pKinectBVH->AddOffset(elbowRight);
						m_pKinectBVH->AddOffset(wristRight);
						m_pKinectBVH->AddOffset(handRight);
						rightArm = true;
					}
				}
			}
		}
	}

    // Left Leg
	if (torso && leftArm && rightArm)
	{
		if (hipCenter.y == hipLeft.y && hipCenter.z == hipLeft.z)
		{
			if (hipLeft.x == kneeLeft.x && hipLeft.z == kneeLeft.z)
			{
				if (hipLeft.x == ankleLeft.x && hipLeft.z == ankleLeft.z)
				{
					if (hipLeft.x == footLeft.x && hipLeft.z == footLeft.z)
					{
						m_pKinectBVH->AddOffset(hipLeft);
						m_pKinectBVH->AddOffset(kneeLeft);
						m_pKinectBVH->AddOffset(ankleLeft);
						m_pKinectBVH->AddOffset(footLeft);
						leftLeg = true;
					}
				}
			}
		}
	}

    // Right Leg
    if (torso && leftArm && rightArm && leftLeg)
	{
		if (hipCenter.y == hipRight.y && hipCenter.z == hipRight.z)
		{
			if (hipRight.x == kneeRight.x && hipRight.z == kneeRight.z)
			{
				if (hipRight.x == ankleRight.x && hipRight.z == ankleRight.z)
				{
					if (hipRight.x == footRight.x && hipRight.z == footRight.z)
					{
						m_pKinectBVH->AddOffset(hipRight);
						m_pKinectBVH->AddOffset(kneeRight);
						m_pKinectBVH->AddOffset(ankleRight);
						m_pKinectBVH->AddOffset(footRight);
						rightLeg = true;
					}
				}
			}
		}
	}

	if (torso && leftArm && rightArm && leftLeg && rightLeg)
	{
		OutputDebugString(L"CALIBRATED");
		m_bIsCalibrated = true;
		return true;
	}
	return false;
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
