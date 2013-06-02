#ifndef KINECTSKELETON_H
#define KINECTSKELETON_H

#define SCALE 100

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTransform>

#include <windows.h>
#include <NuiApi.h>
#include <cmath>
#include <Eigen>

#include "ui_kinectmotioncapture.h"
#include "kinectbvh.h"

static const qreal g_JointThickness = 3.0f;

/**
* Réalise le traitement du squelette que la Kinect détecte afin de l'afficher à l'écran
*/
class KinectSkeleton : public QWidget
{
	Q_OBJECT

public:
	/**
	* Constructeur prenant en paramètre le widget parent et la zone d'affichage du squelette
	*/
	KinectSkeleton(QWidget *, QGraphicsView*);
	/**
	* Destructeur
	*/
	~KinectSkeleton();

	/**
	* Getter du status de la capture de mouvement
	*/
	inline bool IsRecording() { return m_bRecording; };
	/**
	* Getter du status de la description du squelette
	*/
	inline bool IsCalibrated() { return m_bIsCalibrated; };

	/**
	* Affiche le squelette sur l'IHM
	*/
	void DrawSkeleton(const NUI_SKELETON_DATA &);
	/**
	* Envoi des données initiales pour la descrition du squelette
	*/
	void CalibrateSkeleton(const NUI_SKELETON_DATA &);

public slots:
	/**
	* Démarre et stop la capture de mouvements
	*/
	void StartRecording();

private:
	// Largeur de la zone d'affichage du squelette sur l'IHM
	int m_iWidth;
	// Hauteur de la zone d'affichage du squelette sur l'IHM
	int m_iHeight;

	// Capture de mouvements en cours
	bool m_bRecording;
	// Description du squelette
	bool m_bIsCalibrated;

	// Zone d'affichage du squelette
	QGraphicsView *m_pGraphicsView;
	QGraphicsScene *m_pGraphicsScene;
	// Ensemble des points
	QPointF m_Points[NUI_SKELETON_POSITION_COUNT];

	KinectBVH *m_pKinectBVH;

	/**
	* Mise à l'échelle des points du squelette en fonction de la zone d'affichage
	*/
	QPointF SkeletonToScreen(Vector4, int, int);
	/**
	* Affichage d'un membre du squelette
	*/
	void DrawBone(const NUI_SKELETON_DATA &, NUI_SKELETON_POSITION_INDEX, NUI_SKELETON_POSITION_INDEX);
	/**
	* Conversion et envoi des matrices de rotation pour la génération du fichier BVH
	*/
	void ProcessBonesOrientation(const NUI_SKELETON_DATA &);
};

#endif // KINECTSKELETON_H
