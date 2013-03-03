#include "kinectmotioncapture.h"
#include <QMessageBox>
#include <QObject>

#include <iostream>

using namespace std;

KinectMotionCapture::KinectMotionCapture(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_bVideoStreamOpened = false;
	m_bSkeletonTracking = false;
	m_pKinectVideo = NULL;
	m_pKinectSkeleton = NULL;
	m_pKinectThread = NULL;
	m_pThread = NULL;
	m_bRecording = false;
}

KinectMotionCapture::~KinectMotionCapture()
{
	delete m_pKinectVideo;
	delete m_pKinectSkeleton;
	delete m_pKinectThread;
	delete m_pThread;
}

void KinectMotionCapture::Initialize()
{
	HRESULT hr;

	hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON);
	if (FAILED(hr)) {
		QMessageBox::warning(this, "Error", "Kinect cannot be initialized.");
		return;
	}

	m_hNextVideoFrameEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	
	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,NUI_IMAGE_RESOLUTION_640x480,0,2,	m_hNextVideoFrameEvent, &m_videoStream);
	if (FAILED(hr)) {
		QMessageBox::warning(this, "Error", "Video cannot be initialized.");
		return;
	}
	m_bVideoStreamOpened = true;
	m_pKinectVideo = new KinectVideo(this, ui.video, 640, 480);

	m_hNextSkeletonEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	
	hr = NuiSkeletonTrackingEnable(m_hNextSkeletonEvent, 0);
	if (FAILED(hr)) {
		QMessageBox::warning(this, "Error", "Skeleton cannot be initialized.");
		return;
	}
	m_bSkeletonTracking = true;
	m_pKinectSkeleton = new KinectSkeleton(this, ui.skeleton, 320, 240);

	m_pThread = new QThread;
	m_pKinectThread = new KinectThread();

	m_pKinectThread->VideoHandles(m_videoStream, m_hNextVideoFrameEvent);
	m_pKinectThread->SkeletonHandles(m_hNextSkeletonEvent);

	m_pKinectThread->moveToThread(m_pThread);

	connect(m_pThread, SIGNAL(started()), m_pKinectThread, SLOT(process()));
	connect(m_pKinectThread, SIGNAL(finished()), m_pThread, SLOT(quit()));
	connect(m_pKinectThread, SIGNAL(finished()), m_pKinectThread, SLOT(deleteLater()));
	connect(m_pThread, SIGNAL(finished()), m_pThread, SLOT(deleteLater()));
	connect(m_pKinectThread, SIGNAL(EventFrameColor()), this, SLOT(EventFrameColor()));
	connect(m_pKinectThread, SIGNAL(EventSkeleton()), this, SLOT(EventSkeleton()));

	connect(ui.pushButton, SIGNAL(clicked()), m_pKinectSkeleton, SLOT(StartRecording()));

	m_pThread->start();
}

void KinectMotionCapture::EventFrameColor()
{
	const NUI_IMAGE_FRAME* image_frame = NULL;

	HRESULT hr = NuiImageStreamGetNextFrame(m_videoStream, 0, &image_frame);
		
	if(FAILED(hr))
	{
		return;	
	}

	INuiFrameTexture* texture = image_frame->pFrameTexture;
	NUI_LOCKED_RECT locked_rect;
	texture->LockRect(0, &locked_rect, NULL, 0);

	if(locked_rect.Pitch != 0)
    {
    	m_pKinectVideo->DrawVideo((const unsigned char*) locked_rect.pBits);
    }

    texture->UnlockRect(0);

	NuiImageStreamReleaseFrame(m_videoStream, image_frame);
}

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
    m_bScreenBlanked = false;
    m_LastSkeletonFoundTime = -1;
    
    //RECT rct;
    //GetClientRect(GetDlgItem(m_hWnd, IDC_SKELETALVIEW), &rct);
    //int width = rct.right;
    //int height = rct.bottom;

    for (int i = 0 ; i < NUI_SKELETON_COUNT; i++)
    {
        NUI_SKELETON_TRACKING_STATE trackingState = SkeletonFrame.SkeletonData[i].eTrackingState;

        if (trackingState == NUI_SKELETON_TRACKED)
        {
            // We're tracking the skeleton, draw it
            m_pKinectSkeleton->DrawSkeleton(SkeletonFrame.SkeletonData[i], 320, 240);
        }
        else if (trackingState == NUI_SKELETON_POSITION_ONLY)
        {
            // we've only received the center point of the skeleton, draw that
            /*D2D1_ELLIPSE ellipse = D2D1::Ellipse(
                SkeletonToScreen(SkeletonFrame.SkeletonData[i].Position, width, height),
                g_JointThickness,
                g_JointThickness
                );

            m_pRenderTarget->DrawEllipse(ellipse, m_pBrushJointTracked);*/
			m_pKinectSkeleton->Clear();
        }
    }

    //UpdateTrackedSkeletons(SkeletonFrame);
}
