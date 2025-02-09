#include	<SimpleMath.h>

#include	"CShader.h"
#include	"Camera.h"
#include	"SphereDrawer.h"
#include	"Collision.h"
#include	"BoxDrawer.h"

#include	"DebugUI.h"

using namespace DirectX::SimpleMath;

constexpr float RADIUS = 10.0f;

// 半径
static float g_radiusSphere = RADIUS;
static float g_radiusCylinder = RADIUS;

static float g_heightCylinder = 100;

// シェーダー
static CShader	g_Shader;

// カメラ
FreeCamera  g_camera;

// 球の位置
Vector3	g_PositionSphere = Vector3(0, 0, 0);

// 円柱の位置
Vector3	g_PositionCylinder = Vector3(0, 0, 0);

// ボックスのSRT
Vector3 g_PositionBox = Vector3(0, 0, 0);
Vector3 g_RotationBox = Vector3(0, 0, 0);
Vector3 g_ScaleBox = Vector3(1, 1, 1);

// フリーカメラパラメータ
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

	// シェーダオブジェクト生成
	g_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	
	// 球描画初期化
	SphereDrawerInit();
	BoxDrawerInit();

	// デバッグ情報
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

	// 境界球の定義
	BoundingSphere bs = BoundingSphere(
		Vector3(g_PositionSphere.x, 
				g_PositionSphere.y, 
				g_PositionSphere.z),						// 中心
		g_radiusSphere);									// 半径		

	// OBBの定義
	BoundingBoxOBB obb = SetOBB(
		g_RotationBox,					// 姿勢
		g_PositionBox,					// 位置（ワールド座標）
		g_ScaleBox.x,					// 幅
		g_ScaleBox.y,					// 高さ	
		g_ScaleBox.z);					// 奥行き		

	//2025-1-31追記
	// 球とOBBボックスの当たり判定
	bool sts = CollisionSphereOBB(bs, obb);

	Color color(1, 1, 1, 0.4f);
	// 接触した場合
	if(sts) 
	{
		color = Color(1, 0, 0, 0.4f);
	}
	else 
	{
		color = Color(1, 1, 1, 0.4f);
	}

	// 球描画
	SphereDrawerDraw(g_radiusSphere, color, g_PositionSphere.x, g_PositionSphere.y, g_PositionSphere.z);

	// ボックス描画
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