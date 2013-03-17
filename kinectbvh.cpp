#include "kinectbvh.h"

KinectBVH::KinectBVH()
{
	
}

KinectBVH::~KinectBVH()
{
	m_pFile->close();
	delete m_pFile;
	m_pFile = NULL;
}

bool KinectBVH::CreateBVHFile(QString filename)
{
	// création d'un objet qfile
	m_pFile = new QFile(filename);
	// on ouvre notre fichier en lecture seule et on vérifie l'ouverture
	if (!m_pFile->open(QIODevice::Append | QIODevice::Text)) {
		return false;
	}
	return true;
}

void KinectBVH::CreateSkeletonInformation(const NUI_SKELETON_DATA &skeleton)
{
	// création d'un objet qtextstream à partir de notre objet qfile
	QTextStream flux(m_pFile);
	// on choisit le codec correspondant au jeu de caractère que l'on souhaite ; ici, utf-8
	flux.setCodec("utf-8");

	// ROOT
	flux << "HIERARCHY" << endl;
	flux << "ROOT Hips" << endl;
	flux << "{" << endl;

		// Chest
		flux << "\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER].z << endl;
		flux << "\tCHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation" << endl;
		flux << "\tJOINT Chest" << endl;
		flux << "\t{" << endl;

			// Neck
			flux << "\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT Neck" << endl;
			flux << "\t\t{" << endl;
				// Head
				flux << "\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT Head" << endl;
				flux << "\t\t\t{" << endl;
					// End Site
					flux << "\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tEnd Site" << endl;
					flux << "\t\t\t\t{" << endl;
						flux << "\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD].z << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;

			// Left Arm
			flux << "\t\tJOINT LeftCollar" << endl;
			flux << "\t\t{" << endl;
				// Elbow Left
				flux << "\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_LEFT].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT LeftUpArm" << endl;
				flux << "\t\t\t{" << endl;
					// Wrist Left
					flux << "\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_LEFT].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT LeftLowArm" << endl;
					flux << "\t\t\t\t{" << endl;
						// Left Hand
						flux << "\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tJOINT LeftHand" << endl;
						flux << "\t\t\t\t\t{" << endl;
							// End Site
							flux << "\t\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z << endl;
							flux << "\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
							flux << "\t\t\t\t\t\tEnd Site" << endl;
							flux << "\t\t\t\t\t\t{" << endl;
								flux << "\t\t\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_LEFT].z << endl;
							flux << "\t\t\t\t\t\t}" << endl;
						flux << "\t\t\t\t\t}" << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;

			// Right Arm
			flux << "\t\tJOINT RightCollar" << endl;
			flux << "\t\t{" << endl;
				// Elbow Right
				flux << "\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_RIGHT].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT LeftUpArm" << endl;
				flux << "\t\t\t{" << endl;
					// Wrist Right
					flux << "\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ELBOW_RIGHT].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT LeftLowArm" << endl;
					flux << "\t\t\t\t{" << endl;
						// Right Hand
						flux << "\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tJOINT LeftHand" << endl;
						flux << "\t\t\t\t\t{" << endl;
							// End Site
							flux << "\t\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z << endl;
							flux << "\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
							flux << "\t\t\t\t\t\tEnd Site" << endl;
							flux << "\t\t\t\t\t\t{" << endl;
								flux << "\t\t\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT].z << endl;
							flux << "\t\t\t\t\t\t}" << endl;
						flux << "\t\t\t\t\t}" << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;

		flux << "\t}" << endl;

		// Left Leg
		flux << "\tJOINT LeftUpLeg" << endl;
		flux << "\t{" << endl;

			// Hip Left
			flux << "\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT LeftLowLeg" << endl;
			flux << "\t\t{" << endl;

				// Knee Left
				flux << "\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT LeftFoot" << endl;
				flux << "\t\t\t{" << endl;

					// Left Foot
				flux << "\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tEnd Site" << endl;
					flux << "\t\t\t\t{" << endl;

						// End Site
						flux << "\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_LEFT].z << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;
		flux << "\t}" << endl;

		// Right Leg
		flux << "\tJOINT RightUpLeg" << endl;
		flux << "\t{" << endl;

			// Hip Right
			flux << "\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT RightLowLeg" << endl;
			flux << "\t\t{" << endl;

				// Knee Right
				flux << "\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_RIGHT].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT RightFoot" << endl;
				flux << "\t\t\t{" << endl;

					// Right Foot
				flux << "\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tEnd Site" << endl;
					flux << "\t\t\t\t{" << endl;

						// End Site
						flux << "\t\t\t\t\tOFFSET " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].x << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].y << " " << skeleton.SkeletonPositions[NUI_SKELETON_POSITION_FOOT_RIGHT].z << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;
		flux << "\t}" << endl;

	flux << "}" << endl;
}