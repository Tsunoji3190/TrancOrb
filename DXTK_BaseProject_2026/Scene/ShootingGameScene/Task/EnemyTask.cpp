//--------------------------------------------------------------------------------------
// File: EnemyTask.cpp
//
// 敵タスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "EnemyTask.h"
#include "BulletTask.h"
#include "ExplosionTask.h"

using namespace DirectX;

// コンストラクタ
EnemyTask::EnemyTask(
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
    SetTag(L"Enemy");
}

// 更新
bool EnemyTask::Update(float elapsedTime)
{
    // 画面サイズ取得
    RECT rect = m_pGameContext->deviceResources.GetOutputSize();

    // 下へ移動
    m_position.y += SPEED * elapsedTime;

    // 画面外へ出たらタスクを消去
    float moveBottomMax = static_cast<float>(rect.bottom);
    if (m_position.y > moveBottomMax)
    {
        return false;
    }

    // 弾の発射用タイマーを加算
    m_shootTimer += elapsedTime;
    if (m_shootTimer > SHOOT_INTERVAL)
    {
        // 弾の発射用タイマーの初期化
        m_shootTimer = 0.0f;

        // 弾タスクを生成
        GetParent()->AddChild<BulletTask>(
            m_pGameContext,
            m_pSpriteBatch,
            m_pTexture,
            // 敵の中央下から弾を発射
            SimpleMath::Vector2(
                m_position.x + (EnemyTask::SIZE - BulletTask::SIZE) / 2,
                m_position.y + (EnemyTask::SIZE - BulletTask::SIZE)
            ),
            SimpleMath::Vector2(0.0f, BULLET_SPEED),
            Faction::Enemy
        );
    }

    return true;
}

// 描画
void EnemyTask::Render()
{
    // テクスチャ上の矩形
    RECT srcRect = { 96, 0, 96 + 32, 32 };

    // 敵の描画（2倍の大きさで表示）
    m_pSpriteBatch->Draw(m_pTexture, m_position, &srcRect, Colors::White, 0.0f, { 0.0f, 0.0f }, 2.0f);
}

// 境界を取得する関数
RECT EnemyTask::GetBoundingRect() const
{
    RECT rect{};

    rect.left = static_cast<LONG>(m_position.x);
    rect.right = static_cast<LONG>(m_position.x + EnemyTask::SIZE);
    rect.top = static_cast<LONG>(m_position.y);
    rect.bottom = static_cast<LONG>(m_position.y + EnemyTask::SIZE);

    return rect;
}

// 爆発エフェクトを発生する関数
void EnemyTask::Explotion()
{
    // 爆発エフェクトタスクを生成
    GetParent()->AddChild<ExplosionTask>(
        m_pGameContext,
        m_pSpriteBatch,
        m_pTexture,
        m_position
    );
}
