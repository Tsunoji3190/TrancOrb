//--------------------------------------------------------------------------------------
// File: BulletTask.cpp
//
// 弾タスク
//
// Date: 2026.4.5
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "BulletTask.h"
#include "PlayerTask.h"

// コンストラクタ
BulletTask::BulletTask(
    GameContext* pGameContext,
    DirectX::SpriteBatch* pSpriteBatch,
    ID3D11ShaderResourceView* pTexture,
    DirectX::SimpleMath::Vector2 position,
    DirectX::SimpleMath::Vector2 velocity,
    Faction faction
)
    : m_pGameContext(pGameContext)
    , m_pSpriteBatch(pSpriteBatch)
    , m_pTexture(pTexture)
    , m_position(position)
    , m_velocity(velocity)
    , m_faction(faction)
{
    // タグを設定
    SetTag(L"Bullet");
}

// デストラクタ
BulletTask::~BulletTask()
{
    if (m_onDestroy)
    {
        // 弾が消滅した時に呼び出される関数を実行
        m_onDestroy();
    }
}

// 更新
bool BulletTask::Update(float elapsedTime)
{
    // 移動
    m_position += m_velocity * elapsedTime;

    // 画面外に弾が出たか？
    RECT rect = m_pGameContext->deviceResources.GetOutputSize();
    float moveUpMax = static_cast<float>(rect.top - BulletTask::SIZE);
    float moveDownMax = static_cast<float>(rect.bottom);
    if ( (m_position.y < moveUpMax)		// 画面外に出たら（上部）
      || (m_position.y > moveDownMax)	// 画面外に出たら（下部）
       )
    {
        // 弾を消去
        return false;
    }
    return true;
}

// 描画
void BulletTask::Render()
{
    // プレイヤーの弾の絵
    RECT srcRect = { 62, 32, 62 + 16, 32 + 16 };

    // 敵の弾の絵
    if (m_faction == Faction::Enemy)
    {
        srcRect = RECT{ 62 + 16, 32, 62 + 32, 32 + 16 };
    }

    // 弾の描画
    m_pSpriteBatch->Draw(m_pTexture, m_position, &srcRect);
}

// 境界を取得する関数
RECT BulletTask::GetBoundingRect() const
{
    RECT rect{};

    rect.left = static_cast<LONG>(m_position.x);
    rect.right = static_cast<LONG>(m_position.x + BulletTask::SIZE);
    rect.top = static_cast<LONG>(m_position.y);
    rect.bottom = static_cast<LONG>(m_position.y + BulletTask::SIZE);

    return rect;
}
