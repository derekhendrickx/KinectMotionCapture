#include "kinectthread.h"
#include "kinectmotioncapture.h"

/**
* Constructeur
*/
KinectThread::KinectThread()
{
	m_hNextSkeletonEvent = NULL;
}

/**
* Destructeur
*/
KinectThread::~KinectThread()
{

}

/**
* Traitement principal du Thread
*/
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
    }

	emit finished();
}

/**
* Récupère une nouvelle frame contenant un nouveau squelette provenant de la Kinect
*/
void KinectThread::SkeletonHandles(HANDLE frame)
{
	m_hNextSkeletonEvent = frame;
}
