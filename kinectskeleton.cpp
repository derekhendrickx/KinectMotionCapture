#include "kinectskeleton.h"

/**
* Constructeur prenant en paramètre le widget parent et la zone d'affichage du squelette
*/
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

/**
* Destructeur
*/
KinectSkeleton::~KinectSkeleton()
{
	delete m_pGraphicsView;
	m_pGraphicsView = NULL;
	delete m_pGraphicsScene;
	m_pGraphicsScene = NULL;
}

/**
* Démarre et stop la capture de mouvements
*/
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

/**
* Envoi des données initiales pour la descrition du squelette
*/
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

/**
* Mise à l'échelle des points du squelette en fonction de la zone d'affichage
*/
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

/**
* Affichage d'un membre du squelette
*/
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

/**
* Affiche le squelette sur l'IHM
*/
void KinectSkeleton::DrawSkeleton(const NUI_SKELETON_DATA &skel)
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
}

/**
* Conversion et envoi des matrices de rotation pour la génération du fichier BVH
*/
void KinectSkeleton::ProcessBonesOrientation(const NUI_SKELETON_DATA &skel)
{
	NUI_SKELETON_BONE_ORIENTATION boneOrientations[NUI_SKELETON_POSITION_COUNT];
	NuiSkeletonCalculateBoneOrientations(&skel, boneOrientations);
	KinectJoint joints[NUI_SKELETON_POSITION_COUNT];

	// Position de Hip Center
	m_pKinectBVH->AddPosition(skel.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER]);

	// Matrice de rotations des joints
	for (int i = 0; i < NUI_SKELETON_POSITION_COUNT; i++)
    {
        NUI_SKELETON_BONE_ORIENTATION &orientation = boneOrientations[i];

		KinectJoint joint;
		Matrix4 jointRotationMatrix;
		Matrix3f scaleMatrix, rotationMatrix;

		jointRotationMatrix = orientation.hierarchicalRotation.rotationMatrix;
		rotationMatrix <<	jointRotationMatrix.M11, jointRotationMatrix.M12, jointRotationMatrix.M13,
							jointRotationMatrix.M21, jointRotationMatrix.M22, jointRotationMatrix.M23,
							jointRotationMatrix.M31, jointRotationMatrix.M32, jointRotationMatrix.M33;
		joint.transform = rotationMatrix;
		joint.quaternion = orientation.absoluteRotation.rotationQuaternion;
		// Conversion en angles d'Euler selon l'ordre ZXY
		joint.angles = joint.transform.eulerAngles(2, 0, 1);

		joints[i] = joint;
    }

	m_pKinectBVH->AddBonesOrientation(joints);

	// Incrémentation du nombre de frames
	m_pKinectBVH->IncrementNbFrames();
}
