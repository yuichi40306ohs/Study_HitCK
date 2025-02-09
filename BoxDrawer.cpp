#include	<simplemath.h>
#include	<iostream>

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CBoxMesh.h"
#include    "CShader.h"

static CBoxMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;
static CShader g_shader;

using namespace DirectX::SimpleMath;

void BoxDrawerInit()
{
	g_mesh.Init(
		1,					// 幅
		1,					// 高さ
		1,					// 奥行
		Color(1,1,1,1));

	g_renderer.Init(g_mesh);

	// シェーダーの初期化
	g_shader.Create(
		"shader/unlitTextureVS.hlsl",				// 頂点シェーダー
		"shader/unlitTexturePS.hlsl");			// ピクセルシェーダー

	MATERIAL mtrl;
	// マテリアル生成
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;
	g_material.Create(mtrl);
}

void BoxDrawerDraw(float width, float height, float depth, 
	DirectX::SimpleMath::Color col, float posx, float posy, float posz)
{
	Matrix mtx = Matrix::CreateScale(width,height,depth);

	mtx._41 = posx;
	mtx._42 = posy;
	mtx._43 = posz;

	g_shader.SetGPU();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}

void BoxDrawerDraw(SRT srt,DirectX::SimpleMath::Color col)
{
	Matrix mtx = srt.GetMatrix();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();
	g_material.SetGPU();

	g_shader.SetGPU();

	g_renderer.Draw();
}

void BoxDrawerDraw(Matrix mtx, DirectX::SimpleMath::Color col)
{
	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();
	g_material.SetGPU();

	g_shader.SetGPU();

	g_renderer.Draw();
}
