#include	"CFabrik.h"


CFabrik::Segment::Segment() {
	m_startpoint = DirectX::SimpleMath::Vector3(0, 0, 0);		//�@�J�n�ʒu
	m_endpoint = DirectX::SimpleMath::Vector3(0, 0, 0);			//�@�I���ʒu
	m_direction = DirectX::SimpleMath::Vector3(0, 0, 0);		//�@����
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
	CalculateEndPoint();				// �I���ʒu�����߂�
}

void CFabrik::Segment::CalculateEndPoint() {

	DirectX::SimpleMath::Vector3 direction;
	direction = m_endpoint - m_startpoint;

	// ���K��
	direction.Normalize();

	// �I���ʒu�����߂�
	m_endpoint = m_startpoint + m_direction * m_length;
}

void CFabrik::Segment::Update(DirectX::SimpleMath::Vector3 nowpoint) {

	// �I�_���w��̍��W�ɂ���
	m_endpoint = nowpoint;

	// ���������߂�
	DirectX::SimpleMath::Vector3 direction;
	direction = m_endpoint - m_startpoint;

	direction.Normalize();

	m_direction = direction;
	// �J�n�ʒu�����߂�
	m_startpoint = m_endpoint - m_direction * m_length;
}

// �`��
void CFabrik::Segment::Draw() {

	DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 0, 1);

	SphereDrawerDraw(
		5, color, m_startpoint.x, m_startpoint.y, m_startpoint.z);
}

CFabrik::CFabrik() {}

// �R���X�g���N�^(��������)
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

	// �G�t�F�N�^���X�V
	m_segments[endidx]->Update(targetpos);

	// �q���Z�O�����g���X�V
	for (int i = static_cast<int>(endidx - 1); i >= 0; i--)
	{
		// �q���Z�O�����g�̊֐߈ʒu��ړI�ʒu�Ƃ��čX�V
		m_segments[i]->Update(m_segments[i + 1]->GetStartPoint());
	}
}

void CFabrik::Draw()
{
	// root�Z�O�����g�̊֐߂��Œ�
	m_segments[0]->SetStartPoint(m_basepoint);

	// backwards
	for (int i = 1; i < m_segments.size(); i++) {
		m_segments[i]->SetStartPoint(m_segments[i - 1]->GetEndPoint());
	}

	// �Z�O�����g�`��
	for (auto& seg : m_segments) {
		seg->Draw();
	}
}
