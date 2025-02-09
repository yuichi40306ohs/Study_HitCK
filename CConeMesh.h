#pragma once
#include	<simplemath.h>
#include	<random>
#include	"CMesh.h"

class CConeMesh : public CMesh {
	struct Face {
		unsigned int idx[3];
	};
public:
	void Init(
		int		divx,							// 分割数			
		float	radius,							// 半径	
		float	height,							// 高さ	
		DirectX::SimpleMath::Color color);		// 頂点カラー

	void CreateVertex();						// 頂点データ生成

//	void CreateIndex();							// インデックスを生成

	// 面データ		
	std::vector<Face> m_faces;
private:
	unsigned int m_division_x = 1;
	unsigned int m_division_y = 1;

	float  m_radius = 100.0f;					// 半径	

	float  m_height = 100.0f;
	float  m_width;								// 半径から自動計算する

	DirectX::SimpleMath::Color m_color;			// 頂点カラー
};