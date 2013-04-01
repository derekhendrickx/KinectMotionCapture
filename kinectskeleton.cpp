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

bool KinectSkeleton::CalibrateSkeleton(const NUI_SKELETON_DATA &skeleton)
{
	// Torso
	KinectVector4 hipCenter = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER]);
	KinectVector4 spine = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE]);
	KinectVector4 shoulderCenter = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER]);
	KinectVector4 head = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD]);

	// Left Arm
	KinectVector4 shoulderLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT]);
	KinectVector4 elbowLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT]);
	KinectVector4 wristLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT]);
	KinectVector4 handLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT]);

	// Right Arm
	KinectVector4 shoulderRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT]);
	KinectVector4 elbowRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT]);
	KinectVector4 wristRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT]);
	KinectVector4 handRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT]);

	// Left Leg
	KinectVector4 hipLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT]);
	KinectVector4 kneeLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT]);
	KinectVector4 ankleLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT]);
	KinectVector4 footLeft = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT]);

	// Right Leg
	KinectVector4 hipRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT]);
	KinectVector4 kneeRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT]);
	KinectVector4 ankleRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_RIGHT]);
	KinectVector4 footRight = KinectVector4(skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT]);

	bool torso = false, leftArm = false, rightArm = false, leftLeg = false, rightLeg = false;

	//// Torso
	//float angleHipCenter = (spine - hipCenter).angle(shoulderCenter - hipCenter) * 180.0 / PI;
	//float angleSpine = (hipCenter - spine).angle(shoulderCenter - spine) * 180.0 / PI;
	//float angleShoulderCenter = (hipCenter - shoulderCenter).angle(spine - shoulderCenter) * 180.0 / PI;
	//float angleHead = (shoulderCenter - head).angle(spine - head) * 180.0 / PI;

	//if ((angleHipCenter >= 0.0 && angleHipCenter <= 15.0) && (angleSpine >= 160.0 && angleSpine <= 180.0) && (angleShoulderCenter >= 0.0 && angleShoulderCenter <= 15.0) && (angleHead >= 0.0 && angleHead <= 15.0))
	//{
	//	/*m_pKinectBVH->AddOffset(hipCenter);
	//	m_pKinectBVH->AddOffset(spine);
	//	m_pKinectBVH->AddOffset(shoulderCenter);
	//	m_pKinectBVH->AddOffset(head);*/
	//	torso = true;
	//	qDebug() << "TORSO CALIBRATED" << endl;
	//}

	//// Left Arm
	//float angleShoulderLeft = (elbowLeft - shoulderLeft).angle(wristLeft - shoulderLeft) * 180.0 / PI;
	//float angleElbowLeft = (shoulderLeft - elbowLeft).angle(wristLeft - elbowLeft) * 180.0 / PI;
	//float angleWristLeft = (elbowLeft - wristLeft).angle(shoulderLeft - wristLeft) * 180.0 / PI;
	//float angleHandLeft = (elbowLeft - handLeft).angle(wristLeft - handLeft) * 180.0 / PI;

	//if ((angleShoulderLeft >= 0.0 && angleShoulderLeft <= 15.0) && (angleElbowLeft >= 160.0 && angleElbowLeft <= 180.0) && (angleWristLeft >= 0.0 && angleWristLeft <= 15.0) && (angleHandLeft >= 0.0 && angleHandLeft <= 15.0))
	//{
	//	/*m_pKinectBVH->AddOffset(angleShoulderLeft);
	//	m_pKinectBVH->AddOffset(angleElbowLeft);
	//	m_pKinectBVH->AddOffset(angleWristLeft);
	//	m_pKinectBVH->AddOffset(angleHandLeft);*/
	//	leftArm = true;
	//	qDebug() << "LEFT ARM CALIBRATED" << endl;
	//}

 //   // Right Arm
	//float angleShoulderRight = (elbowRight - shoulderRight).angle(wristRight - shoulderRight) * 180.0 / PI;
	//float angleElbowRight = (shoulderRight - elbowRight).angle(wristRight - elbowRight) * 180.0 / PI;
	//float angleWristRight = (elbowRight - wristRight).angle(shoulderRight - wristRight) * 180.0 / PI;
	//float angleHandRight = (elbowRight - handRight).angle(wristRight - handRight) * 180.0 / PI;

	//if ((angleShoulderRight >= 0.0 && angleShoulderRight <= 15.0) && (angleElbowRight >= 160.0 && angleElbowRight <= 180.0) && (angleWristRight >= 0.0 && angleWristRight <= 15.0) && (angleHandRight >= 0.0 && angleHandRight <= 15.0))
	//{
	//	/*m_pKinectBVH->AddOffset(angleShoulderRight);
	//	m_pKinectBVH->AddOffset(angleElbowRight);
	//	m_pKinectBVH->AddOffset(angleWristRight);
	//	m_pKinectBVH->AddOffset(angleHandRight);*/
	//	rightArm = true;
	//	qDebug() << "RIGHT ARM CALIBRATED" << endl;
	//}

 //   // Left Leg
	//float angleHipLeft = (kneeLeft - hipLeft).angle(ankleLeft - hipLeft) * 180.0 / PI;
	//float angleKneeLeft = (hipLeft - kneeLeft).angle(ankleLeft - kneeLeft) * 180.0 / PI;
	//float angleAnkleLeft = (kneeLeft - ankleLeft).angle(hipLeft - ankleLeft) * 180.0 / PI;
	//float angleFootLeft = (kneeLeft - footLeft).angle(ankleLeft - footLeft) * 180.0 / PI;

	//if ((angleHipLeft >= 0.0 && angleHipLeft <= 15.0) && (angleKneeLeft >= 160.0 && angleKneeLeft <= 180.0) && (angleAnkleLeft >= 0.0 && angleAnkleLeft <= 15.0) && (angleFootLeft >= 0.0 && angleFootLeft <= 15.0))
	//{
	//	/*m_pKinectBVH->AddOffset(angleHipLeft);
	//	m_pKinectBVH->AddOffset(angleKneeLeft);
	//	m_pKinectBVH->AddOffset(angleAnkleLeft);
	//	m_pKinectBVH->AddOffset(angleFootLeft);*/
	//	leftLeg = true;
	//	qDebug() << "LEFT LEG CALIBRATED" << endl;
	//}

 //   // Right Leg
 //   float angleHipRight = (kneeRight - hipRight).angle(ankleRight - hipRight) * 180.0 / PI;
	//float angleKneeRight = (hipRight - kneeRight).angle(ankleRight - kneeRight) * 180.0 / PI;
	//float angleAnkleRight = (kneeRight - ankleRight).angle(hipRight - ankleRight) * 180.0 / PI;
	//float angleFootRight = (kneeRight - footRight).angle(ankleRight - footRight) * 180.0 / PI;

	//if ((angleHipRight >= 0.0 && angleHipRight <= 15.0) && (angleKneeRight >= 160.0 && angleKneeRight <= 180.0) && (angleAnkleRight >= 0.0 && angleAnkleRight <= 15.0) && (angleFootRight >= 0.0 && angleFootRight <= 15.0))
	//{
	//	/*m_pKinectBVH->AddOffset(angleHipRight);
	//	m_pKinectBVH->AddOffset(angleKneeRight);
	//	m_pKinectBVH->AddOffset(angleAnkleRight);
	//	m_pKinectBVH->AddOffset(angleFootRight);*/
	//	rightLeg = true;
	//	qDebug() << "RIGHT LEG CALIBRATED" << endl;
	//}

	//if (torso && leftArm && rightArm && leftLeg && rightLeg)
	//{
	//	qDebug() << "CALIBRATED" << endl;
	//	m_bIsCalibrated = true;
	//	return true;
	//}

	// Torso
	m_pKinectBVH->AddOffset(hipCenter);
	m_pKinectBVH->AddOffset(spine);
	m_pKinectBVH->AddOffset(shoulderCenter);
	m_pKinectBVH->AddOffset(head);

	// Left Arm
	m_pKinectBVH->AddOffset(shoulderLeft);
	m_pKinectBVH->AddOffset(elbowLeft);
	m_pKinectBVH->AddOffset(wristLeft);
	m_pKinectBVH->AddOffset(handLeft);

	// Right Arm
	m_pKinectBVH->AddOffset(shoulderRight);
	m_pKinectBVH->AddOffset(elbowRight);
	m_pKinectBVH->AddOffset(wristRight);
	m_pKinectBVH->AddOffset(handRight);

	// Left Leg
	m_pKinectBVH->AddOffset(hipLeft);
	m_pKinectBVH->AddOffset(kneeLeft);
	m_pKinectBVH->AddOffset(ankleLeft);
	m_pKinectBVH->AddOffset(footLeft);

	// Right Leg
	m_pKinectBVH->AddOffset(hipRight);
	m_pKinectBVH->AddOffset(kneeRight);
	m_pKinectBVH->AddOffset(ankleRight);
	m_pKinectBVH->AddOffset(footRight);

	return true;
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
