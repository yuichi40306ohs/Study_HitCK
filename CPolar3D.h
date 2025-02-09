#pragma once
#include <SimpleMath.h>

// �R�c�ɍ��W�n
class CPolor3D {
	float m_radius;				// ���a		
	float m_elevation;			// �p
	float m_azimuth;			// ���ʊp
public:
	CPolor3D() = delete;
	CPolor3D(float radius,
		float elevation,
		float azimuth) : m_radius(radius), m_elevation(elevation), m_azimuth(azimuth) {}
	~CPolor3D() {}

	DirectX::SimpleMath::Vector3 ToOyler() {
		DirectX::SimpleMath::Vector3 euler;

		euler.x = m_radius * sin(m_elevation) * cos(m_azimuth);
		euler.y = m_radius * cos(m_elevation);
		euler.z = m_radius * sin(m_elevation) * sin(m_azimuth);

		return euler;
	}
};
