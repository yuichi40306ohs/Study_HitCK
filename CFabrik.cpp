#include	"CFabrik.h"


CFabrik::Segment::Segment() {
	m_startpoint = DirectX::SimpleMath::Vector3(0, 0, 0);		//　開始位置
	m_endpoint = DirectX::SimpleMath::Vector3(0, 0, 0);			//　終了位置
	m_direction = DirectX::SimpleMath::Vector3(0, 0, 0);		//　方向
}

CFabrik::Segment::Segment(DirectX::SimpleMath::Vector3 startpoint, float len) {
	m_length = len;
	m_startpoint = startpoint;
}

CFabrik::Segment::Segment(Segment* parent) {
	m_length = parent->GetLength();
	m_startpoint = parent->GetStartPoint();
}

DirectX::SimpleMath::Vector3 CFabrik::Segment::GetStartPoint() {
	return m_startpoint;
}

DirectX::SimpleMath::Vector3 CFabrik::Segment::GetEndPoint() {
	return m_endpoint;
}

float CFabrik::Segment::GetLength() {
	return m_length;
}

void CFabrik::Segment::SetStartPoint(DirectX::SimpleMath::Vector3 sp) {
	m_startpoint = sp;
	CalculateEndPoint();				// 終了位置を求める
}

void CFabrik::Segment::CalculateEndPoint() {

	DirectX::SimpleMath::Vector3 direction;
	direction = m_endpoint - m_startpoint;

	// 正規化
	direction.Normalize();

	// 終了位置を求める
	m_endpoint = m_startpoint + m_direction * m_length;
}

void CFabrik::Segment::Update(DirectX::SimpleMath::Vector3 nowpoint) {

	// 終点を指定の座標にする
	m_endpoint = nowpoint;

	// 方向を求める
	DirectX::SimpleMath::Vector3 direction;
	direction = m_endpoint - m_startpoint;

	direction.Normalize();

	m_direction = direction;
	// 開始位置を求める
	m_startpoint = m_endpoint - m_direction * m_length;
}

// 描画
void CFabrik::Segment::Draw() {

	DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 0, 1);

	SphereDrawerDraw(
		5, color, m_startpoint.x, m_startpoint.y, m_startpoint.z);
}

CFabrik::CFabrik() {}

// コンストラクタ(同じ長さ)
CFabrik::CFabrik(DirectX::SimpleMath::Vector3 root, float segmentlength, int segmentnum) {

	m_basepoint = root;

	for (int i = 0; i < segmentnum; i++) {
		std::unique_ptr<Segment> seg = std::make_unique<Segment>(m_basepoint, segmentlength);
		m_segments.push_back(std::move(seg));
	}
}

void CFabrik::Update(DirectX::SimpleMath::Vector3 targetpos)
{
	size_t endidx = m_segments.size() - 1;

	// エフェクタを更新
	m_segments[endidx]->Update(targetpos);

	// 子供セグメントを更新
	for (int i = static_cast<int>(endidx - 1); i >= 0; i--)
	{
		// 子供セグメントの関節位置を目的位置として更新
		m_segments[i]->Update(m_segments[i + 1]->GetStartPoint());
	}
}

void CFabrik::Draw()
{
	// rootセグメントの関節を固定
	m_segments[0]->SetStartPoint(m_basepoint);

	// backwards
	for (int i = 1; i < m_segments.size(); i++) {
		m_segments[i]->SetStartPoint(m_segments[i - 1]->GetEndPoint());
	}

	// セグメント描画
	for (auto& seg : m_segments) {
		seg->Draw();
	}
}
