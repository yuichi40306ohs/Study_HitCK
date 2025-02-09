#include	<SimpleMath.h>
#include	<vector>
#include	<memory>
#include	"sphereDrawer.h"

class CFabrik {

	class Segment {
		float m_length = 0.0f;
		DirectX::SimpleMath::Vector3 m_startpoint;		//　開始位置
		DirectX::SimpleMath::Vector3 m_endpoint;		//　終了位置
		DirectX::SimpleMath::Vector3 m_direction;		//　方向
	public:
		Segment();

		Segment(DirectX::SimpleMath::Vector3 startpoint, float len);

		Segment(Segment* parent);

		DirectX::SimpleMath::Vector3 GetStartPoint();

		DirectX::SimpleMath::Vector3 GetEndPoint();

		float GetLength();

		void SetStartPoint(DirectX::SimpleMath::Vector3 sp);

		void CalculateEndPoint();

		void Update(DirectX::SimpleMath::Vector3 nowpoint);

		// 描画
		void Draw();
	};

	std::vector<std::unique_ptr<Segment>> m_segments;	//　セグメント群
	DirectX::SimpleMath::Vector3 m_basepoint;			//　基準点

public:
	CFabrik();

	// コンストラクタ(同じ長さ)
	CFabrik(DirectX::SimpleMath::Vector3 root, float segmentlength, int segmentnum);
	void Update(DirectX::SimpleMath::Vector3 targetpos);
	void Draw();

};