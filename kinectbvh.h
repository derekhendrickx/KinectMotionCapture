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

struct KinectNode
{
	struct KinectNode *parent;
	struct KinectNode *sons;
	Matrix3f transform;
	Matrix3f inverseTransform;
	Matrix3f localTransform;
};

class KinectBVH : public QObject
{
	Q_OBJECT

public:
	KinectBVH();
	KinectBVH(int);
	~KinectBVH();

	void AddOffset(int, const Vector4 &);
	void AddBonesOrientation(KinectNode *);
	void AddMotionFrame(const Matrix4 &);
	void AddQuaternion(const Vector4 &);
	void AddPosition(const Vector4 &);
	void IncrementNbFrames();
	bool CreateBVHFile(QString);
	void FillBVHFile();
private:
	QFile *m_pFile;
	QString m_vFileName;

	Vector4 m_aOffsets[20];
	vector<Vector4> m_vPositions;
	vector<KinectNode*> m_vBonesOrientation;
	vector<Eigen::Vector3f,Eigen::aligned_allocator<Eigen::Vector3f>> m_vMotionData;
	vector<Vector4> m_vQuaternions;

	int m_nbFrame;

	void CreateSkeletonInformation();
	void CreateMotionInformation();
	int *QuaternionToEulerAngles(const Vector4 &);
};

#endif // KINECTBVH_H
