//--------------------------------------------------------------------------------------
// File: PlayerTask.cpp
//
// プレイヤータスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "PlayerTask.h"
#include "BulletTask.h"
#include "ExplosionTask.h"

using namespace DirectX;

// コンストラクタ
PlayerTask::PlayerTask(
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
    SetTag(L"Player");
}

// 更新
bool PlayerTask::Update(float elapsedTime)
{
    auto kb = Keyboard::Get().GetState();
    
    // 左キー
    if (kb.Left)
    {
        // 左に移動
        m_position.x -= SPEED * elapsedTime;
    }

    // 右キー
    if (kb.Right)
    {
        // 右に移動
        m_position.x += SPEED * elapsedTime;
    }

    // Zキーで弾を発射
    if (m_pGameContext->keyboardTracker.pressed.Z)
    {
        // 弾数制限
        if (m_bulletCount < MAX_BULLET)
        {
            // 弾タスクを生成
            BulletTask* bullet = GetParent()->AddChild<BulletTask>(
                m_pGameContext,
                m_pSpriteBatch,
                m_pTexture,
                SimpleMath::Vector2(m_position.x + (PlayerTask::SIZE - BulletTask::SIZE) / 2, m_position.y),
                SimpleMath::Vector2(0.0f, -BULLET_SPEED),
                Faction::Player
            );
            // 弾タスクが消滅した時に呼び出される関数を登録
            bullet->SetOnDestroy([this]()
                {
                    // 弾の発射数を減算する
                    m_bulletCount--;
                });
            // 弾の発射数を加算
            m_bulletCount++;
        }
    }

    // 画面外へ行かないようにX座標を補正
    RECT rect = m_pGameContext->deviceResources.GetOutputSize();
    float moveRightMax = static_cast<float>(rect.left);
    if (m_position.x < moveRightMax)
    {
        m_position.x = moveRightMax;
    }
    float moveLeftMax = static_cast<float>(rect.right - PlayerTask::SIZE);
    if (m_position.x > moveLeftMax)
    {
        m_position.x = moveLeftMax;
    }

    return true;
}

// 描画
void PlayerTask::Render()
{
    // テクスチャ上の矩形
    RECT srcRect = { 0, 0, 32, 32 };

    // プレイヤーの描画（2倍の大きさで表示）
    m_pSpriteBatch->Draw(m_pTexture, m_position, &srcRect, Colors::White, 0.0f, { 0.0f, 0.0f }, 2.0f);
}

// 境界を取得する関数
RECT PlayerTask::GetBoundingRect() const
{
    RECT rect{};

    rect.left = static_cast<LONG>(m_position.x);
    rect.right = static_cast<LONG>(m_position.x + PlayerTask::SIZE);
    rect.top = static_cast<LONG>(m_position.y);
    rect.bottom = static_cast<LONG>(m_position.y + PlayerTask::SIZE);

    return rect;
}

// 爆発エフェクトを発生する関数
void PlayerTask::Explotion()
{
    // 爆発エフェクトタスクを生成
    GetParent()->AddChild<ExplosionTask>(
        m_pGameContext,
        m_pSpriteBatch,
        m_pTexture,
        m_position
    );
}
