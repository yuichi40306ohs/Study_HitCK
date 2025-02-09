#pragma once
#include	<simplemath.h>
#include	<random>
#include	"CMesh.h"

class CCylinderMesh : public CMesh {
public:
	void Init(
		int		divx,							// ������			
		float	radius,							// ���a	
		float	height,							// ����	
		DirectX::SimpleMath::Color color);		// ���_�J���[

	void CreateVertex();						// ���_�f�[�^����
private:
	unsigned int m_division_x = 1;
	unsigned int m_division_y = 1;

	float  m_radius = 100.0f;					// ���a	

	float  m_height = 100.0f;
	float  m_width;								// ���a���玩���v�Z����

	DirectX::SimpleMath::Color m_color;			// ���_�J���[
};