//--------------------------------------------------------------------------------------
// File: ExplosionTask.h
//
// 爆発エフェクトタスク
//
// Date: 2026.4.10
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/Task.h"
#include "GameContext.h"

class ExplosionTask : public Imase::Task
{
private:

	// 爆発の時間
	static constexpr float ANIMATION_TIME = 0.3f;

	// 爆発アニメーションの間隔
	static constexpr float ANIMATION_INTERVAL = ANIMATION_TIME / 4.0f;

	// 爆発のスプライトシート上の絵の矩形
	static constexpr RECT ExplosionSpritesRect[] =
	{
		{ 96 + 16 * 0, 32, 96 + 16 * 1, 32 + 16,},		// パターン１
		{ 96 + 16 * 1, 32, 96 + 16 * 2, 32 + 16,},		// パターン２
		{ 96 + 16 * 2, 32, 96 + 16 * 3, 32 + 16,},		// パターン３
		{ 96 + 16 * 3, 32, 96 + 16 * 4, 32 + 16,},		// パターン４
	};

public:

	// コンストラクタ
	ExplosionTask(
		GameContext* pGameContext,
		DirectX::SpriteBatch* pSpriteBatch,
		ID3D11ShaderResourceView* pTexture,
		DirectX::SimpleMath::Vector2 position
	);

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

private:

	// ゲームコンテキストへのポインタ
	GameContext* m_pGameContext;

	// スプライトバッチへのポインタ
	DirectX::SpriteBatch* m_pSpriteBatch;

	// テクスチャへのポインタ
	ID3D11ShaderResourceView* m_pTexture = nullptr;

	// 位置
	DirectX::SimpleMath::Vector2 m_position = { 0.0f, 0.0f };

	// アニメーション
	int m_animation = 0;

	// アニメーションタイマー
	float m_animationTimer = 0.0f;

};