#include	<SimpleMath.h>
#include	<vector>
#include	<memory>
#include	"sphereDrawer.h"

class CFabrik {

	class Segment {
		float m_length = 0.0f;
		DirectX::SimpleMath::Vector3 m_startpoint;		//�@�J�n�ʒu
		DirectX::SimpleMath::Vector3 m_endpoint;		//�@�I���ʒu
		DirectX::SimpleMath::Vector3 m_direction;		//�@����
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

		// �`��
		void Draw();
	};

	std::vector<std::unique_ptr<Segment>> m_segments;	//�@�Z�O�����g�Q
	DirectX::SimpleMath::Vector3 m_basepoint;			//�@��_

public:
	CFabrik();

	// �R���X�g���N�^(��������)
	CFabrik(DirectX::SimpleMath::Vector3 root, float segmentlength, int segmentnum);
	void Update(DirectX::SimpleMath::Vector3 targetpos);
	void Draw();

};