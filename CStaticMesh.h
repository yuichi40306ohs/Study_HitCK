#pragma once

#include	<simplemath.h>
#include	<string>
#include	<vector>
#include	<memory>
#include	"CTexture.h"
#include	"CMesh.h"
#include	"renderer.h"

class CStaticMesh : public CMesh {
public:
	void Load(std::string filename, std::string texturedirectory="");

	const std::vector<MATERIAL>& GetMaterials() {
		return m_materials;
	}

	const std::vector<SUBSET>& GetSubsets() {
		return m_subsets;
	}

	const std::vector<std::string>& GetDiffuseTextureNames() {
		return m_diffusetexturenames;
	}

	std::vector<std::unique_ptr<CTexture>> GetDiffuseTextures() {
		return std::move(m_diffusetextures);
	}

private:

	std::vector<MATERIAL> m_materials;					// �}�e���A�����
	std::vector<std::string> m_diffusetexturenames;		// �f�B�t���[�Y�e�N�X�`����
	std::vector<SUBSET> m_subsets;						// �T�u�Z�b�g���

	std::vector<std::unique_ptr<CTexture>>	m_diffusetextures;	// �f�B�t���[�Y�e�N�X�`���Q
};