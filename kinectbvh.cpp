#include "kinectbvh.h"

KinectBVH::KinectBVH()
{
	m_initMotion = false;
	m_initHierarchy = false;
	m_framePerSecond = NULL;
	m_nbFrame = NULL;
}

KinectBVH::~KinectBVH()
{
	m_pFile->close();
	delete m_pFile;
	m_pFile = NULL;
}

void KinectBVH::AddOffset(int i, const Vector4 &offset)
{
	m_aOffsets[i].x = offset.x * SCALE;
	m_aOffsets[i].y = offset.y * SCALE;
	m_aOffsets[i].z = offset.z * SCALE;
}

bool KinectBVH::CreateBVHFile(QString filename)
{
	m_pFile = new QFile(filename);
	if (!m_pFile->open(QIODevice::Append | QIODevice::Text)) {
		return false;
	}
	return true;
}

void KinectBVH::CreateSkeletonInformation()
{
	int i = 0;
	QTextStream flux(m_pFile);
	flux.setCodec("utf-8");

	// ROOT
	flux << "HIERARCHY" << endl;
	flux << "ROOT Hip" << endl;
	flux << "{" << endl;

		// Spine
		flux << "\tOFFSET " << m_aOffsets[0].x << " " << m_aOffsets[0].y << " " << m_aOffsets[0].z << endl;
		flux << "\tCHANNELS 6 Xposition Yposition Zposition Zrotation Xrotation Yrotation" << endl;
		flux << "\tJOINT Spine" << endl;
		flux << "\t{" << endl;

			// Shoulder Center
			flux << "\t\tOFFSET " << m_aOffsets[1].x - m_aOffsets[0].x << " " << m_aOffsets[1].y - m_aOffsets[0].y << " " << m_aOffsets[1].z - m_aOffsets[0].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT ShoulderCenter" << endl;
			flux << "\t\t{" << endl;
				// Head
				flux << "\t\t\tOFFSET " << m_aOffsets[2].x - m_aOffsets[1].x << " " << m_aOffsets[2].y - m_aOffsets[1].y << " " << m_aOffsets[2].z - m_aOffsets[1].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT Head" << endl;
				flux << "\t\t\t{" << endl;
					// End Site
					flux << "\t\t\t\tOFFSET " << m_aOffsets[3].x - m_aOffsets[2].x << " " << m_aOffsets[3].y - m_aOffsets[2].y << " " << m_aOffsets[3].z - m_aOffsets[2].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tEnd Site" << endl;
					flux << "\t\t\t\t{" << endl;
						flux << "\t\t\t\t\tOFFSET 0.0 0.0 0.0" << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;

				// Shoulder Left
				flux << "\t\t\tJOINT ShoulderLeft" << endl;
				flux << "\t\t\t{" << endl;
					// Elbow Left
					flux << "\t\t\t\tOFFSET " << m_aOffsets[4].x - m_aOffsets[2].x << " " << m_aOffsets[4].y - m_aOffsets[2].y << " " << m_aOffsets[4].z - m_aOffsets[2].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT ElbowLeft" << endl;
					flux << "\t\t\t\t{" << endl;
						// Wrist Left
						flux << "\t\t\t\t\tOFFSET " << m_aOffsets[5].x - m_aOffsets[4].x << " " << m_aOffsets[5].y - m_aOffsets[4].y << " " << m_aOffsets[5].z - m_aOffsets[4].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tJOINT WristLeft" << endl;
						flux << "\t\t\t\t\t{" << endl;
							// Hand Left
							flux << "\t\t\t\t\t\tOFFSET " << m_aOffsets[6].x - m_aOffsets[5].x << " " << m_aOffsets[6].y - m_aOffsets[5].y << " " << m_aOffsets[6].z - m_aOffsets[5].z << endl;
							flux << "\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
							flux << "\t\t\t\t\t\tJOINT HandLeft" << endl;
							flux << "\t\t\t\t\t\t{" << endl;
								// End Site
								flux << "\t\t\t\t\t\t\tOFFSET " << m_aOffsets[7].x - m_aOffsets[6].x << " " << m_aOffsets[7].y - m_aOffsets[6].y << " " << m_aOffsets[7].z - m_aOffsets[6].z << endl;
								flux << "\t\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
								flux << "\t\t\t\t\t\t\tEnd Site" << endl;
								flux << "\t\t\t\t\t\t\t{" << endl;
									flux << "\t\t\t\t\t\t\t\tOFFSET 0.0 0.0 0.0" << endl;
								flux << "\t\t\t\t\t\t\t}" << endl;
							flux << "\t\t\t\t\t\t}" << endl;
						flux << "\t\t\t\t\t}" << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;

				// Shoulder Right
				flux << "\t\t\tJOINT ShoulderRight" << endl;
				flux << "\t\t\t{" << endl;
					// Elbow Right
					flux << "\t\t\t\tOFFSET " << m_aOffsets[8].x - m_aOffsets[2].x << " " << m_aOffsets[8].y - m_aOffsets[2].y << " " << m_aOffsets[8].z - m_aOffsets[2].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT ElbowRight" << endl;
					flux << "\t\t\t\t{" << endl;
						// Wrist Right
						flux << "\t\t\t\t\tOFFSET " << m_aOffsets[9].x - m_aOffsets[8].x << " " << m_aOffsets[9].y - m_aOffsets[8].y << " " << m_aOffsets[9].z - m_aOffsets[8].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tJOINT WristRight" << endl;
						flux << "\t\t\t\t\t{" << endl;
							// Hand Right
							flux << "\t\t\t\t\t\tOFFSET " << m_aOffsets[10].x - m_aOffsets[9].x << " " << m_aOffsets[10].y - m_aOffsets[9].y << " " << m_aOffsets[10].z - m_aOffsets[9].z << endl;
							flux << "\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
							flux << "\t\t\t\t\t\tJOINT HandRight" << endl;
							flux << "\t\t\t\t\t\t{" << endl;
								// End Site
								flux << "\t\t\t\t\t\t\tOFFSET " << m_aOffsets[11].x - m_aOffsets[10].x << " " << m_aOffsets[11].y - m_aOffsets[10].y << " " << m_aOffsets[11].z - m_aOffsets[10].z << endl;
								flux << "\t\t\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
								flux << "\t\t\t\t\t\t\tEnd Site" << endl;
								flux << "\t\t\t\t\t\t\t{" << endl;
									flux << "\t\t\t\t\t\t\t\tOFFSET 0.0 0.0 0.0" << endl;
								flux << "\t\t\t\t\t\t\t}" << endl;
							flux << "\t\t\t\t\t\t}" << endl;
						flux << "\t\t\t\t\t}" << endl;
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			
			flux << "\t\t}" << endl;

		flux << "\t}" << endl;

		// Hip Left
		flux << "\tJOINT HipLeft" << endl;
		flux << "\t{" << endl;

			// Knee Left
			flux << "\t\tOFFSET " << m_aOffsets[12].x - m_aOffsets[0].x << " " << m_aOffsets[12].y - m_aOffsets[0].y << " " << m_aOffsets[12].z - m_aOffsets[0].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT KneeLeft" << endl;
			flux << "\t\t{" << endl;

				// Ankle Left
				flux << "\t\t\tOFFSET " << m_aOffsets[13].x - m_aOffsets[12].x << " " << m_aOffsets[13].y - m_aOffsets[12].y << " " << m_aOffsets[13].z - m_aOffsets[12].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT AnkleLeft" << endl;
				flux << "\t\t\t{" << endl;

					// Foot Left
					flux << "\t\t\t\tOFFSET " << m_aOffsets[14].x - m_aOffsets[13].x << " " << m_aOffsets[14].y - m_aOffsets[13].y << " " << m_aOffsets[14].z - m_aOffsets[13].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT FootLeft" << endl;
					flux << "\t\t\t\t{" << endl;
					
						// End Site
						flux << "\t\t\t\t\tOFFSET " << m_aOffsets[15].x - m_aOffsets[14].x << " " << m_aOffsets[15].y - m_aOffsets[14].y << " " << m_aOffsets[15].z - m_aOffsets[14].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tEnd Site" << endl;
						flux << "\t\t\t\t\t{" << endl;
							flux << "\t\t\t\t\t\tOFFSET 0.0 0.0 0.0" << endl;
						flux << "\t\t\t\t\t}" << endl;	
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;
		flux << "\t}" << endl;

		// Hip Right
		flux << "\tJOINT HipRight" << endl;
		flux << "\t{" << endl;

			// Knee Right
			flux << "\t\tOFFSET " << m_aOffsets[16].x - m_aOffsets[0].x << " " << m_aOffsets[16].y - m_aOffsets[0].y << " " << m_aOffsets[16].z - m_aOffsets[0].z << endl;
			flux << "\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
			flux << "\t\tJOINT KneeRight" << endl;
			flux << "\t\t{" << endl;

				// Ankle Right
				flux << "\t\t\tOFFSET " << m_aOffsets[17].x - m_aOffsets[16].x << " " << m_aOffsets[17].y - m_aOffsets[16].y << " " << m_aOffsets[17].z - m_aOffsets[16].z << endl;
				flux << "\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
				flux << "\t\t\tJOINT AnkleRight" << endl;
				flux << "\t\t\t{" << endl;

					// Foot Right
					flux << "\t\t\t\tOFFSET " << m_aOffsets[18].x - m_aOffsets[17].x << " " << m_aOffsets[18].y - m_aOffsets[17].y << " " << m_aOffsets[18].z - m_aOffsets[17].z << endl;
					flux << "\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
					flux << "\t\t\t\tJOINT FootRight" << endl;
					flux << "\t\t\t\t{" << endl;
					
						// End Site
						flux << "\t\t\t\t\tOFFSET " << m_aOffsets[19].x - m_aOffsets[18].x << " " << m_aOffsets[19].y - m_aOffsets[18].y << " " << m_aOffsets[19].z - m_aOffsets[18].z << endl;
						flux << "\t\t\t\t\tCHANNELS 3 Zrotation Xrotation Yrotation" << endl;
						flux << "\t\t\t\t\tEnd Site" << endl;
						flux << "\t\t\t\t\t{" << endl;
							flux << "\t\t\t\t\t\tOFFSET 0.0 0.0 0.0" << endl;
						flux << "\t\t\t\t\t}" << endl;	
					flux << "\t\t\t\t}" << endl;
				flux << "\t\t\t}" << endl;
			flux << "\t\t}" << endl;
		flux << "\t}" << endl;

	flux << "}" << endl;
}

