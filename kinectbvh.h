#ifndef KINECTBVH_H
#define KINECTBVH_H

#define SCALE 100

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>

#include <windows.h>
#include <NuiApi.h>
#include <vector>

using namespace std;

class KinectBVH : public QObject
{
	Q_OBJECT

public:
	KinectBVH();
	KinectBVH(int);
	~KinectBVH();

	void AddOffset(int, const Vector4 &);
	void AddMotionFrame(const Matrix4 &);
	void AddFramePerSecond(float fps);
	void AddNbFrames(int nbFrames);
	bool CreateBVHFile(QString);
	void CreateSkeletonInformation();
	void InitMotion();
private:
	QFile *m_pFile;
	QString m_vFileName;

	Vector4 m_aOffsets[20];
	vector<Matrix4> m_vMotionData;
	float m_framePerSecond;
	int m_nbFrame;
	bool m_initMotion;
	bool m_initHierarchy;
};

#endif // KINECTBVH_H
