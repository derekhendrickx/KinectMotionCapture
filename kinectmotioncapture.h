#ifndef KINECTMOTIONCAPTURE_H
#define KINECTMOTIONCAPTURE_H

#include <QtWidgets/QMainWindow>
#include <QThread>

#include <windows.h>
#include <NuiApi.h>

#include "ui_kinectmotioncapture.h"
#include "kinectthread.h"
#include "kinectskeleton.h"

#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)

/**
* IHM de l'application
*/
class KinectMotionCapture : public QMainWindow
{
	Q_OBJECT

public:
	/**
	* Constructeur
	*/
	KinectMotionCapture(QWidget *parent = 0);
	/**
	* Destructeur
	*/
	~KinectMotionCapture();

	/**
	* Initalise la Kinect
	*/
	void Initialize();

public slots:
	/**
	* Traitement du nouveau squelette envoyé par lã Kinect
	*/
	void EventSkeleton();

private:
	// Pointeur vers le capteur de la Kinect
	INuiSensor *m_pNuiSensor;
	// Evenement du prochain squelette
    HANDLE m_hNextSkeletonEvent;
	// Flux du squelette
	HANDLE m_skeletonStream;

	// Suivis du squelette
	bool m_bSkeletonTracking;
	// Temps depuis le dernier squelette trouvé
	DWORD m_LastSkeletonFoundTime;

	// Capture de mouvement en cours
	bool m_bRecording;

	KinectSkeleton *m_pKinectSkeleton;
	KinectThread *m_pKinectThread;
	QThread* m_pThread;

	// Composants de l'IHM
	Ui::KinectMotionCaptureClass ui;

	// Initialise le suivis du squelette par la Kinect
	bool InitializeSkeletonTracking();
};

#endif // KINECTMOTIONCAPTURE_H
