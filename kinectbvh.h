#ifndef KINECTBVH_H
#define KINECTBVH_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>

#include <windows.h>
#include <NuiApi.h>

class KinectBVH : public QObject
{
	Q_OBJECT

public:
	KinectBVH();
	~KinectBVH();

	bool CreateBVHFile(QString);
	void CreateSkeletonInformation(const NUI_SKELETON_DATA &);

private:
	QFile *m_pFile;
	QString m_vFileName;	
};

#endif // KINECTBVH_H
