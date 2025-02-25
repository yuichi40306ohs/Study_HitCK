#pragma once

#include	<SimpleMath.h>
#include	"Application.h"
#include	"CPolar3D.h"
#include	"Renderer.h"

class Camera {
protected:
	DirectX::SimpleMath::Vector3	m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3	m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	DirectX::SimpleMath::Vector3	m_Target{};
	DirectX::SimpleMath::Matrix		m_ViewMatrix{};

public:
	void Init();
	void Dispose();
	void Update();
	void Draw();

};

class FreeCamera : public Camera {
	float   m_elevation = 90.0f*DirectX::XM_PI/180.0f;		// 仰角
	float	m_azimuth = 0.0f;								// 方位角	
	float   m_radius = 100.0f;								// 半径	

	DirectX::SimpleMath::Vector3 m_UP= DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

public:		
	void SetElevation(float elevation) {
		m_elevation = elevation;
	}

	void SetAzimuth(float azimuth) {
		m_azimuth = azimuth;
	}

	void SetRadius(float radius) {
		m_radius = radius;
	}

	void Update() {
		// 仰角と方位角からカメラの位置を計算
		CPolor3D polar(m_radius,m_elevation,m_azimuth);

		m_Position = polar.ToOyler();

		CPolor3D polarup(m_radius,(m_elevation+ 90.0f * DirectX::XM_PI / 180.0f),m_azimuth);

		m_UP = polarup.ToOyler();
	}
	
	void Draw() {
		m_ViewMatrix =
			DirectX::XMMatrixLookAtLH(
				m_Position,
				m_Target,
				m_UP);				// 左手系にした　20230511 by suzuki.tomoki

		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
		//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

		Renderer::SetViewMatrix(&m_ViewMatrix);

		//プロジェクション行列の生成
		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角

		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
		float nearPlane = 1.0f;       // ニアクリップ
		float farPlane = 3000.0f;      // ファークリップ

		//プロジェクション行列の生成
		DirectX::SimpleMath::Matrix projectionMatrix;
		projectionMatrix =
			DirectX::XMMatrixPerspectiveFovLH(
				fieldOfView,
				aspectRatio,
				nearPlane,
				farPlane);	// 左手系にした　20230511 by suzuki.tomoki

		// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
		// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
		// このコードは確認テストのために残す
	//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

		Renderer::SetProjectionMatrix(&projectionMatrix);

	}
};