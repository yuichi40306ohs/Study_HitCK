#pragma once

#include	<d3d11.h>
#include	<string>
#include	<wrl/client.h>							// ComPtr�̒�`���܂ރw�b�_�t�@�C��
#include	<filesystem>
#include	"NonCopyable.h"

using Microsoft::WRL::ComPtr;

// �e�N�X�`���������N���X
class CTexture : NonCopyable
{
	std::string m_texname{};						// �t�@�C����
	ComPtr<ID3D11ShaderResourceView> m_srv{};		// �V�F�[�_�[���\�[�X�r���[

	int m_width;									// ��
	int m_height;									// ����
	int m_bpp;										// BPP
public:
	bool Load(const std::string& filename);
	bool LoadFromFemory(const unsigned char* data,int len);

	void SetGPU();
};