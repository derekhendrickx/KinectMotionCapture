#ifndef KINECTTHREAD_H
#define KINECTTHREAD_H

#include <QObject>

#include <windows.h>
#include <NuiApi.h>

/**
* Récupére les données de la Kinect et met à jour l'IHM
*/
class KinectThread : public QObject
{
	Q_OBJECT

public:
	/**
	* Constructeur
	*/
	KinectThread();
	/**
	* Destructeur
	*/
	virtual ~KinectThread();

	/**
	* Récupère une nouvelle frame contenant un nouveau squelette provenant de la Kinect
	*/
	void SkeletonHandles(HANDLE frame);

public slots:
	/**
	* Traitement principal du Thread
	*/
	void process();

signals:
	// Signaux pour l'utilisation d'un Thread avec Qt
	void finished();
	void EventSkeleton();

private:
	// Nouvel evenement de la Kinect pour le squelette
	HANDLE m_hNextSkeletonEvent;
	// Temps depuis le dernier squelette trouvé
	DWORD m_LastSkeletonFoundTime;
};

#endif // KINECTTHREAD_H
