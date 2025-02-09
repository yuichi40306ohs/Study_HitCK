#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<SimpleMath.h>
#include	<io.h>
#include	<string>
#include	<vector>
#include	"NonCopyable.h"
#include	"AssimpPerse.h"

// リンクすべき外部ライブラリ
#pragma comment(lib,"directxtk.lib")
#pragma comment(lib,"d3d11.lib")

// SRT情報
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

// ウェイト情報　20231225 追加
struct WEIGHT {
	std::string bonename;						// ボーン名
	std::string meshname;						// メッシュ名
	float weight;								// ウェイト値
	int	vertexindex;							// 頂点インデックス
};

//ボーン構造体　20231231 修正（DX化）
struct BONE
{
	std::string bonename;							// ボーン名
	std::string meshname;							// メッシュ名
	std::string armaturename;						// アーマチュア名
	//	aiMatrix4x4 Matrix{};						// 親子関係を考慮した行列
	//	aiMatrix4x4 AnimationMatrix{};				// 自分の事だけを考えた行列
	//	aiMatrix4x4 OffsetMatrix{};					// ボーンオフセット行列
	DirectX::SimpleMath::Matrix Matrix{};			// 親子関係を考慮した行列
	DirectX::SimpleMath::Matrix AnimationMatrix{};	// 自分の事だけを考えた行列
	DirectX::SimpleMath::Matrix OffsetMatrix{};		// ボーンオフセット行列
	int			idx;								// 配列の何番目か			
	std::vector<WEIGHT> weights;					// このボーンが影響を与える頂点インデックス・ウェイト値
};

// ３Ｄ頂点データ
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


// マテリアル
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


// 平行光源
struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	DirectX::SimpleMath::Vector4	Direction;
	DirectX::SimpleMath::Color	Diffuse;
	DirectX::SimpleMath::Color	Ambient;
};

// メッシュ（マテリアル毎にサブセットが存在する）
struct SUBSET {
	std::string		MtrlName;						// マテリアル名
	unsigned int	IndexNum = 0;					// インデックス数
	unsigned int	VertexNum = 0;					// 頂点数
	unsigned int	IndexBase = 0;					// 開始インデックス
	unsigned int	VertexBase = 0;					// 頂点ベース
	unsigned int	MaterialIdx = 0;				// マテリアルインデックス
};

// ブレンドステート
enum EBlendState {
	BS_NONE = 0,							// 半透明合成無し
	BS_ALPHABLEND,							// 半透明合成
	BS_ADDITIVE,							// 加算合成
	BS_SUBTRACTION,							// 減算合成
	MAX_BLENDSTATE
};

// ボーンコンビネーション行列
constexpr int MAX_BONE = 400;
struct CBBoneCombMatrix {
	DirectX::XMFLOAT4X4 BoneCombMtx[MAX_BONE];	// ボーンコンビネーション行列		// 20240713
};

// レンダラ
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

	static ID3D11BlendState*		m_BlendState[MAX_BLENDSTATE];		// ブレンド ステート;
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
	// ブレンド ステート設定
	//=============================================================================
	static void SetBlendState(int nBlendState)
	{
		if (nBlendState >= 0 && nBlendState < MAX_BLENDSTATE) {
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_DeviceContext->OMSetBlendState(m_BlendState[nBlendState], blendFactor, 0xffffffff);
		}
	}
};