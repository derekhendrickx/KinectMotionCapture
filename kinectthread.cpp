#include "kinectthread.h"

#include "kinectmotioncapture.h"

KinectThread::KinectThread()
{
	m_hNextSkeletonEvent = NULL;
}

KinectThread::~KinectThread()
{

}

void KinectThread::process()
{
	const int numEvents = 1;
    HANDLE hEvents[numEvents] = {m_hNextSkeletonEvent};
    int nEventIdx;
    DWORD t;

    // Blank the skeleton display on startup
    m_LastSkeletonFoundTime = 0;

    // Main thread loop
    bool continueProcessing = true;
    while (continueProcessing)
    {
        //// Wait for any of the events to be signalled
        //nEventIdx = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

        //// Timed out, continue
        //if (nEventIdx == WAIT_TIMEOUT)
        //{
        //    continue;
        //}

        //// stop event was signalled 
        //if (WAIT_OBJECT_0 == nEventIdx)
        //{
        //    continueProcessing = false;
        //    break;
        //}

        // Wait for each object individually with a 0 timeout to make sure to
        // process all signalled objects if multiple objects were signalled
        // this loop iteration

        // In situations where perfect correspondance between color/depth/skeleton
        // is essential, a priority queue should be used to service the item
        // which has been updated the longest ago
        if (WAIT_OBJECT_0 == WaitForSingleObject(m_hNextSkeletonEvent, 0))
        {
            emit EventSkeleton();
        }

        // Once per second, display the depth FPS
        /*t = timeGetTime();
        if ( (t - m_LastDepthFPStime) > 1000 )
        {
            int fps = ((m_DepthFramesTotal - m_LastDepthFramesTotal) * 1000 + 500) / (t - m_LastDepthFPStime);
            PostMessageW( m_hWnd, WM_USER_UPDATE_FPS, IDC_FPS, fps );
            m_LastDepthFramesTotal = m_DepthFramesTotal;
            m_LastDepthFPStime = t;
        }*/

        // Blank the skeleton panel if we haven't found a skeleton recently
        /*if ( (t - m_LastSkeletonFoundTime) > 300 )
        {
            if ( !m_bScreenBlanked )
            {
                Nui_BlankSkeletonScreen( );
                m_bScreenBlanked = true;
            }
        }*/
    }

	emit finished();
}

void KinectThread::SkeletonHandles(HANDLE frame)
{
	m_hNextSkeletonEvent = frame;
}
