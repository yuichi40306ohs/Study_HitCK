#include	<SimpleMath.h>

#include	"CShader.h"
#include	"Camera.h"
#include	"SphereDrawer.h"
#include	"Collision.h"
#include	"BoxDrawer.h"

#include	"DebugUI.h"

using namespace DirectX::SimpleMath;

constexpr float RADIUS = 10.0f;

// ���a
static float g_radiusSphere = RADIUS;
static float g_radiusCylinder = RADIUS;

static float g_heightCylinder = 100;

// �V�F�[�_�[
static CShader	g_Shader;

// �J����
FreeCamera  g_camera;

// ���̈ʒu
Vector3	g_PositionSphere = Vector3(0, 0, 0);

// �~���̈ʒu
Vector3	g_PositionCylinder = Vector3(0, 0, 0);

// �{�b�N�X��SRT
Vector3 g_PositionBox = Vector3(0, 0, 0);
Vector3 g_RotationBox = Vector3(0, 0, 0);
Vector3 g_ScaleBox = Vector3(1, 1, 1);

// �t���[�J�����p�����[�^
static float g_Azimuth = 0.5 * DirectX::XM_PI;
static float g_Elevation = -0.5 * DirectX::XM_PI;
static float g_CameraRadius = 100.0f;

static void DebugSphereMoveParameter() {

	ImGui::Begin("Debug Sphere Move Parameter");

	ImGui::SliderFloat3("Position", &g_PositionSphere.x, -300.0f, 300.0f);
	ImGui::SliderFloat("Radius", &g_radiusSphere, 10.0f, 80.0f);

	ImGui::End();
}

static void DebugBSBCParameter() {

	ImGui::Begin("Debug BSphere BCylinder Parameter");

	ImGui::Text("BS Position %f %f %f", g_PositionSphere.x, g_PositionSphere.y,g_PositionSphere.z);
	ImGui::Text("BS radius %f", g_radiusSphere);

	ImGui::Text("BC Position %f %f %f", g_PositionCylinder.x, g_PositionCylinder.y, g_PositionCylinder.z);
	ImGui::Text("BC radius %f", g_radiusCylinder);

	ImGui::End();
}

	
static void DebugBoxSRTParameter() {

	ImGui::Begin("Debug Box Move Parameter");

	ImGui::SliderFloat3("Position", &g_PositionBox.x, -300.0f, 300.0f);
	ImGui::SliderFloat3("Rotation", &g_RotationBox.x, -DirectX::XM_PI, DirectX::XM_PI);
	ImGui::SliderFloat3("Scale", &g_ScaleBox.x, 1.0f, 50.0f);

	ImGui::End();
}


static void DebugFreeCamera() {

	ImGui::Begin("Debug Free Camera Parameter");

	ImGui::SliderFloat("Radius", &g_CameraRadius, 10.0f, 500.0f);
	ImGui::SliderFloat("Azimuth", &g_Azimuth, 0.0f, 2*DirectX::XM_PI);
	ImGui::SliderFloat("Elevation", &g_Elevation, 0.0f, DirectX::XM_PI);

	g_camera.SetRadius(g_CameraRadius);
	g_camera.SetAzimuth(g_Azimuth);
	g_camera.SetElevation(g_Elevation);

	g_camera.Update();

	ImGui::End();
}

void SceneInit() 
{
	g_camera.Init();

	// �V�F�[�_�I�u�W�F�N�g����
	g_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	
	// ���`�揉����
	SphereDrawerInit();
	BoxDrawerInit();

	// �f�o�b�O���
	DebugUI::RedistDebugFunction(DebugSphereMoveParameter);
	DebugUI::RedistDebugFunction(DebugBSBCParameter);
	DebugUI::RedistDebugFunction(DebugFreeCamera);
	DebugUI::RedistDebugFunction(DebugBoxSRTParameter);
}


void SceneUpdate() 
{
}

void SceneDraw()
{
	g_camera.Draw();

	g_Shader.SetGPU();

	// ���E���̒�`
	BoundingSphere bs = BoundingSphere(
		Vector3(g_PositionSphere.x, 
				g_PositionSphere.y, 
				g_PositionSphere.z),						// ���S
		g_radiusSphere);									// ���a		

	// OBB�̒�`
	BoundingBoxOBB obb = SetOBB(
		g_RotationBox,					// �p��
		g_PositionBox,					// �ʒu�i���[���h���W�j
		g_ScaleBox.x,					// ��
		g_ScaleBox.y,					// ����	
		g_ScaleBox.z);					// ���s��		

	//2025-1-31�ǋL
	// ����OBB�{�b�N�X�̓����蔻��
	bool sts = CollisionSphereOBB(bs, obb);

	Color color(1, 1, 1, 0.4f);
	// �ڐG�����ꍇ
	if(sts) 
	{
		color = Color(1, 0, 0, 0.4f);
	}
	else 
	{
		color = Color(1, 1, 1, 0.4f);
	}

	// ���`��
	SphereDrawerDraw(g_radiusSphere, color, g_PositionSphere.x, g_PositionSphere.y, g_PositionSphere.z);

	// �{�b�N�X�`��
	SRT srt;
	srt.pos = g_PositionBox;
	srt.rot = g_RotationBox;
	srt.scale = g_ScaleBox;

	srt.GetMatrix();
	BoxDrawerDraw(srt.GetMatrix(), color);
}

void SceneDisose() 
{

}