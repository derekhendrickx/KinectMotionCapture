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

	void SkeletonHandles(HANDLE frame);

public slots:
	void process();

signals:
	void finished();
	void EventSkeleton();

private:
	HANDLE m_hNextSkeletonEvent;
	DWORD m_LastSkeletonFoundTime;
};

#endif // KINECTTHREAD_H
