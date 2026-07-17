//--------------------------------------------------------------------------------------
// File: ExplosionTask.cpp
//
// 爆発エフェクトタスク
//
// Date: 2026.4.10
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ExplosionTask.h"

using namespace DirectX;

// コンストラクタ
ExplosionTask::ExplosionTask(
    GameContext* pGameContext,
    DirectX::SpriteBatch* pSpriteBatch,
    ID3D11ShaderResourceView* pTexture,
    DirectX::SimpleMath::Vector2 position
)
    : m_pGameContext(pGameContext)
    , m_pSpriteBatch(pSpriteBatch)
    , m_pTexture(pTexture)
    , m_position(position)
{
    // タグを設定
    SetTag(L"Explosion");
}

// 更新
bool ExplosionTask::Update(float elapsedTime)
{
    // アニメーションタイマーを加算
    m_animationTimer += elapsedTime;
    if (m_animationTimer > ANIMATION_INTERVAL)
    {
        m_animationTimer = 0.0f;
        m_animation++;
        if (m_animation == 4) return false;
    }

    return true;
}

// 描画
void ExplosionTask::Render()
{
    // 爆発の描画（4倍の大きさで表示）
    m_pSpriteBatch->Draw(
        m_pTexture,
        m_position,
        &ExplosionSpritesRect[m_animation],
        Colors::White,
        0.0f,
        { 0.0f, 0.0f },
        4.0f
    );
}
