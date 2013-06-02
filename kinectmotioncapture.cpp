#include "kinectmotioncapture.h"
#include <QMessageBox>
#include <QObject>

#include <iostream>
#include <vld.h>

using namespace std;

/**
* Constructeur
*/
KinectMotionCapture::KinectMotionCapture(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_bSkeletonTracking = false;
	m_pKinectSkeleton = NULL;
	m_pKinectThread = NULL;
	m_pThread = NULL;
	m_bRecording = false;
}

/**
* Destructeur
*/
KinectMotionCapture::~KinectMotionCapture()
{
	if (m_pNuiSensor)
	{
		m_pNuiSensor->NuiShutdown();
	}
	if (m_hNextSkeletonEvent && (m_hNextSkeletonEvent != INVALID_HANDLE_VALUE))
	{
		CloseHandle(m_hNextSkeletonEvent);
		m_hNextSkeletonEvent = NULL;
	}

	delete m_pKinectSkeleton;
	delete m_pKinectThread;
	delete m_pThread;
}

/**
* Initalise la Kinect
*/
void KinectMotionCapture::Initialize()
{
	HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);
	if (FAILED(hr)) {
		QMessageBox::warning(this, "Error", "Kinect cannot be initialized.");
		return;
	}

	m_bSkeletonTracking = InitializeSkeletonTracking();
	if (m_bSkeletonTracking) {
		m_pKinectSkeleton = new KinectSkeleton(this, ui.skeleton);

		// Initialisation du Thread
		m_pThread = new QThread;
		m_pKinectThread = new KinectThread();
		m_pKinectThread->SkeletonHandles(m_hNextSkeletonEvent);
		m_pKinectThread->moveToThread(m_pThread);

		// Mise en place des fonctionnalités du Thread
		connect(m_pThread, SIGNAL(started()), m_pKinectThread, SLOT(process()));
		connect(m_pKinectThread, SIGNAL(finished()), m_pThread, SLOT(quit()));
		connect(m_pKinectThread, SIGNAL(finished()), m_pKinectThread, SLOT(deleteLater()));
		connect(m_pThread, SIGNAL(finished()), m_pThread, SLOT(deleteLater()));
		connect(m_pKinectThread, SIGNAL(EventSkeleton()), this, SLOT(EventSkeleton()));
		connect(ui.record, SIGNAL(clicked()), m_pKinectSkeleton, SLOT(StartRecording()));

		m_pThread->start();
	}
}

bool KinectMotionCapture::InitializeSkeletonTracking()
{
	m_hNextSkeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	
	HRESULT hr = NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
	if (FAILED(hr)) {
		QMessageBox::warning(this, "Error", "Skeleton cannot be initialized.");
		return false;
	}
	return true;
}

/**
* Traitement du nouveau squelette envoyé par le Kinect
*/
void KinectMotionCapture::EventSkeleton()
{
	NUI_SKELETON_FRAME SkeletonFrame = {0};

    bool foundSkeleton = false;
    if ( SUCCEEDED(NuiSkeletonGetNextFrame( 0, &SkeletonFrame )) )
    {
        for ( int i = 0 ; i < NUI_SKELETON_COUNT ; i++ )
        {
            NUI_SKELETON_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[i].eTrackingState;

            if ( trackingState == NUI_SKELETON_TRACKED || trackingState == NUI_SKELETON_POSITION_ONLY )
            {
                foundSkeleton = true;
            }
        }
    }

    // no skeletons!
    if( !foundSkeleton )
    {
        return;
    }

    // smooth out the skeleton data
    HRESULT hr = NuiTransformSmooth(&SkeletonFrame,NULL);
    if ( FAILED(hr) )
    {
        return;
    }

    // we found a skeleton, re-start the skeletal timer
    m_LastSkeletonFoundTime = -1;

    for (int i = 0 ; i < NUI_SKELETON_COUNT; i++)
    {
        NUI_SKELETON_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[i].eTrackingState;

        if (trackingState == NUI_SKELETON_TRACKED)
        {
            // We're tracking the skeleton, draw it
            m_pKinectSkeleton->DrawSkeleton(SkeletonFrame.SkeletonData[i]);

			if (m_pKinectSkeleton->IsRecording() == true && m_pKinectSkeleton->IsCalibrated() == false)
			{
				m_pKinectSkeleton->CalibrateSkeleton(SkeletonFrame.SkeletonData[i]);
			}
        }
    }
}
