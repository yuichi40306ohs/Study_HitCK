#include	<vector>
#include	<iostream>
#include	<unordered_map>
#include	<cassert>
#include	"CTexture.h"
#include	"AssimpPerse.h"
#include	"CTreeNode.h"

#pragma comment(lib, "assimp-vc143-mtd.lib")

namespace AssimpPerse
{
	std::vector<std::vector<VERTEX>> g_vertices{};				// ���_�f�[�^�i���b�V���P�ʁj

	std::vector<std::vector<unsigned int>> g_indices{};			// �C���f�b�N�X�f�[�^�i���b�V���P�ʁj

	std::vector<SUBSET> g_subsets{};							// �T�u�Z�b�g���

	std::vector<MATERIAL> g_materials{};						// �}�e���A��

	std::vector<std::unique_ptr<CTexture>> g_diffuseTextures;	// �f�B�t���[�Y�e�N�X�`���Q

	std::unordered_map<std::string, BONE> g_BoneDictionary;		// �{�[�������i�L�[�F�{�[�����j
	
	std::vector<std::vector<BONE>>	g_BonesPerMeshes;			// ���b�V���P�ʂŃ{�[�������W�߂�����

	CTreeNode<std::string>	g_AssimpNodeNameTree;				// assimp�m�[�h���c���[

	// �m�[�h���c���[�𐶐�����
	void CreateNodeTree(aiNode* node, CTreeNode<std::string>* ptree) {

		ptree->m_nodedata = std::string(node->mName.C_Str());
//		std::cout << node->mName.C_Str() << std::endl;

		for (unsigned int n = 0; n < node->mNumChildren; n++)
		{
			std::unique_ptr<CTreeNode<std::string>> pchild = std::make_unique<CTreeNode<std::string>>();
			pchild->m_parent = ptree;
			ptree->Addchild(std::move(pchild));
			CreateNodeTree(node->mChildren[n], ptree->m_children[n].get());
		}
	}

	CTreeNode<std::string> GetBoneNameTree() 
	{
		return std::move(g_AssimpNodeNameTree);
	}

	// �{�[��������Ԃ�	
	std::unordered_map<std::string, BONE> GetBoneDictionary()
	{
		return g_BoneDictionary;
	}

	// ��̃{�[�������i�L�[�̓{�[�����j���쐬����i�m�[�h���ċA�ŒH���̎������쐬����j
	void CreateEmptyBoneDictionary(aiNode* node)
	{
		BONE bone{};

		// �{�[�����ŎQ�Ƃł���悤�ɋ�̃{�[�������Z�b�g����
		g_BoneDictionary[node->mName.C_Str()] = bone;

		std::cout << node->mName.C_Str() << std::endl;

		for (unsigned int n = 0; n < node->mNumChildren; n++)
		{
			CreateEmptyBoneDictionary(node->mChildren[n]);
		}
	}

	// �T�u�Z�b�g�ɕR�Â��Ă���{�[�������擾����
	std::vector<BONE> GetBonesPerMesh(const aiMesh* mesh)
	{
		std::vector<BONE> bones;		// ���̃T�u�Z�b�g���b�V���Ŏg�p����Ă���{�[���R���e�i

		// �{�[���������[�v
		for (unsigned int bidx = 0; bidx < mesh->mNumBones; bidx++) {

			BONE bone{};

			// �{�[�����擾
			bone.bonename = std::string(mesh->mBones[bidx]->mName.C_Str());

			// ���b�V���m�[�h��
			bone.meshname = std::string(mesh->mBones[bidx]->mNode->mName.C_Str());

			// �A�[�}�`���A�m�[�h��
			bone.armaturename = std::string(mesh->mBones[bidx]->mArmature->mName.C_Str());

			// �f�o�b�O�p
			std::cout << bone.bonename
				<< "(" << bone.meshname << ")"
				<< "(" << bone.armaturename << ")"
				<< std::endl;

			// �{�[���I�t�Z�b�g�s��擾
			bone.OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;						

			// �E�F�C�g��񒊏o
			bone.weights.clear();
			for (unsigned int widx = 0; widx < mesh->mBones[bidx]->mNumWeights; widx++) {

				WEIGHT w;
				w.meshname = bone.meshname;										// ���b�V����
				w.bonename = bone.bonename;										// �{�[����

				w.weight = mesh->mBones[bidx]->mWeights[widx].mWeight;			// �d��
				w.vertexindex = mesh->mBones[bidx]->mWeights[widx].mVertexId;	// ���_�C���f�b�N�X
				bone.weights.emplace_back(w);
			}

			// �R���e�i�ɓo�^
			bones.emplace_back(bone);

			// �{�[�������ɂ����f������
			g_BoneDictionary[mesh->mBones[bidx]->mName.C_Str()].OffsetMatrix = mesh->mBones[bidx]->mOffsetMatrix;
		}

		return bones;
	}

