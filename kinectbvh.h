#ifndef KINECTBVH_H
#define KINECTBVH_H

#define SCALE 100
#define FPS 0.033333
#define PI 3.14159265

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>

#include <windows.h>
#include <NuiApi.h>
#include <vector>
#include <map>
#include <Eigen>

using namespace std;
using namespace Eigen;

struct KinectJoint
{
	Matrix3f transform;
	Vector4 quaternion;
	Vector3f angles;
};

/**
* Manipule les données de la Kinect que la classe KinectSkeleton a envoyé et génère le fichier BVH résultant de la capture de mouvements
*/
class KinectBVH : public QObject
{
	Q_OBJECT

public:
	/**
	* Constructeur
	*/
	KinectBVH();
	/**
	* Destructeur
	*/
	~KinectBVH();

	/**
	* Ajoute un offset à la description du BVH
	*/
	void AddOffset(int, const Vector4 &);
	/**
	* Ajoute un squelette et ses informations pour les données de la capture de mouvements
	*/
	void AddBonesOrientation(KinectJoint *);
	/**
	* Ajoute une position du joint Hip Center pour les données de la capture de mouvements
	*/
	void AddPosition(const Vector4 &);
	/**
	* Incrémente le nombre de frames
	*/
	void IncrementNbFrames();
	/**
	* Créé un nouveau fichier en fonction du nom reçu en paramètre, renvoi true si réussi sinon false
	*/
	bool CreateBVHFile(QString);
	/**
	* Génère le fichier BVH
	*/
	void FillBVHFile();
private:
	// Fichier
	QFile *m_pFile;

	// Offsets
	Vector4 m_aOffsets[20];
	// Positions du Hip Center
	vector<Vector4> m_vPositions;
	// Squelettes et leurs informations des joints
	vector<KinectJoint*> m_vBonesOrientation;

	// Nombre de frames
	int m_nbFrame;

	/**
	* Génère la description du squelette pour le BVH
	*/
	void CreateSkeletonInformation();
	/**
	* Génère les données des mouvements pour le BVH
	*/
	void CreateMotionInformation();
	/**
	* Convertis un quaternion passé en paramètre en angles d'Euler, renvoi un tableau contenant les 3 angles
	*/
	int *QuaternionToEulerAngles(const Vector4 &);
};

#endif // KINECTBVH_H
