#include "CStaticMeshRenderer.h"

void CStaticMeshRenderer::Init(CStaticMesh& mesh)
{
	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�𐶐�
	CMeshRenderer::Init(mesh);

	// �T�u�Z�b�g���擾
	m_Subsets = mesh.GetSubsets();

	// diffuse�e�N�X�`�����擾
	m_DiffuseTextures = mesh.GetDiffuseTextures();

	// �}�e���A�����擾	
	std::vector<MATERIAL> materials;
	materials = mesh.GetMaterials();

	// �}�e���A���������[�v���ă}�e���A���f�[�^�𐶐�
	for (int i = 0; i < materials.size(); i++)
	{
		// �}�e���A���I�u�W�F�N�g����
		std::unique_ptr<CMaterial> m = std::make_unique<CMaterial>();

		// �}�e���A�������Z�b�g
		m->Create(materials[i]);

		// �}�e���A���I�u�W�F�N�g��z��ɒǉ�
		m_Materiales.push_back(std::move(m));
	}
}

void CStaticMeshRenderer::Draw()
{
	// �C���f�b�N�X�o�b�t�@�E���_�o�b�t�@���Z�b�g
	BeforeDraw();

	// �}�e���A���������[�v 
	for (int i = 0; i < m_Subsets.size(); i++)
	{
		// �}�e���A�����Z�b�g(�T�u�Z�b�g���̒��ɂ���}�e���A���C���f�b�N���g�p����)
		m_Materiales[m_Subsets[i].MaterialIdx]->SetGPU();

		if (m_Materiales[m_Subsets[i].MaterialIdx]->isDiffuseTextureEnable())
		{
			m_DiffuseTextures[m_Subsets[i].MaterialIdx]->SetGPU();
		}

		// �T�u�Z�b�g�̕`��
		DrawSubset(
			m_Subsets[i].IndexNum,							// �`�悷��C���f�b�N�X��
			m_Subsets[i].IndexBase,							// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu	
			m_Subsets[i].VertexBase);						// ���_�o�b�t�@�̍ŏ�����g�p
	}
}
