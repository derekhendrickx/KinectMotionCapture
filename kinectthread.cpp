#include "kinectthread.h"

#include "kinectmotioncapture.h"

KinectThread::KinectThread()
{
	m_hNextVideoFrameEvent = NULL;
	m_videoStream = NULL;
	m_hNextSkeletonEvent = NULL;
	m_bRunning = false;
}

KinectThread::~KinectThread()
{

}

void KinectThread::Shutdown()
{
	m_bRunning = true;
}

void KinectThread::process()
{
	HANDLE events[2] = {m_hNextVideoFrameEvent, m_hNextSkeletonEvent};
	int event_idx;

	while(!m_bRunning)
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

		Sleep(15);
	}

	emit finished();
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
