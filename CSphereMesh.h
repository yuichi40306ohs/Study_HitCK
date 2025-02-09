#pragma once
#include	<simplemath.h>
#include	"CMesh.h"

class CSphereMesh : public CMesh {

	// ���_�f�[�^����
	void CreateVertex();
	// �C���f�b�N�X����
	void CreateIndex();

public:
	// ������
	void Init(
		float radius,						// ���a
		DirectX::SimpleMath::Color color,	// �F
		uint32_t division_x,				// X�����̕�����
		uint32_t division_y)				// Y�����̕�����;
	{
		m_color = color;
		m_radius = radius;
		m_division_x = division_x;
		m_division_y = division_y;

		CreateIndex();

		CreateVertex();
	}

	// �ʍ\����
	struct Face {
		uint32_t idx[3];
	};

	// �ʃf�[�^		
	std::vector<Face> m_faces;

private:
	DirectX::SimpleMath::Color m_color;
	float m_radius = 0.0f;
	uint32_t m_division_x = 0;
	uint32_t m_division_y = 0;
};
