#pragma once

#include "CVertexBuffer.h"
#include "CIndexBuffer.h"
#include "CShader.h"
#include "CMaterial.h"
#include "CTexture.h"

class Field {
	// SRT���i�p�����j
	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	// �`��ׂ̈̏��i���b�V���Ɋւ����j
	CIndexBuffer				m_IndexBuffer;				// �C���f�b�N�X�o�b�t�@
	CVertexBuffer<VERTEX_3D>	m_VertexBuffer;				// ���_�o�b�t�@

	// �`��ׂ̈̏��i�����ڂɊւ�镔���j
	CShader						m_Shader;					// �V�F�[�_�[
	CMaterial					m_Material;					// �}�e���A��
	CTexture					m_Texture;					// �e�N�X�`��
public:
	void Init();
	void Draw();
	void Dispose();
};