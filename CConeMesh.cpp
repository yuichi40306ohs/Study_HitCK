#include	<iostream>
#include	"CConeMesh.h"

// 頂点データを作成
void CConeMesh::Init(
	int		divx,							// 分割数			
	float	radius,							// 半径	
	float	height,							// 高さ	
	DirectX::SimpleMath::Color color)		// 頂点カラー
{
	// サイズセット（幅と高さ）（XY平面）
	m_height = height;
	m_width = 2.0f*DirectX::XM_PI*radius;		// 直径×円周率
	m_radius = radius;

	// 分割数
	m_division_x = divx;

	// 頂点カラー
	m_color = color;

	// 頂点データ生成
	CreateVertex();
}

void CConeMesh::CreateVertex() 
{
	// 頂点データクリア
	m_vertices.clear();

	// てっぺんの頂点	
	VERTEX_3D v{};

	v.Position = DirectX::SimpleMath::Vector3(0.0f, m_height, 0.0f);
	v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;

	m_vertices.emplace_back(v);

	// 底面の円周上の頂点データを作成
	float azimuth = 0.0f;			// 方位角
	for (unsigned int i = 0; i <= m_division_x; i++) {

		azimuth = (2 * DirectX::XM_PI * (float)i) / (float)m_division_x;	// 方位角をセット

		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = 0.0f;
		v.Position.z = m_radius * sinf(azimuth);	

		v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);		// 法線ベクトルを頂点座標と同じにする	
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// 頂点から各セグメントの底面の頂点へのインデックスを追加
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(0);						// 頂点
		m_indices.push_back(i + 2);					// 次の底面頂点
		m_indices.push_back(i + 1);					// 現在の底面頂点
	}

	// 底面の中心座標
	v.Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	v.Normal = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;
	m_vertices.emplace_back(v);

	int bottomcenteridx = static_cast<int>(m_vertices.size() - 1);	// 底面の中心座標のインデックス

	// 底面の三角形をインデックス化
	for (unsigned int i = 0; i <= m_division_x; i++) {
		m_indices.push_back(bottomcenteridx);		// 底面の中心
		m_indices.push_back(i + 1);					// 次の底面頂点
		m_indices.push_back(i + 2);					// 現在の底面頂点
	}
}


