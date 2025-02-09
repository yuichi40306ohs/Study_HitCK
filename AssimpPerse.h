#pragma once
#include	<assimp\Importer.hpp>
#include	<assimp\scene.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>
#include	"CTexture.h"
#include	"CTreeNode.h"

namespace AssimpPerse
{
	struct VERTEX {
		std::string meshname;				// ���b�V����
		aiVector3D	pos;					// �ʒu
		aiVector3D	normal;					// �@��
		aiColor4D	color;					// ���_�J���[	
		aiVector3D	texcoord;				// �e�N�X�`�����W	
		int			materialindex;			// �}�e���A���C���f�b�N�X
		std::string mtrlname;				// �}�e���A����

		int			BoneIndex[4];			// �{�[���C���f�b�N�X
		float		BoneWeight[4];			// �{�[���E�F�C�g
		std::string	BoneName[4];			// �{�[����
		int			bonecnt = 0;			// �{�[����
	};

	struct SUBSET {
		std::string meshname;				// ���b�V����
		int materialindex;					// �}�e���A���C���f�b�N�X
		unsigned int VertexBase;			// ���_�o�b�t�@�̃x�[�X
		unsigned int VertexNum;				// ���_��
		unsigned int IndexBase;				// �C���f�b�N�X�o�b�t�@�̃x�[�X
		unsigned int IndexNum;				// �C���f�b�N�X��
		std::string	 mtrlname;				// �}�e���A����
	};

	struct MATERIAL {
		std::string mtrlname;					// �}�e���A����
		aiColor4D	Ambient;					// �A���r�G���g
		aiColor4D	Diffuse;					// �f�B�t���[�Y
		aiColor4D	Specular;					// �X�y�L����
		aiColor4D	Emission;					// �G�~�b�V����
		float		Shiness;					// �V���C�l�X
		std::string diffusetexturename;			// �f�B�t���[�Y�e�N�X�`����
	};

	// �E�F�C�g���
	struct WEIGHT {
		std::string bonename;						// �{�[����
		std::string meshname;						// ���b�V����
		float weight;								// �E�F�C�g�l
		int	vertexindex;							// ���_�C���f�b�N�X
	};

	//�{�[���\����
	struct BONE
	{
		std::string bonename;						// �{�[����
		std::string meshname;						// ���b�V����
		std::string armaturename;					// �A�[�}�`���A��
		aiMatrix4x4 Matrix{};						// �e�q�֌W���l�������s��
		aiMatrix4x4 AnimationMatrix{};				// �����̎��������l�����s��
		aiMatrix4x4 OffsetMatrix{};					// �{�[���I�t�Z�b�g�s��
		int			idx;							// �z��̉��Ԗڂ�			
		std::vector<WEIGHT> weights;				// ���̃{�[�����e����^���钸�_�C���f�b�N�X�E�E�F�C�g�l
	};

	void GetModelData(std::string filename, std::string texturedirectory);

	std::vector<SUBSET> GetSubsets();

	std::vector<std::vector<VERTEX>> GetVertices();

	std::vector<std::vector<unsigned int>> GetIndices();

	std::vector<MATERIAL> GetMaterials();

	std::vector<std::unique_ptr<CTexture>> GetDiffuseTextures();

	std::unordered_map<std::string, BONE> GetBoneDictionary();

	CTreeNode<std::string> GetBoneNameTree();
}
