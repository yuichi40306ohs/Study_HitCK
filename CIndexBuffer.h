#pragma once

#include	<vector>
#include	<wrl/client.h>
#include	"dx11helper.h"
#include	"NonCopyable.h"

#include	"renderer.h"

using Microsoft::WRL::ComPtr;

class CIndexBuffer : NonCopyable {

	ComPtr<ID3D11Buffer> m_IndexBuffer;

public:
	void Create(const std::vector<unsigned int>& indices) {

		// �f�o�C�X�擾
		ID3D11Device* device = nullptr;

		device = Renderer::GetDevice();

		assert(device);

		// �C���f�b�N�X�o�b�t�@�쐬
		bool sts = CreateIndexBuffer(
			device,										// �f�o�C�X
			(unsigned int)(indices.size()),				// �C���f�b�N�X��
			(void*)indices.data(),						// �C���f�b�N�X�f�[�^�擪�A�h���X
			&m_IndexBuffer);							// �C���f�b�N�X�o�b�t�@

		assert(sts == true);
	}

	void SetGPU() {
		// �f�o�C�X�R���e�L�X�g�擾
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = Renderer::GetDeviceContext();

		// �C���f�b�N�X�o�b�t�@���Z�b�g
		devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}
};
