#ifndef KINECTSKELETON_H
#define KINECTSKELETON_H

#define SCALE 100

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTransform>

#include <windows.h>
#include <NuiApi.h>
#include <cmath>
#include <Eigen>

#include "ui_kinectmotioncapture.h"
#include "kinectbvh.h"

static const qreal g_JointThickness = 3.0f;

class KinectSkeleton : public QWidget
{
	Q_OBJECT

public:
	KinectSkeleton(QWidget *, QGraphicsView*);
	~KinectSkeleton();

	inline bool IsRecording() { return m_bRecording; };
	inline bool IsCalibrated() { return m_bIsCalibrated; };
	inline KinectBVH* getBVH() { return m_pKinectBVH; };

	void DrawSkeleton(const NUI_SKELETON_DATA &, int, int);

	void CalibrateSkeleton(const NUI_SKELETON_DATA &);

public slots:
	void StartRecording();

private:
	int m_iWidth;
	int m_iHeight;

	bool m_bRecording;
	bool m_bIsCalibrated;

	QGraphicsView *m_pGraphicsView;
	QGraphicsScene *m_pGraphicsScene;
	QPointF m_Points[NUI_SKELETON_POSITION_COUNT];

	KinectBVH *m_pKinectBVH;

	QPointF SkeletonToScreen(Vector4, int, int);
	void DrawBone(const NUI_SKELETON_DATA &, NUI_SKELETON_POSITION_INDEX, NUI_SKELETON_POSITION_INDEX);
	void drawJointure();
	void ProcessBonesOrientation(const NUI_SKELETON_DATA &);
};

#endif // KINECTSKELETON_H
