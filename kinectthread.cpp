#include "kinectthread.h"

#include "kinectmotioncapture.h"

KinectThread::KinectThread(QObject *parent)
	: QThread(parent)
{
	m_hNextVideoFrameEvent = NULL;
	m_videoStream = NULL;
	m_hNextSkeletonEvent = NULL;
	m_bRunning = true;
}

KinectThread::~KinectThread()
{

}

void KinectThread::stop()
{
	m_bRunning = false;
}

void KinectThread::run()
{
	HANDLE events[2] = {m_hNextVideoFrameEvent, m_hNextSkeletonEvent};

	m_bRunning = true;
	int event_idx;

	while(m_bRunning)
	{
		// [rad] Wait for events, (not necessarily all) and time out is 100 msec.
		event_idx = WaitForMultipleObjects((sizeof(events) / sizeof(events[0])), events, FALSE, 100);

		// [rad] Process events.
		if(0 == event_idx)
		{
			// [rad] We have a color frame ready, emit signal.
			emit EventFrameColor();
		}
		else if(1 == event_idx)
		{
			// [rad] We have a depth frame ready, emit signal.
			emit EventSkeleton();
		}

		usleep(15);
	}
}

void KinectThread::VideoHandles(HANDLE stream, HANDLE frame)
{
	m_hNextVideoFrameEvent = frame;
	m_videoStream = stream;
}

void KinectThread::SkeletonHandles(HANDLE frame)
{
	m_hNextSkeletonEvent = frame;
}