void KinectBVH::AddNbFrames(int nbFrames)
{
	m_nbFrame = nbFrames;
}

void KinectBVH::AddFramePerSecond(float fps)
{
	m_framePerSecond = fps;
}

void KinectBVH::AddMotionFrame(const Matrix4 &rotationMatrix)
{
	//if (m_initMotion)
	//{
	//	// création d'un objet qtextstream à partir de notre objet qfile
	//	QTextStream flux(m_pFile);
	//	// on choisit le codec correspondant au jeu de caractère que l'on souhaite ; ici, utf-8
	//	flux.setCodec("utf-8");

	//	// on ajoute la motion courant dans le fichier bvh 
	//	for (unsigned i=0; i<offsets.size();i++)
	//	{
	//		flux << offsets[i].x() << "\t";
	//		flux << offsets[i].y() << "\t";
	//		flux << offsets[i].z() << "\t";
	//	}
	//	flux << endl;

	//	// on vide le buffer correspondant à la motion courante
	//	offsets.clear();
	//}

	m_vMotionData.push_back(rotationMatrix);
}

void KinectBVH::InitMotion()
{
	if (m_nbFrame != NULL && m_framePerSecond != NULL && m_initHierarchy)
	{
		QTextStream flux(m_pFile);
		flux.setCodec("utf-8");

		flux << "\nMOTION" << endl;
		flux << "Frames: " << m_nbFrame << endl;
		flux << "Frame Time: " << m_framePerSecond << endl;
		m_initMotion = true;
	}
}
