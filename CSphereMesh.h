#pragma once
#include	<simplemath.h>
#include	"CMesh.h"

class CSphereMesh : public CMesh {

	// 頂点データ生成
	void CreateVertex();
	// インデックス生成
	void CreateIndex();

public:
	// 初期化
	void Init(
		float radius,						// 半径
		DirectX::SimpleMath::Color color,	// 色
		uint32_t division_x,				// X方向の分割数
		uint32_t division_y)				// Y方向の分割数;
	{
		m_color = color;
		m_radius = radius;
		m_division_x = division_x;
		m_division_y = division_y;

		CreateIndex();

		CreateVertex();
	}

	// 面構造体
	struct Face {
		uint32_t idx[3];
	};

	// 面データ		
	std::vector<Face> m_faces;

private:
	DirectX::SimpleMath::Color m_color;
	float m_radius = 0.0f;
	uint32_t m_division_x = 0;
	uint32_t m_division_y = 0;
};
