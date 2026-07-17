//--------------------------------------------------------------------------------------
// File: EnemyTask.h
//
// 敵タスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/Task.h"
#include "GameContext.h"
#include "GameTypes.h"

class EnemyTask : public Imase::Task, public IFaction
{
public:

	// 大きさ(dot)
	static constexpr int SIZE = 64;

	// 移動の速さ(dot/s)
	static constexpr int SPEED = 300;

	// 弾の発射の速さ(dot/s)
	static constexpr int BULLET_SPEED = 500;

private:

	// 弾の発射間隔
	static constexpr float SHOOT_INTERVAL = 1.0f;

public:

	// コンストラクタ
	EnemyTask(
		GameContext* pGameContext,
		DirectX::SpriteBatch* pSpriteBatch,
		ID3D11ShaderResourceView* pTexture,
		DirectX::SimpleMath::Vector2 position
	);

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 当たり判定用のグループを取得する関数
	Faction GetFaction() const override
	{
		return Faction::Enemy;
	}

	// 境界を取得する関数
	RECT GetBoundingRect() const;

	// 爆発エフェクトを発生する関数
	void Explotion();

private:

	// ゲームコンテキストへのポインタ
	GameContext* m_pGameContext;

	// スプライトバッチへのポインタ
	DirectX::SpriteBatch* m_pSpriteBatch;

	// テクスチャへのポインタ
	ID3D11ShaderResourceView* m_pTexture;

	// 位置
	DirectX::SimpleMath::Vector2 m_position = { 0.0f, 0.0f };

	// 弾の発射用のタイマー
	float m_shootTimer = 0.0f;
};