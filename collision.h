#pragma once

#include	<simplemath.h>
#include	<algorithm>

// 円柱定義
struct BoundingCylinder {
	DirectX::SimpleMath::Vector3    bottom;						// 底
	DirectX::SimpleMath::Vector3    top;						// 上
	float	radius;												// 半径
};

// 球定義
struct BoundingSphere {
	DirectX::SimpleMath::Vector3	center;
	float	radius;
};

// セグメント（線分）
struct Segment {
	DirectX::SimpleMath::Vector3		startpoint;				// 開始点
	DirectX::SimpleMath::Vector3		endpoint;				// 終了点
};

// 境界ボックスAABB
struct BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 min;
	DirectX::SimpleMath::Vector3 max;
};

// 境界ボックスOBB
struct BoundingBoxOBB : public BoundingBoxAABB {
	DirectX::SimpleMath::Vector3 center;
	DirectX::SimpleMath::Vector3 worldcenter;

	// 中心軸（X軸、Y軸、Z軸）
	DirectX::SimpleMath::Vector3 axisX;
	DirectX::SimpleMath::Vector3 axisY;
	DirectX::SimpleMath::Vector3 axisZ;

	// BOXサイズ
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

// 球
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB
bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2);

// make AABB
BoundingBoxAABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, int height, int depth);

// 直線と点の距離を求める
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width);

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3 up,
	DirectX::SimpleMath::Vector3* v,
	float width);

// 直線と点の距離を求める
float calcPointLineDist(
	const DirectX::SimpleMath::Vector3& point,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 線分と直線の長さを求める
float calcPointSegmentDist(
	const DirectX::SimpleMath::Vector3& p,
	const Segment& segment,
	DirectX::SimpleMath::Vector3& intersectionpoint,
	float& t);

// 円柱と球の当たり判定
bool CollisionSphereCylinder(
	BoundingSphere sphere, 
	BoundingCylinder cylinder);

// AABBと点の最近接点を求める
void ClosestPtPointAABB(DirectX::SimpleMath::Vector3 p, BoundingBoxAABB aabb, DirectX::SimpleMath::Vector3& q);

// AABBと点の距離を求める
float SqDistPointAABB(DirectX::SimpleMath::Vector3 p, BoundingBoxAABB aabb);

// AABBと球の当たり判定
bool CollisionSphereAABB(
	BoundingSphere sphere,
	BoundingBoxAABB aabb);

// OBBと点のもっと近い点を求める
void ClosestPtPointOBB(DirectX::SimpleMath::Vector3 point, BoundingBoxOBB obb, DirectX::SimpleMath::Vector3& answer);

// OBBと点の距離を求める
float DistPointOBB(DirectX::SimpleMath::Vector3 point, BoundingBoxOBB obb);

BoundingBoxOBB SetOBB(
	DirectX::SimpleMath::Vector3 rot,					// 姿勢（回転角度）
	DirectX::SimpleMath::Vector3 pos,					// 中心座標（ワールド）
	float width,					// 幅
	float height,					// 高さ
	float depth);					// 奥行き

// OBBと球の当たり判定
bool CollisionSphereOBB(
	BoundingSphere sphere,
	BoundingBoxOBB obb);
