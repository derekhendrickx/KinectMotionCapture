#ifndef KINECTVECTOR4_H
#define KINECTVECTOR4_H

#include <iostream>
#include <windows.h>
#include <NuiApi.h>
#include <cmath>

class KinectVector4
{
public:
	KinectVector4();
	KinectVector4(float, float, float);
	KinectVector4(float, float, float, float);
	KinectVector4(const Vector4&);
	KinectVector4(const KinectVector4&);
	~KinectVector4();

	inline float x() const { return m_fX; };
	inline float y() const { return m_fY; };
	inline float z() const { return m_fZ; };
	inline float w() const { return m_fW; };

	inline void x(float x) { m_fX = x; };
	inline void y(float y) { m_fY = y; };
	inline void z(float z) { m_fZ = z; };
	inline void w(float w) { m_fW = w; };

	void setVector4(float, float, float);
	void setVector4(float, float, float, float);

	const KinectVector4 &normalize();
	float length() const;
	void subtract(const KinectVector4 &);
	float dot(const KinectVector4 &) const;
	float angle(const KinectVector4 &) const;

	friend KinectVector4 operator -(const KinectVector4 &, const KinectVector4 &);

private:
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fW;
};

#endif // KINECTVECTOR4_H
