//--------------------------------------------------------------------------------------
// File: CollisionManager.h
//
// 当たり判定を行うクラス
//
// Date: 2026.4.11
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/TaskSystem.h"

class CollisionManager
{
private:

	// 矩形同士の衝突判定
	bool IsColliding(RECT a, RECT b);

	// プレイヤーの弾と敵の当たり判定
	void PlayerBulletVsEnemy(Imase::TaskSystem& taskSystem);

	// 敵の弾とプレイヤーの当たり判定
	void EnemyBulletVsPlayer(Imase::TaskSystem& taskSystem);

public:

	// 当たり判定
	void Check(Imase::TaskSystem& taskSystem);

};
