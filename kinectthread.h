#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QThread>

#include <windows.h>
#include <NuiApi.h>

class KinectThread : public QThread
{
	Q_OBJECT

public:
	KinectThread(QObject *parent);
	virtual ~KinectThread();

	void VideoHandles(HANDLE stream, HANDLE frame);
	void SkeletonHandles(HANDLE frame);

	void stop();

protected:
	void run();

signals:
	void EventFrameColor();
	void EventSkeleton();

private:
	HANDLE m_hNextVideoFrameEvent;
	HANDLE m_videoStream;
	HANDLE m_hNextSkeletonEvent;

	bool m_bRunning;
};

#endif // KINECTTHREAD_H
