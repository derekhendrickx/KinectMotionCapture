#include "kinectvideo.h"

KinectVideo::KinectVideo(QObject *parent, QLabel *label, int width, int height)
	: QObject(parent)
{
	m_pLabel = label;
	m_iWidth = width;
	m_iHeigth = height;

	m_pVideoBuffer = new unsigned char[width * height];
}

KinectVideo::~KinectVideo()
{
	delete[] m_pVideoBuffer;
}

void KinectVideo::DrawVideo(const unsigned char *videoData)
{
	QImage qimage((const unsigned char*) videoData, m_iWidth, m_iHeigth, QImage::Format_RGB32);
    m_pLabel->setPixmap(QPixmap::fromImage(qimage.scaled(m_pLabel->frameSize())));
}