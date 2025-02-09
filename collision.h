#pragma once

#include	<simplemath.h>
#include	<algorithm>

// �~����`
struct BoundingCylinder {
	DirectX::SimpleMath::Vector3    bottom;						// ��
	DirectX::SimpleMath::Vector3    top;						// ��
	float	radius;												// ���a
};

// ����`
struct BoundingSphere {
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// �Z�O�����g�i�����j
struct Segment {
	DirectX::SimpleMath::Vector3		startpoint;				// �J�n�_
	DirectX::SimpleMath::Vector3		endpoint;				// �I���_
};

// ���E�{�b�N�XAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
};

// ���E�{�b�N�XOBB
struct BoundingBoxOBB : public BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// ���S���iX���AY���AZ���j
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOX�T�C�Y
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB
bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2);

// make AABB
BoundingBoxAABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width);

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3 up,
	DirectX::SimpleMath::Vector3* v,
	float width);

// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const DirectX::SimpleMath::Vector3& p,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// �~���Ƌ��̓����蔻��
bool CollisionSphereCylinder(
	BoundingSphere sphere, 
	BoundingCylinder cylinder);

// AABB�Ɠ_�̍ŋߐړ_�����߂�
void ClosestPtPointAABB(DirectX::SimpleMath::Vector3 p, BoundingBoxAABB aabb, DirectX::SimpleMath::Vector3& q);

// AABB�Ɠ_�̋��������߂�
float SqDistPointAABB(DirectX::SimpleMath::Vector3 p, BoundingBoxAABB aabb);

// AABB�Ƌ��̓����蔻��
bool CollisionSphereAABB(
	BoundingSphere sphere,
	BoundingBoxAABB aabb);

// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
void ClosestPtPointOBB(DirectX::SimpleMath::Vector3 point, BoundingBoxOBB obb, DirectX::SimpleMath::Vector3& answer);

// OBB�Ɠ_�̋��������߂�
float DistPointOBB(DirectX::SimpleMath::Vector3 point, BoundingBoxOBB obb);

BoundingBoxOBB SetOBB(
	DirectX::SimpleMath::Vector3 rot,					// �p���i��]�p�x�j
	DirectX::SimpleMath::Vector3 pos,					// ���S���W�i���[���h�j
	float width,					// ��
	float height,					// ����
	float depth);					// ���s��

// OBB�Ƌ��̓����蔻��
bool CollisionSphereOBB(
	BoundingSphere sphere,
	BoundingBoxOBB obb);