	// �{�[�����A�{�[���C���f�b�N�X�A�{�[���E�F�C�g�𒸓_�ɃZ�b�g����i20231225�ǉ��j
	void SetBoneDataToVertices() {

		// �{�[���C���f�b�N�X��������
		for (auto& vtbl : g_vertices) {
			for (auto& v : vtbl) {
				v.bonecnt = 0;
				for (int b = 0; b < 4; b++) {
					v.BoneIndex[b] = -1;
					v.BoneWeight[b] = 0.0f;
					v.BoneName[b] = "";
				}
			}
		}

		// ���b�V�����̃{�[���R���e�i
		int subsetid = 0;
		for (auto& bones : g_BonesPerMeshes) {

			// ���̃X�^�e�B�b�N���b�V�����̒��_�f�[�^�̃X�^�[�g�ʒu���擾
//			int vertexbase = g_subsets[subsetid].VertexBase;

			// ���̃T�u�Z�b�g���̃{�[�����ЂƂÂ��o��
			for (auto& bone : bones)
			{
				for (auto& w : bone.weights) {
					int& idx = g_vertices[subsetid][w.vertexindex].bonecnt;

					g_vertices[subsetid][w.vertexindex].BoneName[idx] = w.bonename;	// �{�[�������Z�b�g
					g_vertices[subsetid][w.vertexindex].BoneWeight[idx] = w.weight;	// weight�l���Z�b�g
					g_vertices[subsetid][w.vertexindex].BoneIndex[idx] = g_BoneDictionary[w.bonename].idx;

					//�{�[���̔z��ԍ����Z�b�g
					idx++;
					assert(idx <= 4);
				}
			}
			subsetid++;				// ���̃��b�V����
		}
	}

	// �{�[�������擾����i�m�[�h���ċA�ŒH��{�[�������擾����j
	void GetBone(const aiScene* pScene)
	{
		// ��̃{�[���������쐬����i�L�[�i�{�[�����j�����̎��o���j
		CreateEmptyBoneDictionary(pScene->mRootNode);

		// �{�[���̔z��ʒu�i�C���f�b�N�X�l�j���i�[����
		unsigned int num = 0;						
		for (auto& data : g_BoneDictionary) 
		{											
			data.second.idx = num;					
			num++;									
		}					

		// ���b�V���������[�v
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// �T�u�Z�b�g�ɕR�Â��Ă���{�[�������擾����
			std::vector<BONE> BonesPerMesh = GetBonesPerMesh(mesh);
			g_BonesPerMeshes.emplace_back(BonesPerMesh);
		}

		// ���_�f�[�^�Ƀ{�[�������Z�b�g����
		SetBoneDataToVertices();

