#include	<simplemath.h>
#include	<iostream>

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CSphereMesh.h"
#include	"CCylinderMesh.h"

static CCylinderMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;

using namespace DirectX::SimpleMath;

void CylinderDrawerDraw(float radius, float hieght, DirectX::SimpleMath::Color col, float posx, float posy, float posz);

void CylinderDrawerInit()
{
	g_mesh.Init(50,			// •ªŠ„”
		1,					// ”¼Œa
		1,					// ‚‚³
		Color(1,1,1,1));

	g_renderer.Init(g_mesh);

	MATERIAL mtrl;
	// ƒ}ƒeƒŠƒAƒ‹¶¬
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;

	g_material.Create(mtrl);
}

void CylinderDrawerDraw(float radius, float height, DirectX::SimpleMath::Color col, float posx, float posy, float posz)
{
	Matrix mtx = Matrix::CreateScale(radius,height,radius);
//	Matrix mtx = Matrix::Identity;

	mtx._41 = posx;
	mtx._42 = posy;
	mtx._43 = posz;

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}
