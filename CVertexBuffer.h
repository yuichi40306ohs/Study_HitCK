#pragma once
#include	<vector>
#include	<wrl/client.h>
#include	"dx11helper.h"
#include	"renderer.h"
#include	"NonCopyable.h"


using Microsoft::WRL::ComPtr;

template <typename T>
class CVertexBuffer : NonCopyable{

	ComPtr<ID3D11Buffer> m_VertexBuffer;

public:
	void Create(const std::vector<T>& vertices) {

		// �f�o�C�X�擾
		ID3D11Device* device = nullptr;
		device = Renderer::GetDevice();
		assert(device);

		// ���_�o�b�t�@�쐬
		bool sts = CreateVertexBufferWrite(
			device,
			sizeof(T),						// �P���_������o�C�g��
			(unsigned int)vertices.size(),	// ���_��
			(void*)vertices.data(),			// ���_�f�[�^�i�[�������擪�A�h���X
			&m_VertexBuffer);				// ���_�o�b�t�@

		assert(sts == true);
	}

	// GPU�ɃZ�b�g
	void SetGPU() {

		// �f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// ���_�o�b�t�@���Z�b�g����
		unsigned int stride = sizeof(T);
		unsigned  offset = 0;
		devicecontext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	}

	// ���_�o�b�t�@������������
	void Modify(const std::vector<T>& vertices)
	{
		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		HRESULT hr = Renderer::GetDeviceContext()->Map(
			m_VertexBuffer.Get(), 
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr)) {
			memcpy(msr.pData, vertices.data(), vertices.size() * sizeof(T));
			Renderer::GetDeviceContext()->Unmap(m_VertexBuffer.Get(), 0);
		}
	}
};
