#include	"CSphereMesh.h"

using namespace DirectX::SimpleMath;

// インデックスデータを作成
void CSphereMesh::CreateIndex() {

	// インデックス生成
	for (unsigned int y = 0; y < m_division_y; y++) {
		for (unsigned int x = 0; x < m_division_x; x++) {
			int count = (m_division_x + 1) * y + x;			// 左上座標のインデックス

			Face tempface{};

			// 上半分
			tempface.idx[0] = count;
			tempface.idx[1] = count + 1;
			tempface.idx[2] = count + 1 + (m_division_x + 1);

			m_faces.emplace_back(tempface);

			// 下半分
			tempface.idx[0] = count;
			tempface.idx[1] = count + (m_division_x + 1) + 1;
			tempface.idx[2] = count + (m_division_x + 1);

			m_faces.emplace_back(tempface);
		}
	}
	
	m_indices.clear();

	for(auto& f : m_faces) {
		m_indices.emplace_back(f.idx[0]);
		m_indices.emplace_back(f.idx[1]);
		m_indices.emplace_back(f.idx[2]);
	}
}

// 頂点データを作成
void CSphereMesh::CreateVertex() {

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_division_y; y++) {

		// 仰角計算
		elevation = (DirectX::XM_PI * (float)y) / (float)m_division_y;			// 仰角をセット

		float r = m_radius * sinf(elevation);									// 仰角に応じた半径を計算
			
		for (unsigned int x = 0; x <= m_division_x; x++) {

			azimuth = (2 * DirectX::XM_PI * (float)x) / (float)m_division_x;	// 方位角をセット

			VERTEX_3D	v;

			// 頂点座標セット
			v.Position.x = r * cosf(azimuth);
			v.Position.y = m_radius * cosf(elevation);
			v.Position.z = r * sinf(azimuth);

			v.Normal = v.Position;		// 法線ベクトルを頂点座標と同じにする

			v.Diffuse = m_color;

			// 法線ベクトル
			v.Normal.Normalize(v.Normal);	// 法線を正規化

			// 頂点登録
			m_vertices.emplace_back(v);
		}
	}
}