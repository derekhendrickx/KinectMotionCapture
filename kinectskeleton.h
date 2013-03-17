#ifndef KINECTSKELETON_H
#define KINECTSKELETON_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>

#include <windows.h>
#include <NuiApi.h>

#include "kinectbvh.h"

static const qreal g_JointThickness = 3.0f;

class KinectSkeleton : public QWidget
{
	Q_OBJECT

public:
	KinectSkeleton(QWidget *parent, QLabel *, int, int);
	~KinectSkeleton();

	void DrawSkeleton(const NUI_SKELETON_DATA &, int, int);
	void Clear();

public slots:
	void StartRecording();

private:
	int m_iWidth;
	int m_iHeight;

	bool m_bRecording;

	QLabel *m_pLabel;
	QPixmap *m_pPixmap;
	QPainter *m_pPainter;
	QPointF m_Points[NUI_SKELETON_POSITION_COUNT];

	KinectBVH m_oKinectBVH;

	QPointF SkeletonToScreen(Vector4, int, int);
	void DrawBone(const NUI_SKELETON_DATA &, NUI_SKELETON_POSITION_INDEX, NUI_SKELETON_POSITION_INDEX);
	void drawJointure();
	void paintEvent(QPaintEvent *);
};

#endif // KINECTSKELETON_H