		// �{�[���̃m�[�h���c���[�𐶐�����
		CreateNodeTree(pScene->mRootNode, &g_AssimpNodeNameTree);
	}

	// �f�B�t���[�Y�b�s�������������R���e�i��Ԃ�
	std::vector<std::unique_ptr<CTexture>> GetDiffuseTextures()
	{
		return std::move(g_diffuseTextures);
	}

	// �}�e���A���������������������g�p���Ď擾����
	void GetMaterialData(const aiScene* pScene,std::string texturedirectory)
	{
		// �}�e���A�������e�N�X�`���i�[�G���A��p�ӂ���
		g_diffuseTextures.resize(pScene->mNumMaterials);

		// �}�e���A���������[�v
		for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
		{
			aiMaterial* material = pScene->mMaterials[m];

			// �}�e���A�����擾
			std::string mtrlname = std::string(material->GetName().C_Str());
			std::cout << mtrlname << std::endl;

			// �}�e���A�����
			aiColor4D ambient;
			aiColor4D diffuse;
			aiColor4D specular;
			aiColor4D emission;
			float shiness;

			// �A���r�G���g
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
			}
			else {
					ambient = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// �f�B�t���[�Y
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
			}
			else {
					diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �X�y�L����
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
			}
			else {
					specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// �G�~�b�V����
			if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
			}
			else {
					emission = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
			}

			// �V���C�l�X
			if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shiness)) {
			}
			else {
					shiness = 0.0f;
			}

			// ���̃}�e���A���ɕR�Â��Ă���f�B�t���[�Y�e�N�X�`���������[�v
			std::vector<std::string> texpaths{};

			for (unsigned int t = 0; t < material->GetTextureCount(aiTextureType_DIFFUSE); t++)
			{
				aiString path{};

				// t�Ԗڂ̃e�N�X�`���p�X�擾
				if (AI_SUCCESS == material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, t), path))
				{
					// �e�N�X�`���p�X�擾
					std::string texpath = std::string(path.C_Str());
					std::cout << texpath << std::endl;

					texpaths.push_back(texpath);
					// �����e�N�X�`�����ǂ����𔻒f����
					if (auto tex = pScene->GetEmbeddedTexture(path.C_Str())) {

						std::unique_ptr<CTexture> texture =std::make_unique<CTexture>();

						// �����e�N�X�`���̏ꍇ
						bool sts = texture->LoadFromFemory(
							(unsigned char*)tex->pcData,			// �擪�A�h���X
							tex->mWidth);			// �e�N�X�`���T�C�Y�i�������ɂ���ꍇ�����T�C�Y�j	
						if (sts) {
							g_diffuseTextures[m] = std::move(texture);
						}
						std::cout << "Embedded" << std::endl;

					}
					else {
						// �O���e�N�X�`���t�@�C���̏ꍇ
						std::unique_ptr<CTexture>	texture;
						texture = std::make_unique<CTexture>();

						std::string texname = texturedirectory + "/" + texpath;

						bool sts=texture->Load(texname);
						if (sts) {
							g_diffuseTextures[m] = std::move(texture);
						}

						std::cout << "other Embedded" << std::endl;
					}
				}
				// �f�B�t���[�Y�e�N�X�`�����Ȃ������ꍇ
				else
				{
					// �O���e�N�X�`���t�@�C���̏ꍇ
					std::unique_ptr<CTexture>	texture;
					texture = std::make_unique<CTexture>();
					g_diffuseTextures[m] = std::move(texture);
				}
			}

			// �}�e���A������ۑ�
			AssimpPerse::MATERIAL mtrl{};

			mtrl.mtrlname = mtrlname;
			mtrl.Ambient = ambient;
			mtrl.Diffuse = diffuse;
			mtrl.Specular = specular;
			mtrl.Emission = emission;
			mtrl.Shiness = shiness;

			if (texpaths.size() == 0)
				mtrl.diffusetexturename = "";
			else
				mtrl.diffusetexturename = texpaths[0];

			g_materials.push_back(mtrl);
		}

	}

	void GetModelData(std::string filename,std::string texturedirectory)
	{
		// �V�[�����\�z
		Assimp::Importer importer;

		// �V�[�������\�z
		const aiScene* pScene = importer.ReadFile(
			filename.c_str(),
//			aiProcess_ConvertToLeftHanded |	// ������W�n�ɕϊ�����
//			aiProcess_Triangulate);			// �O�p�`������
			aiProcessPreset_TargetRealtime_MaxQuality |
			aiProcess_ConvertToLeftHanded |
			aiProcess_PopulateArmatureData);		// 20231225�ǉ�

		if (pScene == nullptr)
		{
			std::cout << "load error" << filename.c_str() << importer.GetErrorString() << std::endl;
		}
		assert(pScene != nullptr);

		// �}�e���A�����擾
		GetMaterialData(pScene,texturedirectory);

		// ���b�V���������[�v�i�}�e���A�����Ƀ��b�V���𕪊�����悤�Ɏw�肵�Ă���j
		g_vertices.resize(pScene->mNumMeshes);

		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// ���b�V�����擾
			std::string meshname = std::string(mesh->mName.C_Str());

			//�@���_�������[�v
			for (unsigned int vidx = 0; vidx < mesh->mNumVertices; vidx++)
			{
				// ���_�f�[�^
				VERTEX	v{};
				v.meshname = meshname;		// ���b�V�����Z�b�g

				// ���W		
				v.pos = mesh->mVertices[vidx];

				// ���̒��_���g�p���Ă���}�e���A���̃C���f�b�N�X�ԍ��i���b�V�����́j
				// ���g�p���ă}�e���A�������Z�b�g
				v.materialindex = mesh->mMaterialIndex;

				v.mtrlname = g_materials[mesh->mMaterialIndex].mtrlname;

				// �@������H
				if (mesh->HasNormals()) {
					v.normal = mesh->mNormals[vidx];
				}
				else
				{
					v.normal = aiVector3D(0.0f, 0.0f, 0.0f);
				}

				// ���_�J���[�H�i�O�Ԗځj
				if (mesh->HasVertexColors(0)) {
					v.color = mesh->mColors[0][vidx];
				}
				else
				{
					v.color = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
				}

				// �e�N�X�`������H�i�O�Ԗځj
				if (mesh->HasTextureCoords(0)) {
					v.texcoord = mesh->mTextureCoords[0][vidx];
				}
				else
				{
					v.texcoord = aiVector3D(0.0f, 0.0f, 0.0f);
				}

				// ���_�f�[�^��ǉ�
				g_vertices[m].push_back(v);
			}
		}

		// ���b�V���������[�v
		// �C���f�b�N�X�f�[�^�쐬
		g_indices.resize(pScene->mNumMeshes);
		for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
		{
			aiMesh* mesh = pScene->mMeshes[m];

			// ���b�V�����擾
			std::string meshname = std::string(mesh->mName.C_Str());

			// �C���f�b�N�X�������[�v
			for (unsigned int fidx = 0; fidx < mesh->mNumFaces; fidx++)
			{
				aiFace face = mesh->mFaces[fidx];

//				assert(face.mNumIndices == 3);	// �O�p�`�̂ݑΉ�   car000.x�@�Ή�
 				assert(face.mNumIndices <= 3);	// �O�p�`�ȉ��ł����OK�i�k�ރ|���S���j

				// �C���f�b�N�X�f�[�^��ǉ�
				for (unsigned int i = 0; i < face.mNumIndices; i++)
				{
					g_indices[m].push_back(face.mIndices[i]);
				}
			}
		}

		// �T�u�Z�b�g���𐶐�
		g_subsets.resize(pScene->mNumMeshes);
		for (unsigned int m = 0; m < g_subsets.size(); m++)
		{
			g_subsets[m].IndexNum = static_cast<unsigned int>(g_indices[m].size());
			g_subsets[m].VertexNum = static_cast<unsigned int>(g_vertices[m].size());
			g_subsets[m].VertexBase = 0;
			g_subsets[m].IndexBase = 0;
			g_subsets[m].meshname = g_vertices[m][0].meshname;
			g_subsets[m].mtrlname = g_vertices[m][0].mtrlname;
			g_subsets[m].materialindex = g_vertices[m][0].materialindex;
		}

		// �T�u�Z�b�g���𑊑ΓI�Ȃ��̂ɂ���	
		for (int m = 0; m < g_subsets.size(); m++)
		{
			// ���_�o�b�t�@�̃x�[�X���v�Z
			g_subsets[m].VertexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				g_subsets[m].VertexBase += g_subsets[i].VertexNum;
			}

			// �C���f�b�N�X�o�b�t�@�̃x�[�X���v�Z
			g_subsets[m].IndexBase = 0;
			for (int i = m - 1; i >= 0; i--) {
				g_subsets[m].IndexBase += g_subsets[i].IndexNum;
			}
		}

		// �{�[�����擾	
		GetBone(pScene);
	}

	std::vector<SUBSET> GetSubsets() 
	{
		return g_subsets;
	}

		// �T�u�Z�b�g���{
	std::vector<std::vector<VERTEX>> GetVertices() 
	{
		return g_vertices;		// ���_�f�[�^�i���b�V���P�ʁj
	}

	std::vector<std::vector<unsigned int>> GetIndices() 
	{
		return g_indices;		// �C���f�b�N�X�f�[�^�i���b�V���P�ʁj
	}

	std::vector<MATERIAL> GetMaterials()
	{
		return g_materials;		// �}�e���A��
	}
}
