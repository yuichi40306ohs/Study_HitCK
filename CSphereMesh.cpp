#include	"CSphereMesh.h"

using namespace DirectX::SimpleMath;

// �C���f�b�N�X�f�[�^���쐬
void CSphereMesh::CreateIndex() {

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_division_y; y++) {
		for (unsigned int x = 0; x < m_division_x; x++) {
			int count = (m_division_x + 1) * y + x;			// ������W�̃C���f�b�N�X

			Face tempface{};

			// �㔼��
			tempface.idx[0] = count;
			tempface.idx[1] = count + 1;
			tempface.idx[2] = count + 1 + (m_division_x + 1);

			m_faces.emplace_back(tempface);

			// ������
			tempface.idx[0] = count;
			tempface.idx[1] = count + (m_division_x + 1) + 1;
			tempface.idx[2] = count + (m_division_x + 1);

			m_faces.emplace_back(tempface);
		}
	}
	
	m_indices.clear();

	for(auto& f : m_faces) {
		m_indices.emplace_back(f.idx[0]);
		m_indices.emplace_back(f.idx[1]);
		m_indices.emplace_back(f.idx[2]);
	}
}

// ���_�f�[�^���쐬
void CSphereMesh::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= m_division_y; y++) {

		// �p�v�Z
		elevation = (DirectX::XM_PI * (float)y) / (float)m_division_y;			// �p���Z�b�g

		float r = m_radius * sinf(elevation);									// �p�ɉ��������a���v�Z
			
		for (unsigned int x = 0; x <= m_division_x; x++) {

			azimuth = (2 * DirectX::XM_PI * (float)x) / (float)m_division_x;	// ���ʊp���Z�b�g

			VERTEX_3D	v;

			// ���_���W�Z�b�g
			v.Position.x = r * cosf(azimuth);
			v.Position.y = m_radius * cosf(elevation);
			v.Position.z = r * sinf(azimuth);

			v.Normal = v.Position;		// �@���x�N�g���𒸓_���W�Ɠ����ɂ���

			v.Diffuse = m_color;

			// �@���x�N�g��
			v.Normal.Normalize(v.Normal);	// �@���𐳋K��

			// ���_�o�^
			m_vertices.emplace_back(v);
		}
	}
}