#ifndef KINECTMOTIONCAPTURE_H
#define KINECTMOTIONCAPTURE_H

#include <QtWidgets/QMainWindow>

#include <windows.h>
#include <NuiApi.h>
#include <NuiSensor.h>
#include <NuiImageCamera.h>

#include "ui_kinectmotioncapture.h"
#include "kinectvideo.h"
#include "kinectthread.h"
#include "kinectskeleton.h"

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

class KinectMotionCapture : public QMainWindow
{
	Q_OBJECT

public:
	KinectMotionCapture(QWidget *parent = 0);
	~KinectMotionCapture();

	void Initialize();

public slots:
	void EventFrameColor();
	void EventSkeleton();

private:
	INuiSensor *m_pNuiSensor;
	HANDLE m_hNextVideoFrameEvent;
    HANDLE m_hNextSkeletonEvent;
	HANDLE m_videoStream;
	HANDLE m_skeletonStream;

	bool m_bVideoStreamOpened;
	bool m_bSkeletonTracking;
	DWORD m_LastSkeletonFoundTime;
    bool m_bScreenBlanked;

	KinectVideo *m_pKinectVideo;
	KinectSkeleton *m_pKinectSkeleton;
	KinectThread *m_pKinectThread;

	Ui::KinectMotionCaptureClass ui;
};

#endif // KINECTMOTIONCAPTURE_H
