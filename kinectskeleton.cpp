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

    int i;

	for (i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
	{
		m_Points[i] = SkeletonToScreen(skel.SkeletonPositions[i], m_pGraphicsView->width(), m_pGraphicsView->height());
	}

	if (IsRecording() == true && IsCalibrated() == TRUE)
	{
		ProcessBonesOrientation(skel);
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

	/*if(IsRecording() == true && IsCalibrated() == TRUE) {
		ProcessBonesOrientation(skel);
	}*/
}

void KinectSkeleton::ProcessBonesOrientation(const NUI_SKELETON_DATA &skel)
{
	NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
	NuiSkeletonCalculateBoneOrientations(&skel, boneOrientations);

	m_pKinectBVH->AddPosition(skel.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER]);

	KinectNode root, spine, shoulderCenter, head, shoulderLeft, elbowLeft, wristLeft, handLeft, shoulderRight, elbowRight, wristRight, handRight, hipLeft, kneeLeft, ankleLeft, footLeft, hipRight, kneeRight, ankleRight, footRight;
	root.parent = NULL;
	Matrix4 rotationMatrix;
	Matrix3f matrix;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HIP_CENTER].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	root.transform = matrix;
	root.inverseTransform = matrix.inverse();
	root.sons = (KinectNode *) malloc(3 * sizeof(KinectNode));

	spine.parent = &root;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_SPINE].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	spine.transform = matrix;
	spine.inverseTransform = matrix.inverse();
	spine.localTransform = matrix * root.inverseTransform;
	root.sons[0] = spine;

	shoulderCenter.parent = &spine;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_CENTER].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	shoulderCenter.transform = matrix;
	shoulderCenter.inverseTransform = matrix.inverse();
	shoulderCenter.localTransform = matrix * spine.inverseTransform * root.inverseTransform;
	spine.sons = &shoulderCenter;

	head.parent = &shoulderCenter;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HEAD].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	head.transform = matrix;
	head.inverseTransform = matrix.inverse();
	head.localTransform = matrix * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	shoulderCenter.sons = (KinectNode*) malloc(3 * sizeof(KinectNode));
	shoulderCenter.sons[0] = head;

	shoulderLeft.parent = &shoulderCenter;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	shoulderLeft.transform = matrix;
	shoulderLeft.inverseTransform = matrix.inverse();
	shoulderLeft.localTransform = matrix * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	shoulderCenter.sons[1] = shoulderLeft;

	elbowLeft.parent = &shoulderLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_ELBOW_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	elbowLeft.transform = matrix;
	elbowLeft.inverseTransform = matrix.inverse();
	elbowLeft.localTransform = matrix * shoulderLeft.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	shoulderLeft.sons = &elbowLeft;

	wristLeft.parent = &elbowLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_WRIST_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	wristLeft.transform = matrix;
	wristLeft.inverseTransform = matrix.inverse();
	wristLeft.localTransform = matrix * elbowLeft.inverseTransform * shoulderLeft.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	elbowLeft.sons = &wristLeft;

	handLeft.parent = &wristLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HAND_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	handLeft.transform = matrix;
	handLeft.inverseTransform = matrix.inverse();
	handLeft.localTransform = matrix * wristLeft.inverseTransform * elbowLeft.inverseTransform * shoulderLeft.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	wristLeft.sons = &handLeft;

	shoulderRight.parent = &shoulderCenter;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_SHOULDER_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	shoulderRight.transform = matrix;
	shoulderRight.inverseTransform = matrix.inverse();
	shoulderRight.localTransform = matrix * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	shoulderCenter.sons[2] = shoulderRight;

	elbowRight.parent = &shoulderRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_ELBOW_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	elbowRight.transform = matrix;
	elbowRight.inverseTransform = matrix.inverse();
	elbowRight.localTransform = matrix * shoulderRight.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	shoulderRight.sons = &elbowRight;

	wristRight.parent = &elbowRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_WRIST_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	wristRight.transform = matrix;
	wristRight.inverseTransform = matrix.inverse();
	wristRight.localTransform = matrix * elbowRight.inverseTransform * shoulderRight.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	elbowRight.sons = &wristRight;

	handRight.parent = &wristRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HAND_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	handRight.transform = matrix;
	handRight.inverseTransform = matrix.inverse();
	handRight.localTransform = matrix * wristRight.inverseTransform * elbowRight.inverseTransform * shoulderRight.inverseTransform * shoulderCenter.inverseTransform * spine.inverseTransform * root.inverseTransform;
	wristRight.sons = &handRight;

	hipLeft.parent = &root;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HIP_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	hipLeft.transform = matrix;
	hipLeft.inverseTransform = matrix.inverse();
	hipLeft.localTransform = matrix * root.inverseTransform;
	root.sons[1] = hipLeft;

	kneeLeft.parent = &hipLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_KNEE_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	kneeLeft.transform = matrix;
	kneeLeft.inverseTransform = matrix.inverse();
	kneeLeft.localTransform = matrix * hipLeft.inverseTransform * root.inverseTransform;
	hipLeft.sons = &kneeLeft;

	ankleLeft.parent = &kneeLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_ANKLE_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	ankleLeft.transform = matrix;
	ankleLeft.inverseTransform = matrix.inverse();
	ankleLeft.localTransform = matrix * kneeLeft.inverseTransform * hipLeft.inverseTransform * root.inverseTransform;
	kneeLeft.sons = &ankleLeft;

	footLeft.parent = &ankleLeft;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_FOOT_LEFT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	footLeft.transform = matrix;
	footLeft.inverseTransform = matrix.inverse();
	footLeft.localTransform = matrix * ankleLeft.inverseTransform * kneeLeft.inverseTransform * hipLeft.inverseTransform * root.inverseTransform;
	ankleLeft.sons = &footLeft;

	hipRight.parent = &root;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_HIP_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	hipRight.transform = matrix;
	hipRight.inverseTransform = matrix.inverse();
	hipRight.localTransform = matrix * root.inverseTransform;
	root.sons[2] = hipRight;

	kneeRight.parent = &hipRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_KNEE_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	kneeRight.transform = matrix;
	kneeRight.inverseTransform = matrix.inverse();
	kneeRight.localTransform = matrix * hipRight.inverseTransform * root.inverseTransform;
	hipRight.sons = &kneeRight;

	ankleRight.parent = &kneeRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_ANKLE_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	ankleRight.transform = matrix;
	ankleRight.inverseTransform = matrix.inverse();
	ankleRight.localTransform = matrix * kneeRight.inverseTransform * hipRight.inverseTransform * root.inverseTransform;
	kneeRight.sons = &ankleRight;

	footRight.parent = &ankleRight;
	rotationMatrix = boneOrientations[NUI_SKELETON_POSITION_FOOT_RIGHT].hierarchicalRotation.rotationMatrix;
	matrix << rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13,
				rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23,
				rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33;
	footRight.transform = matrix;
	footRight.inverseTransform = matrix.inverse();
	footRight.localTransform = matrix * ankleRight.inverseTransform * kneeRight.inverseTransform * hipRight.inverseTransform * root.inverseTransform;
	ankleRight.sons = &footRight;

	KinectNode joints[NUI_SKELETON_POSITION_COUNT];
	joints[NUI_SKELETON_POSITION_HIP_CENTER] = root;
	joints[NUI_SKELETON_POSITION_SPINE] = spine;
	joints[NUI_SKELETON_POSITION_SHOULDER_CENTER] = shoulderCenter;
	joints[NUI_SKELETON_POSITION_HEAD] = head;
	joints[NUI_SKELETON_POSITION_SHOULDER_LEFT] = shoulderLeft;
	joints[NUI_SKELETON_POSITION_ELBOW_LEFT] = elbowLeft;
	joints[NUI_SKELETON_POSITION_WRIST_LEFT] = wristLeft;
	joints[NUI_SKELETON_POSITION_HAND_LEFT] = handLeft;
	joints[NUI_SKELETON_POSITION_SHOULDER_RIGHT] = shoulderRight;
	joints[NUI_SKELETON_POSITION_ELBOW_RIGHT] = elbowRight;
	joints[NUI_SKELETON_POSITION_WRIST_RIGHT] = wristRight;
	joints[NUI_SKELETON_POSITION_HAND_RIGHT] = handRight;
	joints[NUI_SKELETON_POSITION_HIP_LEFT] = hipLeft;
	joints[NUI_SKELETON_POSITION_KNEE_LEFT] = kneeLeft;
	joints[NUI_SKELETON_POSITION_ANKLE_LEFT] = ankleLeft;
	joints[NUI_SKELETON_POSITION_FOOT_LEFT] = footLeft;
	joints[NUI_SKELETON_POSITION_HIP_RIGHT] = hipRight;
	joints[NUI_SKELETON_POSITION_KNEE_RIGHT] = kneeRight;
	joints[NUI_SKELETON_POSITION_ANKLE_RIGHT] = ankleRight;
	joints[NUI_SKELETON_POSITION_FOOT_RIGHT] = footRight;

	m_pKinectBVH->AddBonesOrientation(joints);

	m_pKinectBVH->IncrementNbFrames();
}
