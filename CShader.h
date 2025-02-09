#pragma once
#include	<wrl/client.h>
#include	<string>

#include	<d3d11.h>
#include	"NonCopyable.h"

using Microsoft::WRL::ComPtr;

class CShader : NonCopyable{
public:
	void Create(std::string vs, std::string ps);
	void SetGPU();
private:
	ComPtr<ID3D11VertexShader> m_pVertexShader;		// ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader>  m_pPixelShader;		// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>  m_pVertexLayout;		// ���_���C�A�E�g
};

