#ifndef KINECTVIDEO_H
#define KINECTVIDEO_H

#include <QObject>
#include <QLabel>
#include <QImage>

class KinectVideo : public QObject
{
	Q_OBJECT

public:
	KinectVideo(QObject *parent, QLabel *label, int width, int height);
	~KinectVideo();

	void DrawVideo(const unsigned char *videoData);

private:
	QLabel *m_pLabel;

	int m_iWidth;
	int m_iHeigth;

	unsigned char *m_pVideoBuffer;
};

#endif // KINECTVIDEO_H
