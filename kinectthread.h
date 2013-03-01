#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QObject>

#include <windows.h>
#include <NuiApi.h>

class KinectThread : public QObject
{
	Q_OBJECT

public:
	KinectThread();
	virtual ~KinectThread();

	void Shutdown();

	void VideoHandles(HANDLE stream, HANDLE frame);
	void SkeletonHandles(HANDLE frame);

public slots:
	void process();

signals:
	void finished();
	void EventFrameColor();
	void EventSkeleton();

private:
	HANDLE m_hNextVideoFrameEvent;
	HANDLE m_videoStream;
	HANDLE m_hNextSkeletonEvent;

	bool m_bRunning;
};

#endif // KINECTTHREAD_H
