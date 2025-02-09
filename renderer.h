#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<io.h>
#include	<string>
#include	<vector>
#include	"NonCopyable.h"
#include	"AssimpPerse.h"

// �����N���ׂ��O�����C�u����
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// SRT���
struct SRT {
	DirectX::SimpleMath::Vector3 rot{};
	DirectX::SimpleMath::Vector3 pos{};
	DirectX::SimpleMath::Vector3 scale{};

	DirectX::SimpleMath::Matrix GetMatrix() const {
		return
			DirectX::SimpleMath::Matrix::CreateScale(scale) *
			DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z) *
			DirectX::SimpleMath::Matrix::CreateTranslation(pos);
	}
};

// �E�F�C�g���@20231225 �ǉ�
struct WEIGHT {
	std::string bonename;						// �{�[����
	std::string meshname;						// ���b�V����
	float weight;								// �E�F�C�g�l
	int	vertexindex;							// ���_�C���f�b�N�X
};

//�{�[���\���́@20231231 �C���iDX���j
struct BONE
{
	std::string bonename;							// �{�[����
	std::string meshname;							// ���b�V����
	std::string armaturename;						// �A�[�}�`���A��
	//	aiMatrix4x4 Matrix{};						// �e�q�֌W���l�������s��
	//	aiMatrix4x4 AnimationMatrix{};				// �����̎��������l�����s��
	//	aiMatrix4x4 OffsetMatrix{};					// �{�[���I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix Matrix{};			// �e�q�֌W���l�������s��
	DirectX::SimpleMath::Matrix AnimationMatrix{};	// �����̎��������l�����s��
	DirectX::SimpleMath::Matrix OffsetMatrix{};		// �{�[���I�t�Z�b�g�s��
	int			idx;								// �z��̉��Ԗڂ�			
	std::vector<WEIGHT> weights;					// ���̃{�[�����e����^���钸�_�C���f�b�N�X�E�E�F�C�g�l
};

// �R�c���_�f�[�^
struct VERTEX_3D
{
	DirectX::SimpleMath::Vector3	Position;
	DirectX::SimpleMath::Vector3	Normal;
	DirectX::SimpleMath::Color		Diffuse;
	DirectX::SimpleMath::Vector2	TexCoord;
	int		BoneIndex[4];			// 20231225
	float	BoneWeight[4];			// 20231225
	std::string	BoneName[4];		// 20231226
	int		bonecnt = 0;			// 20231226
};


// �}�e���A��
struct MATERIAL
{
	DirectX::SimpleMath::Color	Ambient;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Specular;
	DirectX::SimpleMath::Color	Emission;
	float		Shiness;
	BOOL		TextureEnable;
	float		Dummy[2]{};
};


// ���s����
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::SimpleMath::Vector4	Direction;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Ambient;
};

// ���b�V���i�}�e���A�����ɃT�u�Z�b�g�����݂���j
struct SUBSET {
	std::string		MtrlName;						// �}�e���A����
	unsigned int	IndexNum = 0;					// �C���f�b�N�X��
	unsigned int	VertexNum = 0;					// ���_��
	unsigned int	IndexBase = 0;					// �J�n�C���f�b�N�X
	unsigned int	VertexBase = 0;					// ���_�x�[�X
	unsigned int	MaterialIdx = 0;				// �}�e���A���C���f�b�N�X
};

// �u�����h�X�e�[�g
enum EBlendState {
	BS_NONE = 0,							// ��������������
	BS_ALPHABLEND,							// ����������
	BS_ADDITIVE,							// ���Z����
	BS_SUBTRACTION,							// ���Z����
	MAX_BLENDSTATE
};

// �{�[���R���r�l�[�V�����s��
constexpr int MAX_BONE = 400;
struct CBBoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[MAX_BONE];	// �{�[���R���r�l�[�V�����s��		// 20240713
};

// �����_��
class Renderer : NonCopyable
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;

	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState[MAX_BLENDSTATE];		// �u�����h �X�e�[�g;
	static ID3D11BlendState*		m_BlendStateATC;


public:

	static void Init();
	static void Uninit();
	static void Begin();
	static void End();

	static void SetDepthEnable(bool Enable);

	static void SetATCEnable(bool Enable);

	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix* WorldMatrix);
	static void SetViewMatrix(DirectX::SimpleMath::Matrix* ViewMatrix);
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetBoneCombMatrices(CBBoneCombMatrix* BoneCombMatrices);	// 20240704 add by tomoki.suzuki


	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);

	//=============================================================================
	// �u�����h �X�e�[�g�ݒ�
	//=============================================================================
	static void SetBlendState(int nBlendState)
	{
		if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_DeviceContext->OMSetBlendState(m_BlendState[nBlendState], blendFactor, 0xffffffff);
		}
	}
};