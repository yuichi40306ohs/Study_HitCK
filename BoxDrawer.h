#pragma once

#include	<simplemath.h>

void BoxDrawerInit();
void BoxDrawerDraw(
	float width, float height, float depth,
	DirectX::SimpleMath::Color col, float posx, float posy, float posz);
void BoxDrawerDraw(SRT srt, DirectX::SimpleMath::Color col);
void BoxDrawerDraw(DirectX::SimpleMath::Matrix mtx, DirectX::SimpleMath::Color col);

