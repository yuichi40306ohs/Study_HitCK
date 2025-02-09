#include	<simplemath.h>
#include	<iostream>

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CSphereMesh.h"

static CSphereMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;

using namespace DirectX::SimpleMath;

void SphereDrawerInit() 
{
	g_mesh.Init(1, Color(1, 1, 1, 1), 100, 100);
	g_renderer.Init(g_mesh);

	MATERIAL mtrl;
	// É}ÉeÉäÉAÉãê∂ê¨
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;

	g_material.Create(mtrl);
}

void SphereDrawerDraw(float radius,Color col,float ex, float ey, float ez)
{
	Matrix mtx = Matrix::CreateScale(radius);

	mtx._41 = ex;
	mtx._42 = ey;
	mtx._43 = ez;

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}
