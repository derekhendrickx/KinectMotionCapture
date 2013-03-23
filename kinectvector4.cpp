#include "kinectvector4.h"

KinectVector4::KinectVector4()
{

}

KinectVector4::KinectVector4(float x, float y, float z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}

KinectVector4::KinectVector4(float x, float y, float z, float w)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = w;
}

KinectVector4::KinectVector4(const Vector4 &vector)
{
	m_fX = vector.x;
	m_fY = vector.y;
	m_fZ = vector.z;
	m_fW = vector.w;
}

KinectVector4::KinectVector4(const KinectVector4 &vector)
{
	m_fX = vector.x();
	m_fY = vector.y();
	m_fZ = vector.z();
	m_fW = vector.w();
}

KinectVector4::~KinectVector4()
{

}

void KinectVector4::setVector4(float x, float y, float z)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
}

void KinectVector4::setVector4(float x, float y, float z, float w)
{
	m_fX = x;
	m_fY = y;
	m_fZ = z;
	m_fW = w;
}

const KinectVector4 &KinectVector4::normalize()
{
	float d = length();
	m_fX /= d;
	m_fY /= d;
	m_fZ /= d;
	return *this;
}

float KinectVector4::length() const
{
	float temp = x() * x() + y() * y() + z() * z();
	return sqrt(temp);
}

void KinectVector4::subtract(const KinectVector4 &b)
{
	float x = b.x() - this->x();
	float y = b.y() - this->y();
	float z = b.z() - this->z();
	setVector4(x, y, z);
}

float KinectVector4::dot(const KinectVector4 &b) const
{
	return x() * b.x() + y() * b.y() + z() * b.z();
}

float KinectVector4::angle(const KinectVector4 &b) const
{
	KinectVector4 a = KinectVector4(*this);
	KinectVector4 u = KinectVector4(b);

	a.normalize();
	u.normalize();

	float angle = acos(u.dot(a));

	return angle;
}

KinectVector4 operator -(const KinectVector4 &a, const KinectVector4 &b) {
    KinectVector4 p(a);
    p.subtract(b);
    return p;
}