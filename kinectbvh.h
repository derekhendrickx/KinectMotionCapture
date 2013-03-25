#ifndef KINECTBVH_H
#define KINECTBVH_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>

#include <windows.h>
#include <NuiApi.h>
#include <vector>

#include "kinectvector4.h"

struct bvh_s {
	std::vector<KinectVector4> offsets;
};

class KinectBVH : public QObject
{
	Q_OBJECT

public:
	KinectBVH();
	~KinectBVH();

	void AddOffset(const KinectVector4 &);
	void AddMotionFrame(std::vector<KinectVector4> offsets);
	void AddFramePerSecond(float fps);
	void AddNbFrames(int nbFrames);
	bool CreateBVHFile(QString);
	void CreateSkeletonInformation(const NUI_SKELETON_DATA &);
	void InitMotion();
	std::vector<KinectVector4> GetOffset();
private:
	QFile *m_pFile;
	QString m_vFileName;

	bvh_s *m_pBVH_s;
	float m_framePerSecond;
	int m_nbFrame;
	bool m_initMotion;
	bool m_initHierarchy;
};

#endif // KINECTBVH_H
