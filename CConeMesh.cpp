#include	<iostream>
#include	"CConeMesh.h"

// ���_�f�[�^���쐬
void CConeMesh::Init(
	int		divx,							// ������			
	float	radius,							// ���a	
	float	height,							// ����	
	DirectX::SimpleMath::Color color)		// ���_�J���[
{
	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_height = height;
	m_width = 2.0f*DirectX::XM_PI*radius;		// ���a�~�~����
	m_radius = radius;

	// ������
	m_division_x = divx;

	// ���_�J���[
	m_color = color;

	// ���_�f�[�^����
	CreateVertex();
}

void CConeMesh::CreateVertex() 
{
	// ���_�f�[�^�N���A
	m_vertices.clear();

	// �Ă��؂�̒��_	
	VERTEX_3D v{};

	v.Position = DirectX::SimpleMath::Vector3(0.0f, m_height, 0.0f);
	v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;

	m_vertices.emplace_back(v);

	// ��ʂ̉~����̒��_�f�[�^���쐬
	float azimuth = 0.0f;			// ���ʊp
	for (unsigned int i = 0; i <= m_division_x; i++) {

		azimuth = (2 * DirectX::XM_PI * (float)i) / (float)m_division_x;	// ���ʊp���Z�b�g

		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = 0.0f;
		v.Position.z = m_radius * sinf(azimuth);	

		v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);		// �@���x�N�g���𒸓_���W�Ɠ����ɂ���	
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// ���_����e�Z�O�����g�̒�ʂ̒��_�ւ̃C���f�b�N�X��ǉ�
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(0);						// ���_
		m_indices.push_back(i + 2);					// ���̒�ʒ��_
		m_indices.push_back(i + 1);					// ���݂̒�ʒ��_
	}

	// ��ʂ̒��S���W
	v.Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;
	m_vertices.emplace_back(v);

	int bottomcenteridx = static_cast<int>(m_vertices.size() - 1);	// ��ʂ̒��S���W�̃C���f�b�N�X

	// ��ʂ̎O�p�`���C���f�b�N�X��
	for (unsigned int i = 0; i <= m_division_x; i++) {
		m_indices.push_back(bottomcenteridx);		// ��ʂ̒��S
		m_indices.push_back(i + 1);					// ���̒�ʒ��_
		m_indices.push_back(i + 2);					// ���݂̒�ʒ��_
	}
}


